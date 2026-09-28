#ifndef EC_TASK_ETHERCAT_SLAVE__FACTORY_H
#define EC_TASK_ETHERCAT_SLAVE__FACTORY_H

#include "EcSlaveEl1008.h"
#include "EcSlaveEl2008.h"
#include "EcSlaveEl3024.h"
#include "EcSlaveEl6002.h"
#include "EcSlaveSscIoModule.h"
#include "EcSlaveEncbrkctrl.h"
#include "EcSlaveRfidslave.h"

namespace EcTaskEthercatSlaveFactoryNs
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
    };
};

class EcTaskEthercatSlaveFactory
{
public:
    EcTaskEthercatSlaveFactory();

    ~EcTaskEthercatSlaveFactory();

    static EcTaskEthercatSlaveBase *create(EcTaskEthercatSlaveFactoryNs::TaskData &taskData);
};

#endif // EC_TASK_ETHERCAT_SLAVE_FACTORY_H
