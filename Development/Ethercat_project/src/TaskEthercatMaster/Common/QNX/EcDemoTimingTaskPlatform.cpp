/*-----------------------------------------------------------------------------
 * EcDemoTimingTaskPlatform.cpp
 * Copyright                acontis technologies GmbH, Ravensburg, Germany
 * Response                 Timo Nussbaumer
 * Description              EtherCAT demo timing task
 *----------------------------------------------------------------------------*/

/*-LOGGING-------------------------------------------------------------------*/
#define pEcLogParms G_pEcLogParms

/*-INCLUDES------------------------------------------------------------------*/
#include "EcDemoApp.h"

#include <time.h>
#if (EC_VERSION_QNX >= 700)
    #include <sys/procmgr.h>
    #include <sys/netmgr.h>
#endif /* EC_VERSION_QNX >= 700 */
#if !(defined ND_LOCAL_NODE)
    #define ND_LOCAL_NODE 0
#endif

#define NSEC_PER_SEC                (1000000000)

CDemoTimingTaskPlatform::CDemoTimingTaskPlatform()
    : TBaseClass()
    , m_TimerId()
    , m_nTimerChannel()
{
}

CDemoTimingTaskPlatform::CDemoTimingTaskPlatform(_T_EC_DEMO_APP_CONTEXT& rAppContext)
    : TBaseClass(rAppContext)
    , m_TimerId()
    , m_nTimerChannel()
{
}

EC_T_DWORD CDemoTimingTaskPlatform::StartTimingTask(EC_T_INT nCycleTimeNsec)
{
    EC_T_INT nRes = 0;
    EC_T_DWORD dwTimeout = 0;
    m_nCycleTimeNsec = EC_MAX(nCycleTimeNsec, 1000);
    m_nOriginalCycleTimeNsec = m_nCycleTimeNsec;
    m_bShutdown = EC_FALSE;
    m_bIsRunning = EC_FALSE;

    EC_T_DWORD ret = this->CreateTimingEvent();
    if (ret != EC_E_NOERROR)
    {
        return ret;
    }

#if (EC_VERSION_QNX < 800)
    /* calculate and set clock period */
    {
        _clockperiod newClockPeriod;
        OsMemset(&newClockPeriod, 0, sizeof(newClockPeriod));

#if (EC_VERSION_QNX >= 700)
        nRes = procmgr_ability(0, PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_CLOCKPERIOD, PROCMGR_AID_EOL);
        if (nRes != 0)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: procmgr_ability PROCMGR_AID_CLOCKPERIOD failed (%d)\n", nRes));
            return EC_E_ACCESSDENIED;
        }
#endif
        newClockPeriod.nsec = EC_AT_MOST(m_nCycleTimeNsec / 2, 500000);
        nRes = ClockPeriod(CLOCK_REALTIME, &newClockPeriod, EC_NULL, 0);
        if (nRes != 0)
        {
            EcLogMsg(EC_LOG_LEVEL_WARNING, (pEcLogContext, EC_LOG_LEVEL_WARNING, "WARNING: Set clock period failed (%d)\n", nRes));
        }
    }
#endif
    /* create, attach communication channel and create timer */
    {
        sigevent timerSigEvent;
        OsMemset(&timerSigEvent, 0, sizeof(timerSigEvent));

        m_nTimerChannel = ChannelCreate(0);
        if (m_nTimerChannel == -1)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: Create timer channel failed (%d)\n", nRes));
            return EC_E_ERROR;
        }

        timerSigEvent.sigev_notify = SIGEV_PULSE;
        timerSigEvent.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, m_nTimerChannel, _NTO_SIDE_CHANNEL, 0);
        if (timerSigEvent.sigev_coid == -1)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: Attach timer channel failed (%d)\n", nRes));
            return EC_E_ERROR;
        }

        /* create timer */
#if (EC_VERSION_QNX >= 700)
        nRes = procmgr_ability(0, PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_TIMER, PROCMGR_AID_EOL);
        if (nRes != 0)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: procmgr_ability PROCMGR_AID_TIMER failed (%d)\n", nRes));
            return EC_E_ACCESSDENIED;
        }
#endif
        timerSigEvent.sigev_priority = TIMER_THREAD_PRIO;
        timerSigEvent.sigev_code = _PULSE_CODE_MINAVAIL;
        nRes = timer_create(CLOCK_MONOTONIC, &timerSigEvent, &m_TimerId);
        if (nRes != 0)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: Create timer failed (%d)\n", nRes));
            return EC_E_ERROR;
        }
    }
