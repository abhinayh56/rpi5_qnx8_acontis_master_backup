#include "EcStateJoystickControl.h"

EcStateJoystickControl::EcStateJoystickControl(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateJoystickControl::~EcStateJoystickControl()
{
}

uint32_t EcStateJoystickControl::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

   addOnEntry(new EcStateJoystickControlNs::OnEntrySubroutine(0, "EcStateJoystickControlNs::OnEntrySubroutine"));
   addOnExit(new EcStateJoystickControlNs::OnExitSubroutine(0, "EcStateJoystickControlNs::OnExitSubroutine"));
   addCallback(new EcStateJoystickControlNs::CallbackSubroutine(0, "EcStateJoystickControlNs::CallbackSubroutine"));
   addTransition(new EcStateJoystickControlNs::TransitionSubroutine(0, "EcStateJoystickControlNs::TransitionSubroutine"));

    return dwRes;
}

EcStateJoystickControlNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateJoystickControlNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateJoystickControlNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateJoystickControlNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::config()
{
   return 0;
}

uint32_t EcStateJoystickControlNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
   return 0;
}

EcStateJoystickControlNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateJoystickControlNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::config()
{
   return 0;
}

uint32_t EcStateJoystickControlNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModeTorque();
   return 0;
}

EcStateJoystickControlNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateJoystickControlNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateJoystickControlNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateJoystickControlNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateJoystickControlNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateJoystickControlNs::TransitionSubroutine::config()
{
   return 0;
}

uint32_t EcStateJoystickControlNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        stop command by user
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault() ||
        m_eStopTrig ||
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop() ||
        m_disableTrig ||
        m_stopTrig
    )
    {
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()){std::cout << "          Trigger: " << "fault" << std::endl;}
        if(m_eStopTrig){std::cout << "          Trigger: " << "e_stop" << std::endl;}
        if(p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()){std::cout << "          Trigger: " << "quick_stop" << std::endl;}
        if(m_disableTrig){std::cout << "          Trigger: " << "user pressed stop" << std::endl;}
        if(m_stopTrig){std::cout << "          Trigger: " << "user pressed stop" << std::endl;}
        
        ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::JOYSTICK_CONTROL;
    }
    
   return CallbackStatus::SUCCESS;
}
