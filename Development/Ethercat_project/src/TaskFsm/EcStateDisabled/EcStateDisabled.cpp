#include "EcStateDisabled.h"

EcStateDisabled::EcStateDisabled(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateDisabled::~EcStateDisabled()
{
}

uint32_t EcStateDisabled::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateDisabledNs::OnEntrySubroutine(0, "EcStateDisabledNs::OnEntrySubroutine"));
    addOnExit(new EcStateDisabledNs::OnExitSubroutine(0, "EcStateDisabledNs::OnExitSubroutine"));
    addCallback(new EcStateDisabledNs::CallbackSubroutine(0, "EcStateDisabledNs::CallbackSubroutine"));
    addTransition(new EcStateDisabledNs::TransitionSubroutine(0, "EcStateDisabledNs::TransitionSubroutine"));

    return dwRes;
}

EcStateDisabledNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisabledNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateDisabledNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisabledNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateDisabledNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisabledNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateDisabledNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisabledNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateDisabledNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisabledNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisabledNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisabledNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateDisabledNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateDisabledNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateDisabledNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateDisabledNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisabledNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateDisabledNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisabledNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        /*
        (not op) ||
        (
            ((op) && (fault)) ||
            (op) && (e_stop) ||
            (op) && (quick_stop)
        )
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault() ||
        m_eStopTrig ||
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()
    )
    {
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()) {std::cout << "          Trigger: " << "fault" << std::endl;}
        if(m_eStopTrig) {std::cout << "          Trigger: " << "e_stop" << std::endl;}
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()) {std::cout << "          Trigger: " << "quick_stop" << std::endl;}
    	
        ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else if
    (
        /*
        user trigger
        */
       m_enableTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed enabled" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::ENABLING;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::DISABLED;
    }

    return CallbackStatus::SUCCESS;
}
