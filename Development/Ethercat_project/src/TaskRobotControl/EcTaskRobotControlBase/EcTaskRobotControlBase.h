#ifndef EC_TASK_ROBOT_CONTROL_BASE_H
#define EC_TASK_ROBOT_CONTROL_BASE_H

#include "EcTaskBase.h"

class EcTaskRobotControlBase : public EcTaskBase
{
public:
    EcTaskRobotControlBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

    virtual ~EcTaskRobotControlBase();
};

#endif // EC_TASK_ROBOT_CONTROL_BASE_H
