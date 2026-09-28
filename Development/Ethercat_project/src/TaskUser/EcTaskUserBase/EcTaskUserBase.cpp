#include "EcTaskUserBase.h"

EcTaskUserBase::EcTaskUserBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskBase(taskId, taskName, taskPath)
{
}

EcTaskUserBase::~EcTaskUserBase()
{
}
