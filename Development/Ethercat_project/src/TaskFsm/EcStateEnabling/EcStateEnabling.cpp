#include "EcStateEnabling.h"

EcStateEnabling::EcStateEnabling(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateEnabling::~EcStateEnabling()
{
}

uint32_t EcStateEnabling::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateEnablingNs::OnEntrySubroutine(0, "EcStateEnablingNs::OnEntrySubroutine"));
    addOnExit(new EcStateEnablingNs::OnExitSubroutine(0, "EcStateEnablingNs::OnExitSubroutine"));
    addCallback(new EcStateEnablingNs::CallbackSubroutine(0, "EcStateEnablingNs::CallbackSubroutine"));
    addTransition(new EcStateEnablingNs::TransitionSubroutine(0, "EcStateEnablingNs::TransitionSubroutine"));

    return dwRes;
}

EcStateEnablingNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnablingNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateEnablingNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnablingNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateEnablingNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnablingNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateEnablingNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateEnablingNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateEnablingNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateEnablingNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnablingNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateEnablingNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->enable();

    return 0;
}

EcStateEnablingNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateEnablingNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateEnablingNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateEnablingNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateEnablingNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateEnablingNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateEnablingNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        all servo enabled
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isEnable()
    )
    {
        std::cout << "          Trigger: " << "servo enabled" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::ENABLED;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::ENABLING;
    }

    return CallbackStatus::SUCCESS;
}
