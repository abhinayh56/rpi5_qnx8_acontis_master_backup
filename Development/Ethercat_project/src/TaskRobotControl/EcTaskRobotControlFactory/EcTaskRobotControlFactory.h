#ifndef EC_TASK_ROBOT_CONTROL_FACTORY_H
#define EC_TASK_ROBOT_CONTROL_FACTORY_H

#include "EcTaskRobotControlBase.h"

namespace EcTaskRobotControlFactoryNs
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

class EcTaskRobotControlFactory
{
public:
    EcTaskRobotControlFactory();

    ~EcTaskRobotControlFactory();

    static EcTaskRobotControlBase *create(EcTaskRobotControlFactoryNs::TaskData &taskData);
};

#endif // EC_TASK_ROBOT_CONTROL_FACTORY_H
