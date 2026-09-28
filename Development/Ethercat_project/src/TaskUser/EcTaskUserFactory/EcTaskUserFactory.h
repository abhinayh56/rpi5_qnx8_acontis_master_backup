#ifndef EC_TASK_USER_FACTORY_H
#define EC_TASK_USER_FACTORY_H

#include "EcUser_1.h"
#include "EcUser_2.h"
#include "EcUser_3.h"
#include "EcUser_4.h"
#include "EcUser_5.h"

namespace EcTaskUserFactoryNs
{
    struct TaskData
    {
        std::string taskType;
        uint16_t    taskId;
        std::string taskName;
        std::string taskPath;
        std::string xPath;
    };
};

class EcTaskUserFactory
{
public:
    EcTaskUserFactory();

    ~EcTaskUserFactory();

    static EcTaskUserBase *create(EcTaskUserFactoryNs::TaskData &slaveData);
};

#endif // EC_TASK_USER_FACTORY_H
