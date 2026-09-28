#include "EcStateX.h"

EcStateX::EcStateX(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateX::~EcStateX()
{
}

uint32_t EcStateX::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateXNs::OnEntrySubroutine(0, "EcStateXNs::OnEntrySubroutine"));
    addOnExit(new EcStateXNs::OnExitSubroutine(0, "EcStateXNs::OnExitSubroutine"));
    addCallback(new EcStateXNs::CallbackSubroutine(0, "EcStateXNs::CallbackSubroutine"));
    addTransition(new EcStateXNs::TransitionSubroutine(0, "EcStateXNs::TransitionSubroutine"));

    return dwRes;
}

EcStateXNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateXNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateXNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateXNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateXNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateXNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateXNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateXNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateXNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateXNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateXNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateXNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateXNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateXNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateXNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
    // m_dseActualPosition("ACTUAL_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0 , false),
    // m_dseCommandPosition("COMMAND_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0 , false),
    // m_dseTiLimitSwitchPressed("TI_LIMIT_SWITCH_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", false , false),
    // m_dseTiClutchButtonPressed("TI_CLUTCH_BUTTON_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", false , false),
    // m_actualPosition(0),
    // m_commandPosition(0),
    // m_tiLimitSwitchPressed(false),
    // m_tiClutchButtonPressed(false)
{
}

EcStateXNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateXNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseActualPosition", m_dseActualPosition);
    registerXmlElementDataStoreValue("dseCommandPosition", m_dseCommandPosition);
    registerXmlElementDataStoreValue("dseTiLimitSwitchPressed", m_dseTiLimitSwitchPressed);
    registerXmlElementDataStoreValue("dseTiClutchButtonPressed", m_dseTiClutchButtonPressed);
    registerXmlElementValue("actualPosition", m_actualPosition);
    registerXmlElementValue("commandPosition", m_commandPosition);
    registerXmlElementValue("tiLimitSwitchPressed", m_tiLimitSwitchPressed);
    registerXmlElementValue("tiClutchButtonPressed", m_tiClutchButtonPressed);
    
    return dwRes;
}

uint32_t EcStateXNs::CallbackSubroutine::registerPublisher()
{
    m_dseCommandPosition.publish();

    return 0;
}

uint32_t EcStateXNs::CallbackSubroutine::registerSubscriber()
{
    m_dseActualPosition.subscribe();
    m_dseTiLimitSwitchPressed.subscribe();
    m_dseTiClutchButtonPressed.subscribe();

    return 0;
}

uint32_t EcStateXNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateXNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModeTorque();

    // TI calibration logic
    m_dseActualPosition.get(m_actualPosition);
    m_dseTiLimitSwitchPressed.get(m_tiLimitSwitchPressed);
    m_dseTiClutchButtonPressed.get(m_tiClutchButtonPressed);

    // std::cout << "(FSM) Ti clutch, limitSwitch, ACTUAL_POSITION: " << m_tiClutchButtonPressed << ", " << m_tiLimitSwitchPressed << ", " << m_actualPosition << std::endl;

    if(m_tiLimitSwitchPressed == true)
    {
        int32_t actualPositionIU = p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->getElement(4)->getActualPositionIU();
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->getElement(4)->setOffsetPosition(actualPositionIU);
    }

    // m_dseCommandPosition.set(m_commandPosition);
    
    return 0;
}

EcStateXNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateXNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateXNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateXNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateXNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateXNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateXNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        (op) && (fault)
        (op) && (e_stop)
        (op) && (quick_stop)
        (op) && (disable command by user)
        (op) && (stop command by user)
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
        if(m_disableTrig){std::cout << "          Trigger: " << "user pressed disable" << std::endl;}
        if(m_stopTrig){std::cout << "          Trigger: " << "user pressed stop" << std::endl;}
        
        ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::STATE_X;
    }

    return CallbackStatus::SUCCESS;
}
