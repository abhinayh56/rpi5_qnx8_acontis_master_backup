#ifndef EC_TASK_STATE_BASE_H
#define EC_TASK_STATE_BASE_H

#include "StateBase.h"
#include "EcState.h"
#include "EcTaskBase.h"
#include <string>

class EcTaskStateBase : public StateBase
{
public:
    EcTaskStateBase(uint32_t taskId ,const std::string& taskName, const std::string &taskPath);
    
    virtual ~EcTaskStateBase();
    
    virtual uint32_t addSubroutine();

    uint16_t getTaskId();

    std::string &getTaskName();

    std::string &getTaskPath();

protected:
    std::string m_taskPath;
};

#endif // EC_TASK_STATE_BASE_H
