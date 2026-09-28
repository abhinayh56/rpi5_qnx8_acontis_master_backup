#ifndef EC_TASK_ETHERCAT_SLAVE_SERVO_FACTORY_H
#define EC_TASK_ETHERCAT_SLAVE_SERVO_FACTORY_H

#include "EcSlaveDenNetE.h"
#include "EcSlaveDenXcrE.h"
#include "EcSlaveElmoDrive.h"
#include "EcSlaveIpos2401MxCat.h"
#include "EcSlavePitchDrive.h"

namespace EcTaskEthercatSlaveServoFactoryNs
{
    struct TaskData
    {
        std::string taskType;
        uint16_t    taskId;
        std::string taskName;
        std::string taskPath;
        std::string xPath;
        uint16_t    slaveAddress;
        std::string slaveName;
        int32_t     offsetPosition;
        int32_t     offsetVelocity;
        int16_t     offsetTorque;
        double      factorPosition;
        double      factorVelocity;
        double      factorTorque;
    };
};

class EcTaskEthercatSlaveServoFactory
{
public:
    EcTaskEthercatSlaveServoFactory();

    ~EcTaskEthercatSlaveServoFactory();

    static EcTaskEthercatSlaveServoBase *create(EcTaskEthercatSlaveServoFactoryNs::TaskData &taskData);
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_FACTORY_H
