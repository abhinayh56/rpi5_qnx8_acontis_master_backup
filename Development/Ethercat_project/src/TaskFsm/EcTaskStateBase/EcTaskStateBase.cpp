#include "EcTaskStateBase.h"

EcTaskStateBase::EcTaskStateBase(uint32_t taskId, const std::string &taskName, const std::string &taskPath) : 
    StateBase(taskId, taskName),
    m_taskPath(taskPath)
{
}

EcTaskStateBase::~EcTaskStateBase()
{
}

uint32_t EcTaskStateBase::addSubroutine()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint16_t EcTaskStateBase::getTaskId()
{
    return m_id;
}

std::string &EcTaskStateBase::getTaskName()
{
    return m_name;
}

std::string &EcTaskStateBase::getTaskPath()
{
    return m_taskPath;
}
