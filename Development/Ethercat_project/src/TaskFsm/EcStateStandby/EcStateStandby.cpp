#include "EcStateStandby.h"

EcStateStandby::EcStateStandby(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateStandby::~EcStateStandby()
{
}

uint32_t EcStateStandby::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateStandbyNs::OnEntrySubroutine(0, "EcStateStandbyNs::OnEntrySubroutine"));
    addOnExit(new EcStateStandbyNs::OnExitSubroutine(0, "EcStateStandbyNs::OnExitSubroutine"));
    addCallback(new EcStateStandbyNs::CallbackSubroutine(0, "EcStateStandbyNs::CallbackSubroutine"));
    addTransition(new EcStateStandbyNs::TransitionSubroutine(0, "EcStateStandbyNs::TransitionSubroutine"));

    return dwRes;
}

EcStateStandbyNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateStandbyNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateStandbyNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateStandbyNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateStandbyNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    return 0;
}

EcStateStandbyNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : 
    SubroutineBase(id, name)
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

EcStateStandbyNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateStandbyNs::CallbackSubroutine::registerXml()
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

uint32_t EcStateStandbyNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStandbyNs::CallbackSubroutine::registerSubscriber()
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

uint32_t EcStateStandbyNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
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

    // if (m_eStopTrig == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->emergencyStop();
    // }
    // if (m_eStopClear == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->faultClear();
    // }
    // if (m_disableTrig == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    // }
    // if (m_data_4 == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    // }
    // if (m_enableTrig == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModeVelocity();
    // }
    // if (m_readyTrig == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModeTorque();
    // }
    // if(m_data_7 == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->enable();
    // }
    // if (m_data_8 == true)
    // {
    //     p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->quickStop();
    // }

    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();

    return 0;
}

EcStateStandbyNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateStandbyNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateStandbyNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateStandbyNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStandbyNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateStandbyNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateStandbyNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        (fault)      ||
        (e_stop)     ||
        (quick_stop)
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
        (op) && (user trigger)
        */
       m_enableTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed enable" << std::endl;
        ctxtCallback.idNextState = EcStateData::StateId::ENABLING;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::STANDBY;
    }

    return CallbackStatus::SUCCESS;
}
