#include "EcStateReady.h"

EcStateReady::EcStateReady(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateReady::~EcStateReady()
{
}

uint32_t EcStateReady::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateReadyNs::OnEntrySubroutine(0, "EcStateReadyNs::OnEntrySubroutine"));
    addOnExit(new EcStateReadyNs::OnExitSubroutine(0, "EcStateReadyNs::OnExitSubroutine"));
    addCallback(new EcStateReadyNs::CallbackSubroutine(0, "EcStateReadyNs::CallbackSubroutine"));
    addTransition(new EcStateReadyNs::TransitionSubroutine(0, "EcStateReadyNs::TransitionSubroutine"));

    return dwRes;
}

EcStateReadyNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateReadyNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateReadyNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateReadyNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateReadyNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateReadyNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->setModePosition();
    std::cout << "m_Xpath : " << getXPath() << std::endl;
    return 0;
}

EcStateReadyNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateReadyNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateReadyNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateReadyNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateReadyNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateReadyNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateReadyNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateReadyNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateReadyNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateReadyNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateReadyNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateReadyNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{    
    return 0;
}

EcStateReadyNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateReadyNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateReadyNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateReadyNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateReadyNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateReadyNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateReadyNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        if(m_disableTrig){std::cout << "          Trigger: " << "user pressed disable" << std::endl;}
        
    	ctxtCallback.idNextState = EcStateData::StateId::STOP;
    }
    else if(
        /*
        user trigger
        */
        m_customStateTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed to joystick control" << std::endl;
        // ctxtCallback.idNextState = EcStateData::StateId::JOYSTICK_CONTROL;
        ctxtCallback.idNextState = EcStateData::StateId::STATE_X;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::READY;
    }

    return CallbackStatus::SUCCESS;
}
