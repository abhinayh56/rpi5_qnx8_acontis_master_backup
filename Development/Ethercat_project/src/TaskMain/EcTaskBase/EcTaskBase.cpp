#include "EcTaskBase.h"

EcTaskBase::EcTaskBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    XmlDataRegister(),
    m_taskId(taskId),
    m_taskName(taskName),
    m_taskPath(taskPath)
{
}

EcTaskBase::~EcTaskBase()
{
}

uint32_t EcTaskBase::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskBase::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskBase::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskBase::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskBase::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskBase::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint16_t EcTaskBase::getTaskId()
{
    return m_taskId;
}

std::string &EcTaskBase::getTaskName()
{
    return m_taskName;
}

std::string &EcTaskBase::getTaskPath()
{
    return m_taskPath;
}
