#include "EcStateClearingFault.h"

EcStateClearingFault::EcStateClearingFault(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateClearingFault::~EcStateClearingFault()
{
}

uint32_t EcStateClearingFault::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateClearingFaultNs::OnEntrySubroutine(0, "EcStateClearingFaultNs::OnEntrySubroutine"));
    addOnExit(new EcStateClearingFaultNs::OnExitSubroutine(0, "EcStateClearingFaultNs::OnExitSubroutine"));
    addCallback(new EcStateClearingFaultNs::CallbackSubroutine(0, "EcStateClearingFaultNs::CallbackSubroutine"));
    addTransition(new EcStateClearingFaultNs::TransitionSubroutine(0, "EcStateClearingFaultNs::TransitionSubroutine"));

    return dwRes;
}

EcStateClearingFaultNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingFaultNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingFaultNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingFaultNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingFaultNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingFaultNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->faultClear();

    return 0;
}

EcStateClearingFaultNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
    // m_dseEStopTrig("E_STOP_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseEStopClear("E_STOP_CLEAR", "/group_1/task/user/user1", false, true),
    // m_dseDisableTrig("DISABLE_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseClearFaultTrig("CLEAR_FAULT_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseClearQuickStopTrig("CLEAR_QUICKSTOP_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseEnableTrig("ENABLE_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseReadyTrig("READY_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseCustomStateTrig("CUSTOM_STATE_TRIG", "/group_1/task/user/user1", false, true),
    // m_dseStopTrig("STOP_TRIG", "/group_1/task/user/user1", false, true)
{
}

EcStateClearingFaultNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseEStopTrig", m_dseEStopTrig);
    registerXmlElementDataStoreValue("dseEStopClear", m_dseEStopClear);
    registerXmlElementDataStoreValue("dseDisableTrig", m_dseDisableTrig);
    registerXmlElementDataStoreValue("dseClearFaultTrig", m_dseClearFaultTrig);
    registerXmlElementDataStoreValue("dseClearQuickStopTrig", m_dseClearQuickStopTrig);
    registerXmlElementDataStoreValue("dseEnableTrig", m_dseEnableTrig);
    registerXmlElementDataStoreValue("dseReadyTrig", m_dseReadyTrig);
    registerXmlElementDataStoreValue("dseCustomStateTrig", m_dseCustomStateTrig);
    registerXmlElementDataStoreValue("dseStopTrig", m_dseStopTrig);
    
    return dwRes;
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::registerSubscriber()
{
    m_dseEStopTrig.subscribe();
    m_dseEStopClear.subscribe();
    m_dseDisableTrig.subscribe();
    m_dseClearFaultTrig.subscribe();
    m_dseClearQuickStopTrig.subscribe();
    m_dseEnableTrig.subscribe();
    m_dseReadyTrig.subscribe();
    m_dseCustomStateTrig.subscribe();
    m_dseStopTrig.subscribe();
    
    return 0;
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingFaultNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
{
    m_dseEStopTrig.get(m_eStopTrig);
    m_dseEStopClear.get(m_eStopClear);
    m_dseDisableTrig.get(m_disableTrig);
    m_dseClearFaultTrig.get(m_clearFaultTrig);
    m_dseClearQuickStopTrig.get(m_clearQuickStopTrig);
    m_dseEnableTrig.get(m_enableTrig);
    m_dseReadyTrig.get(m_readyTrig);
    m_dseCustomStateTrig.get(m_customStateTrig);
    m_dseStopTrig.get(m_stopTrig);
    
    if
    (
       m_eStopTrig
    )
    {
        std::cout << "          Trigger: " << "e_stop" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::E_STOP;
    }
    else if
    (
        /*
        not op
        fault cleared
        */
       !p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()
    )
    {
        std::cout << "          Trigger: " << "fault cleared" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::STANDBY;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::CLEARING_FAULT;
    }

    return CallbackStatus::SUCCESS;
}
