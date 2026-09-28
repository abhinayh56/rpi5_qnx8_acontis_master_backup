#include "EcTaskEthercatSlaveServoFactory.h"

EcTaskEthercatSlaveServoFactory::EcTaskEthercatSlaveServoFactory()
{
}

EcTaskEthercatSlaveServoFactory::~EcTaskEthercatSlaveServoFactory()
{
}

EcTaskEthercatSlaveServoBase* EcTaskEthercatSlaveServoFactory::create(EcTaskEthercatSlaveServoFactoryNs::TaskData &taskData)
{
    EcTaskEthercatSlaveServoBase *newTask = nullptr;

    if (taskData.taskType == "PitchDrive")
    {
        newTask = new EcSlavePitchDrive(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "DenNetE")
    {
        newTask = new EcSlaveDenNetE(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "DenXcrE")
    {
        newTask = new EcSlaveDenXcrE(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "Ipos2401MxCat")
    {
        newTask = new EcSlaveIpos2401MxCat(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else if (taskData.taskType == "ElmoDrive")
    {
        newTask = new EcSlaveElmoDrive(taskData.taskId, taskData.taskName, taskData.taskPath, taskData.slaveAddress, taskData.slaveName);
    }
    else
    {
        newTask = nullptr;
    }

    if (newTask != nullptr)
    {
        newTask->setOffsetPosition(taskData.offsetPosition);
        newTask->setOffsetVelocity(taskData.offsetVelocity);
        newTask->setOffsetTorque(taskData.offsetTorque);
        newTask->setFactorPosition(taskData.factorPosition);
        newTask->setFactorVelocity(taskData.factorVelocity);
        newTask->setFactorTorque(taskData.factorTorque);
        newTask->setXPath(taskData.xPath);
    }

    return newTask;
}
