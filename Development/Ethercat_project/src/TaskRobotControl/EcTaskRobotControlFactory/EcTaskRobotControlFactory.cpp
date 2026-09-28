#include "EcTaskRobotControlFactory.h"

EcTaskRobotControlFactory::EcTaskRobotControlFactory()
{
}

EcTaskRobotControlFactory::~EcTaskRobotControlFactory()
{
}

EcTaskRobotControlBase* EcTaskRobotControlFactory::create(EcTaskRobotControlFactoryNs::TaskData &taskData)
{
    EcTaskRobotControlBase *newTask = nullptr;

    if (taskData.taskType == "RobotControl_1")
    {
        // newTask = new EcRobotControl_1(taskData.taskId, taskData.taskName);
    }
    else
    {
        newTask = nullptr;
    }

    if (newTask != nullptr)
    {
        newTask->setXPath(taskData.xPath);
    }

    return newTask;
}
