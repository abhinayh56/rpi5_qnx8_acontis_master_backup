#include "EcTaskEthercatSlaveFactory.h"

EcTaskEthercatSlaveFactory::EcTaskEthercatSlaveFactory()
{
}

EcTaskEthercatSlaveFactory::~EcTaskEthercatSlaveFactory()
{
}

EcTaskEthercatSlaveBase* EcTaskEthercatSlaveFactory::create(EcTaskEthercatSlaveFactoryNs::TaskData &taskData)
{
    EcTaskEthercatSlaveBase *newTask = nullptr;

    if (taskData.taskType == "El1008")
    {
        newTask = new EcSlaveEl1008(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "El2008")
    {
        newTask = new EcSlaveEl2008(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "SscIoModule")
    {
        newTask = new EcSlaveSscIoModule(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "Rfidslave")
    {
        newTask = new EcSlaveRfidslave(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "El6002")
    {
        newTask = new EcSlaveEl6002(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
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
