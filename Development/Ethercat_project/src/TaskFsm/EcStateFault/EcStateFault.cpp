#include "EcStateFault.h"

EcStateFault::EcStateFault(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateFault::~EcStateFault()
{
}

uint32_t EcStateFault::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateFaultNs::OnEntrySubroutine(0, "EcStateFaultNs::OnEntrySubroutine"));
    addOnExit(new EcStateFaultNs::OnExitSubroutine(0, "EcStateFaultNs::OnExitSubroutine"));
    addCallback(new EcStateFaultNs::CallbackSubroutine(0, "EcStateFaultNs::CallbackSubroutine"));
    addTransition(new EcStateFaultNs::TransitionSubroutine(0, "EcStateFaultNs::TransitionSubroutine"));

    return dwRes;
}

EcStateFaultNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateFaultNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateFaultNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateFaultNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateFaultNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateFaultNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateFaultNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateFaultNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateFaultNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateFaultNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateFaultNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateFaultNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateFaultNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateFaultNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateFaultNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateFaultNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateFaultNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateFaultNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateFaultNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateFaultNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateFaultNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateFaultNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateFaultNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateFaultNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateFaultNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
       m_clearFaultTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed clear fault" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::CLEARING_FAULT;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::FAULT;
    }
    
    return CallbackStatus::SUCCESS;
}
