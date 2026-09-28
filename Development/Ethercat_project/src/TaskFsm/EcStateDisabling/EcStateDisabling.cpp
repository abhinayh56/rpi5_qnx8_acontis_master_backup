#include "EcStateDisabling.h"

EcStateDisabling::EcStateDisabling(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateDisabling::~EcStateDisabling()
{
}

uint32_t EcStateDisabling::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateDisablingNs::OnEntrySubroutine(0, "EcStateDisablingNs::OnEntrySubroutine"));
    addOnExit(new EcStateDisablingNs::OnExitSubroutine(0, "EcStateDisablingNs::OnExitSubroutine"));
    addCallback(new EcStateDisablingNs::CallbackSubroutine(0, "EcStateDisablingNs::CallbackSubroutine"));
    addTransition(new EcStateDisablingNs::TransitionSubroutine(0, "EcStateDisablingNs::TransitionSubroutine"));

    return dwRes;
}

EcStateDisablingNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisablingNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateDisablingNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisablingNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateDisablingNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisablingNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateDisablingNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateDisablingNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateDisablingNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateDisablingNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisablingNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateDisablingNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    
    return 0;
}

EcStateDisablingNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateDisablingNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateDisablingNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateDisablingNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateDisablingNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateDisablingNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateDisablingNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault() ||
        m_eStopTrig ||
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()
    )
    {
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()){std::cout << "          Trigger: " << "fault" << std::endl;}
        if(m_eStopTrig){std::cout << "          Trigger: " << "e_stop" << std::endl;}
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()){std::cout << "          Trigger: " << "quick_stop" << std::endl;}
    	ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else if(
        /*
        servo disabled
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isDisable()
    )
    {
        std::cout << "          Trigger: " << "servo disabled" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::DISABLED;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::DISABLING;
    }

    return CallbackStatus::SUCCESS;
}
