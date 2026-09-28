#include "EcTaskFsm.h"

EcTaskFsm::EcTaskFsm() : m_fsm(EcStateData::StateId::STANDBY)
{
    m_stateVector.reserve(25);
}

EcTaskFsm::~EcTaskFsm()
{
}

uint32_t EcTaskFsm::addAll(std::vector<EcTaskStateFactoryNs::TaskData>& taskData)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "adding states start" << std::endl;

    for (uint32_t i = 0; i < taskData.size(); i++)
    {
        dwRes |= addTaskState(EcTaskStateFactory::create(taskData[i]));
    }

    std::cout << "adding states stop" << std::endl;

    for (uint32_t i = 0; i < m_stateVector.size(); i++)
    {
        m_fsm.addState(m_stateVector[i]);
    }

    return dwRes;
}

uint32_t EcTaskFsm::cleanup()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskFsm::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.registerXml();

    return dwRes;
}

uint32_t EcTaskFsm::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.registerPublisher();

    return dwRes;
}

uint32_t EcTaskFsm::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.registerSubscriber();

    return dwRes;
}

uint32_t EcTaskFsm::setContext(CtxtSubroutine *ctxt)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.setContext(ctxt);

    return dwRes;
}

uint32_t EcTaskFsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.config();

    return dwRes;
}

uint32_t EcTaskFsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_fsm.update();

    return dwRes;
}

uint32_t EcTaskFsm::addTaskState(EcTaskStateBase *pState)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_stateVector.push_back(pState);
    std::cout << "m_stateVector.size(): " << m_stateVector.size() << std::endl;

    return dwRes;
}