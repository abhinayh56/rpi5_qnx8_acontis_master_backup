#ifndef EC_TASK_BASE_H
#define EC_TASK_BASE_H

#include "ITC.h"
#include "CallbackStatus.h"
#include "xmlDataRegister.h"

class EcTaskBase : public XmlDataRegister
{
public:
     EcTaskBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

     virtual ~EcTaskBase();

     virtual uint32_t registerPublisher();

     virtual uint32_t registerSubscriber();

     virtual uint32_t config();

     virtual uint32_t publishData();

     virtual uint32_t subscribeData();

     virtual uint32_t mainProcess();

     virtual uint16_t getTaskId();

     virtual std::string &getTaskName();

     virtual std::string &getTaskPath();
     
protected:
     uint16_t m_taskId;
     std::string m_taskName;
     std::string m_taskPath;
};

#endif // EC_TASK_BASE_H
