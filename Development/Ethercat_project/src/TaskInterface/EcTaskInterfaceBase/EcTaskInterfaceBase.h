#ifndef EC_TASK_INTERFACE_BASE_H
#define EC_TASK_INTERFACE_BASE_H

#include "EcTaskBase.h"
#include "IPC.h"

class EcTaskInterfaceBase : public EcTaskBase
{
public:
    EcTaskInterfaceBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

    virtual ~EcTaskInterfaceBase();

    virtual void setShmStore(Shm_store *shm_store);

    virtual Shm_store* getShmStore();

    virtual void setShmLayoutXpath(const std::string &shmLayoutXpath);

    virtual std::string getShmLayoutXpath();

    virtual uint32_t registerAllShm();

    virtual uint32_t registerPublisherShm();

    virtual uint32_t registerSubscriberShm();

    virtual uint32_t publishDataShm();

    virtual uint32_t subscribeDataShm();

protected:
    Shm_store *m_shm_store;
    std::string m_shmLayoutXpath;
};

#endif // EC_TASK_INTERFACE_BASE_H
