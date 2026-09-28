#include "EcStateEnabled.h"

EcStateEnabled::EcStateEnabled(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateEnabled::~EcStateEnabled()
{
}

uint32_t EcStateEnabled::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateEnabledNs::OnEntrySubroutine(0, "EcStateEnabledNs::OnEntrySubroutine"));
    addOnExit(new EcStateEnabledNs::OnExitSubroutine(0, "EcStateEnabledNs::OnExitSubroutine"));
    addCallback(new EcStateEnabledNs::CallbackSubroutine(0, "EcStateEnabledNs::CallbackSubroutine"));
    addTransition(new EcStateEnabledNs::TransitionSubroutine(0, "EcStateEnabledNs::TransitionSubroutine"));

    return dwRes;
}

EcStateEnabledNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnabledNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateEnabledNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnabledNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateEnabledNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnabledNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateEnabledNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnabledNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateEnabledNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnabledNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnabledNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnabledNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{    
    return 0;
}

EcStateEnabledNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateEnabledNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateEnabledNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateEnabledNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnabledNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateEnabledNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnabledNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        not op
        fault
        e_stop
        quick_stop
        disable command by user
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault() ||
        m_eStopTrig ||
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop() ||
        m_disableTrig
    )
    {
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()){std::cout << "          Trigger: " << "fault" << std::endl;}
        if(m_eStopTrig){std::cout << "          Trigger: " << "e_stop" << std::endl;}
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()){std::cout << "          Trigger: " << "quick_stop" << std::endl;}
        if(m_disableTrig){std::cout << "          Trigger: " << "user pressed disabled" << std::endl;}
    	
        ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else if(
        /*
        user trigger
        */
        m_readyTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed ready" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::READY;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::ENABLED;
    }
    
    return CallbackStatus::SUCCESS;
}