#if (EC_VERSION_QNX >= 700)
    /* use high resolution timer by set timer tolerance */
    {
        itimerspec itime;
        OsMemset(&itime, 0, sizeof(itime));

        itime.it_value.tv_nsec = 1;
        nRes = timer_settime(m_TimerId, TIMER_TOLERANCE, &itime, EC_NULL);
        if (nRes != 0)
        {
            EcLogMsg(EC_LOG_LEVEL_ERROR, (pEcLogContext, EC_LOG_LEVEL_ERROR, "ERROR: Set timer tolerance failed (%d)\n", nRes));
            return EC_E_ERROR;
        }
    }
#endif
    ret = this->CreateThread();
    if (ret != EC_E_NOERROR)
    {
        return ret;
    }

    for (dwTimeout = 5000; 0 != dwTimeout; dwTimeout--)
    {
        if (m_bIsRunning)
        {
            break;
        }
        OsSleep(1);
    }
    if (!m_bIsRunning)
    {
        return EC_E_TIMEOUT;
    }

    return EC_E_NOERROR;
}

EC_T_DWORD CDemoTimingTaskPlatform::StopTimingTask()
{
    CDemoTimingTask::StopTimingTask();
    timer_delete(m_TimerId);
    ChannelDestroy(m_nTimerChannel);
    return EC_E_NOERROR;
}

EC_T_VOID CDemoTimingTaskPlatform::TimingTask()
{
    EC_T_INT nRes = 0;
    EC_T_CPUSET CpuSet;
    itimerspec itime;
    OsMemset(&itime, 0, sizeof(itime));

    EC_CPUSET_ZERO(CpuSet);
    EC_CPUSET_SET(CpuSet, m_dwCpuIndex);
    OsSetThreadAffinity(EC_NULL, CpuSet);

    /* get current time */
    {
        timespec t;
        OsMemset(&t, 0, sizeof(struct timespec));

        clock_gettime(CLOCK_MONOTONIC, &t);

        t.tv_nsec = t.tv_nsec + m_nCycleTimeNsec;

        /* norm time */
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec = t.tv_nsec - NSEC_PER_SEC;
            t.tv_sec++;
        }

        itime.it_value.tv_sec = t.tv_sec;
        itime.it_value.tv_nsec = t.tv_nsec;
    }
    /* start timer */
    nRes = timer_settime(m_TimerId, TIMER_ABSTIME, &itime, EC_NULL);
    if (nRes != 0)
    {
        return;
    }

    /* timing task started */
    m_bIsRunning = EC_TRUE;

    /* periodically generate events as long as the application runs */
    while (!m_bShutdown)
    {
        _pulse pulse;
        /* wait for next cycle */
        MsgReceive(m_nTimerChannel, &pulse, sizeof(pulse), NULL);

        /* trigger jobtask */
        this->SetTimingEvent();

        itime.it_value.tv_nsec = itime.it_value.tv_nsec + m_nCycleTimeNsec;
        /* norm time */
        while (itime.it_value.tv_nsec >= NSEC_PER_SEC)
        {
            itime.it_value.tv_nsec = itime.it_value.tv_nsec - NSEC_PER_SEC;
            itime.it_value.tv_sec++;
        }
        nRes = timer_settime(m_TimerId, TIMER_ABSTIME, &itime, EC_NULL);
        if (nRes != 0)
        {
            m_bShutdown = EC_TRUE;
        }
    }

    m_bIsRunning = EC_FALSE;
}

CDemoTimingTaskPlatform::~CDemoTimingTaskPlatform()
{
    CDemoTimingTaskPlatform::StopTimingTask();
}

EC_T_DWORD CDemoTimingTaskPlatform::GetHostTime(EC_T_VOID* pvContext, EC_T_UINT64* pnActualHostTimeInNsec)
{
#if 1
    return TBaseClass::GetHostTime(pvContext, pnActualHostTimeInNsec);
#else
    /* TBaseClass::GetHostTime calls OsSystemTimeGet and this implementation corresponds to */
    time_t ltime;
    time(&ltime);
    ltime = ltime - 946684800ul;
    *pnActualHostTimeInNsec = ((EC_T_UINT64)ltime)*1000000000ul;
#endif
}
