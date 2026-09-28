#ifndef EC_TASK_USER_BASE_H
#define EC_TASK_USER_BASE_H

#include "EcTaskBase.h"

class EcTaskUserBase : public EcTaskBase
{
public:
    EcTaskUserBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

    virtual ~EcTaskUserBase();
};

#endif // EC_TASK_USER_BASE_H
