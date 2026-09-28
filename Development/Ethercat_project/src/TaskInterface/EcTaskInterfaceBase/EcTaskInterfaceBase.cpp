#include "EcTaskInterfaceBase.h"

EcTaskInterfaceBase::EcTaskInterfaceBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath) : EcTaskBase(taskId, taskName, taskPath)
{
}

EcTaskInterfaceBase::~EcTaskInterfaceBase()
{
}

void EcTaskInterfaceBase::setShmStore(Shm_store *shm_store)
{
    m_shm_store = shm_store;
}

Shm_store *EcTaskInterfaceBase::getShmStore()
{
    return m_shm_store;
}

void EcTaskInterfaceBase::setShmLayoutXpath(const std::string &shmLayoutXpath)
{
    m_shmLayoutXpath = shmLayoutXpath;
}

std::string EcTaskInterfaceBase::getShmLayoutXpath()
{
    return m_shmLayoutXpath;
}

uint32_t EcTaskInterfaceBase::registerAllShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcTaskInterfaceBase::registerPublisherShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcTaskInterfaceBase::registerSubscriberShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcTaskInterfaceBase::publishDataShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcTaskInterfaceBase::subscribeDataShm()
{
    return CallbackStatus::SUCCESS;
}
