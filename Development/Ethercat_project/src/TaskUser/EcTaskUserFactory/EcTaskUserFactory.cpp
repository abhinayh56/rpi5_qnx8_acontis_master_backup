#include "EcTaskUserFactory.h"

EcTaskUserFactory::EcTaskUserFactory()
{
}

EcTaskUserFactory::~EcTaskUserFactory()
{
}

EcTaskUserBase* EcTaskUserFactory::create(EcTaskUserFactoryNs::TaskData &taskData)
{
    EcTaskUserBase *newTask = nullptr;

    if (taskData.taskType == "User_1")
    {
        newTask = new EcUser_1(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "User_2")
    {
        newTask = new EcUser_2(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if(taskData.taskType == "User_3")
    {
        newTask = new EcUser_3(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if(taskData.taskType == "User_4")
    {
        newTask = new EcUser_4(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if(taskData.taskType == "User_5")
    {
        newTask = new EcUser_5(taskData.taskId, taskData.taskName, taskData.taskPath);
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
