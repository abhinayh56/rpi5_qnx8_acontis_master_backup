#include "EcStateClearingEStop.h"

EcStateClearingEStop::EcStateClearingEStop(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateClearingEStop::~EcStateClearingEStop()
{
}

uint32_t EcStateClearingEStop::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateClearingEStopNs::OnEntrySubroutine(0, "EcStateClearingEStopNs::OnEntrySubroutine"));
    addOnExit(new EcStateClearingEStopNs::OnExitSubroutine(0, "EcStateClearingEStopNs::OnExitSubroutine"));
    addCallback(new EcStateClearingEStopNs::CallbackSubroutine(0, "EcStateClearingEStopNs::CallbackSubroutine"));
    addTransition(new EcStateClearingEStopNs::TransitionSubroutine(0, "EcStateClearingEStopNs::TransitionSubroutine"));

    return dwRes;
}

EcStateClearingEStopNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingEStopNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateClearingEStopNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingEStopNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingEStopNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingEStopNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateClearingEStopNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingEStopNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingEStopNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateClearingEStopNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateClearingEStopNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateClearingEStopNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateClearingEStopNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateClearingEStopNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateClearingEStopNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateClearingEStopNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateClearingEStopNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateClearingEStopNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        e_stop cleared
        */
       m_eStopClear
    )
    {
        std::cout << "          Trigger: " << "e_stop cleared" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::STANDBY;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::CLEARING_E_STOP;
    }
    
    return CallbackStatus::SUCCESS;
}
