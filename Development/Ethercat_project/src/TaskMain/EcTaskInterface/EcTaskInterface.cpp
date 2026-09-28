#include "EcTaskInterface.h"

EcTaskInterface::EcTaskInterface()
{
}

EcTaskInterface::~EcTaskInterface()
{
}

uint32_t EcTaskInterface::addAll(std::vector<Shm_store> &shmStoreVector, std::vector<std::string> &shmLayoutXpathVector)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecTaskInterfaceBaseVector.resize(shmStoreVector.size());

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        m_ecTaskInterfaceBaseVector[i] = new EcInterfaceShm(i, "shmInterfaceName_" + std::to_string(i), "shmInterfacePath_" + std::to_string(i));
        m_ecTaskInterfaceBaseVector[i]->setShmStore(&shmStoreVector[i]);
        m_ecTaskInterfaceBaseVector[i]->setShmLayoutXpath(shmLayoutXpathVector[i]);
    }

    return dwRes;
}

uint32_t EcTaskInterface::cleanup()
{
    std::cout << "Cleanup user task from memory start\n";
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        std::cout << "Cleanup user task " << i + 1 << " from memory\n";
        delete m_ecTaskInterfaceBaseVector[i];
    }

    std::cout << "Cleanup user task from memory complete\n";

    return dwRes;
}

uint32_t EcTaskInterface::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerXml();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerPublisher();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerSubscriber();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->publishData();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->subscribeData();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->mainProcess();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::registerAllShm()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerAllShm();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::registerPublisherShm()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerPublisherShm();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::registerSubscriberShm()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->registerSubscriberShm();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::publishDataShm()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->publishDataShm();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskInterface::subscribeDataShm()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for (size_t i = 0; i < m_ecTaskInterfaceBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskInterfaceBaseVector[i]->subscribeDataShm();
        if (dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

EcTaskInterfaceBase *EcTaskInterface::getElement(uint32_t index)
{
    if (index < m_ecTaskInterfaceBaseVector.size())
    {
        return m_ecTaskInterfaceBaseVector[index];
    }
    return nullptr;
}
