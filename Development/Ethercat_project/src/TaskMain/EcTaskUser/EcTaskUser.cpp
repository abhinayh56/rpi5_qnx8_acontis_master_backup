#include "EcTaskUser.h"

EcTaskUser::EcTaskUser()
{
    m_ecTaskUserBaseVector.reserve(10);
}

EcTaskUser::~EcTaskUser()
{
}

uint32_t EcTaskUser::addAll(std::vector<EcTaskUserFactoryNs::TaskData>& taskData)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < taskData.size(); i++)
    {
        dwRes |= addTaskUser(EcTaskUserFactory::create(taskData[i]));
    }

    return dwRes;
}

uint32_t EcTaskUser::cleanup()
{
    std::cout << "Cleanup user task from memory start\n";
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
    	std::cout << "Cleanup user task " << i + 1 << " from memory\n";
        delete m_ecTaskUserBaseVector[i];
    }

    std::cout << "Cleanup user task from memory complete\n";

    return dwRes;
}

uint32_t EcTaskUser::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->registerXml();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->registerPublisher();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->registerSubscriber();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->publishData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->subscribeData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskUserBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskUserBaseVector[i]->mainProcess();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskUser::addTaskUser(EcTaskUserBase *pTaskUser)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecTaskUserBaseVector.push_back(pTaskUser);
    std::cout << "m_ecTaskUserBaseVector.size(): " << m_ecTaskUserBaseVector.size() << std::endl;

    return dwRes;
}

EcTaskUserBase* EcTaskUser::getElement(uint32_t index)
{
    if(index < m_ecTaskUserBaseVector.size())
    {
        return m_ecTaskUserBaseVector[index];
    }
    return nullptr;
}
