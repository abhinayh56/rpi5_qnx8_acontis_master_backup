#include "EcStateClearingQuickStop.h"

EcStateClearingQuickStop::EcStateClearingQuickStop(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateClearingQuickStop::~EcStateClearingQuickStop()
{
}

uint32_t EcStateClearingQuickStop::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateClearingQuickStopNs::OnEntrySubroutine(0, "EcStateClearingQuickStopNs::OnEntrySubroutine"));
    addOnExit(new EcStateClearingQuickStopNs::OnExitSubroutine(0, "EcStateClearingQuickStopNs::OnExitSubroutine"));
    addCallback(new EcStateClearingQuickStopNs::CallbackSubroutine(0, "EcStateClearingQuickStopNs::CallbackSubroutine"));
    addTransition(new EcStateClearingQuickStopNs::TransitionSubroutine(0, "EcStateClearingQuickStopNs::TransitionSubroutine"));

    return dwRes;
}

EcStateClearingQuickStopNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingQuickStopNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateClearingQuickStopNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingQuickStopNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingQuickStopNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingQuickStopNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateClearingQuickStopNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingQuickStopNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingQuickStopNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingQuickStopNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateClearingQuickStopNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingQuickStopNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    return 0;
}

EcStateClearingQuickStopNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateClearingQuickStopNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateClearingQuickStopNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateClearingQuickStopNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateClearingQuickStopNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingQuickStopNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
    else if (
        /*
        quick_stop cleared
        */
       (!p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()) || (p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isDisable())
    )
    {
        std::cout << "          Trigger: " << "quick_stop cleared" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::STANDBY;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::CLEARING_QUICK_STOP;
    }

    return CallbackStatus::SUCCESS;
}
