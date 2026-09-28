#include "EcTaskRobotControlBase.h"

EcTaskRobotControlBase::EcTaskRobotControlBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskBase(taskId, taskName, taskPath)
{
}

EcTaskRobotControlBase::~EcTaskRobotControlBase()
{
}
