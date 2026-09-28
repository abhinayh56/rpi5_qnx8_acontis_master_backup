#include "EcStateQuickStop.h"

EcStateQuickStop::EcStateQuickStop(uint32_t taskId ,const std::string& taskName, const std::string &taskPath) : EcTaskStateBase(taskId, taskName, taskPath)
{
}

EcStateQuickStop::~EcStateQuickStop()
{
}

uint32_t EcStateQuickStop::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    addOnEntry(new EcStateQuickStopNs::OnEntrySubroutine(0, "EcStateQuickStopNs::OnEntrySubroutine"));
    addOnExit(new EcStateQuickStopNs::OnExitSubroutine(0, "EcStateQuickStopNs::OnExitSubroutine"));
    addCallback(new EcStateQuickStopNs::CallbackSubroutine(0, "EcStateQuickStopNs::CallbackSubroutine"));
    addTransition(new EcStateQuickStopNs::TransitionSubroutine(0, "EcStateQuickStopNs::TransitionSubroutine"));

    return dwRes;
}

EcStateQuickStopNs::OnEntrySubroutine::OnEntrySubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateQuickStopNs::OnEntrySubroutine::~OnEntrySubroutine()
{
}

uint32_t EcStateQuickStopNs::OnEntrySubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateQuickStopNs::OnEntrySubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnEntrySubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnEntrySubroutine::config()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnEntrySubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->disable();
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateQuickStopNs::OnExitSubroutine::OnExitSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateQuickStopNs::OnExitSubroutine::~OnExitSubroutine()
{
}

uint32_t EcStateQuickStopNs::OnExitSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateQuickStopNs::OnExitSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnExitSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnExitSubroutine::config()
{
    return 0;
}

uint32_t EcStateQuickStopNs::OnExitSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();

    return 0;
}

EcStateQuickStopNs::CallbackSubroutine::CallbackSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
{
}

EcStateQuickStopNs::CallbackSubroutine::~CallbackSubroutine()
{
}

uint32_t EcStateQuickStopNs::CallbackSubroutine::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    return dwRes;
}

uint32_t EcStateQuickStopNs::CallbackSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateQuickStopNs::CallbackSubroutine::registerSubscriber()
{
    return 0;
}

uint32_t EcStateQuickStopNs::CallbackSubroutine::config()
{
    return 0;
}

uint32_t EcStateQuickStopNs::CallbackSubroutine::callback(CtxtCallback &ctxtCallback)
{
    p_ctxtSubroutine->p_ecTaskEthercatSlaveServo->initPosition();
    
    return 0;
}

EcStateQuickStopNs::TransitionSubroutine::TransitionSubroutine(uint32_t id, const std::string &name) : SubroutineBase(id, name)
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

EcStateQuickStopNs::TransitionSubroutine::~TransitionSubroutine()
{
}

uint32_t EcStateQuickStopNs::TransitionSubroutine::registerXml()
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

uint32_t EcStateQuickStopNs::TransitionSubroutine::registerPublisher()
{
    return 0;
}

uint32_t EcStateQuickStopNs::TransitionSubroutine::registerSubscriber()
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

uint32_t EcStateQuickStopNs::TransitionSubroutine::config()
{
    return 0;
}

uint32_t EcStateQuickStopNs::TransitionSubroutine::callback(CtxtCallback &ctxtCallback)
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
        m_clearQuickStopTrig
    )
    {
        std::cout << "          Trigger: " << "user pressed clear quick_stop" << std::endl;
    	ctxtCallback.idNextState = EcStateData::StateId::CLEARING_QUICK_STOP;
    }
    else
    {
        ctxtCallback.idNextState = EcStateData::StateId::QUICK_STOP;
    }
    
    return CallbackStatus::SUCCESS;
}
