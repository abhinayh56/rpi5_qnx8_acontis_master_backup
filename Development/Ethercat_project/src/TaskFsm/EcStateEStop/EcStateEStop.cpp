#include "EcStateEStop.h"

EcStateEStop::EcStateEStop(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateEStop::~EcStateEStop()
{
}

uint32_t EcStateEStop::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateEStopNs::OnEntrySubroutine(0, "EcStateEStopNs::OnEntrySubroutine"));
    addOnExit(new EcStateEStopNs::OnExitSubroutine(0, "EcStateEStopNs::OnExitSubroutine"));
    addCallback(new EcStateEStopNs::CallbackSubroutine(0, "EcStateEStopNs::CallbackSubroutine"));
    addTransition(new EcStateEStopNs::TransitionSubroutine(0, "EcStateEStopNs::TransitionSubroutine"));

    return dwRes;
}

EcStateEStopNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEStopNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateEStopNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEStopNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEStopNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEStopNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateEStopNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateEStopNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEStopNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateEStopNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEStopNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEStopNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEStopNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateEStopNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateEStopNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEStopNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateEStopNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEStopNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEStopNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEStopNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateEStopNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateEStopNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateEStopNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateEStopNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateEStopNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEStopNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateEStopNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateEStopNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
    
    if(
        /*
        user trigger
        */
       m_eStopClear
    )
    {
        std::cout << "          Trigger: " << "user pressed clear e_stop" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::CLEARING_E_STOP;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::E_STOP;
    }

    return CallbackStatus::SUCCESS;
}
