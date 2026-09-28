#include "EcStateStop.h"

EcStateStop::EcStateStop(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateStop::~EcStateStop()
{
}

uint32_t EcStateStop::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateStopNs::OnEntrySubroutine(0, "EcStateStopNs::OnEntrySubroutine"));
    addOnExit(new EcStateStopNs::OnExitSubroutine(0, "EcStateStopNs::OnExitSubroutine"));
    addCallback(new EcStateStopNs::CallbackSubroutine(0, "EcStateStopNs::CallbackSubroutine"));
    addTransition(new EcStateStopNs::TransitionSubroutine(0, "EcStateStopNs::TransitionSubroutine"));

    return dwRes;
}

EcStateStopNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateStopNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateStopNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateStopNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStopNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateStopNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateStopNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateStopNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateStopNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateStopNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateStopNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStopNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateStopNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateStopNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateStopNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateStopNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateStopNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateStopNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStopNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateStopNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateStopNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateStopNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateStopNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateStopNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateStopNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateStopNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateStopNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateStopNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isFault()
    )
    {
        std::cout << "          Trigger: " << "fault" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::FAULT;
    }
    else if(
        /*
        e_stop
        */
       m_eStopTrig
    )
    {
        std::cout << "          Trigger: " << "e_stop" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::E_STOP;
    }
    else if(
        /*
        quick_stop
        */
        p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->isQuickStop()
    )
    {
        std::cout << "          Trigger: " << "quick_stop" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::QUICK_STOP;
    }
    else if(
        /*
        disable command by user
        */
       m_disableTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed disable" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::DISABLING;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::READY;
    }

    return CallbackStatus::SUCCESS;
}
