#include "EcTaskRobotControl.h"

EcTaskRobotControl::EcTaskRobotControl()
{
}

EcTaskRobotControl::~EcTaskRobotControl()
{
}

uint32_t EcTaskRobotControl::addAll(std::vector<EcTaskRobotControlFactoryNs::TaskData>& taskData)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < taskData.size(); i++)
    {
        dwRes |= addTaskRobotControl(EcTaskRobotControlFactory::create(taskData[i]));
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::cleanup()
{
    std::cout << "Cleanup robot control task from memory start\n";
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
    	std::cout << "Cleanup robot control task " << i + 1 << " from memory\n";
        delete m_ecTaskRobotControlBaseVector[i];
    }

    std::cout << "Cleanup robot control task from memory complete\n";

    return dwRes;
}

uint32_t EcTaskRobotControl::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->registerXml();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->registerPublisher();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->registerSubscriber();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->publishData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->subscribeData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(int i = 0; i < m_numTaskRobotControl; i++)
    {
        dwRes |= m_ecTaskRobotControlBaseVector[i]->mainProcess();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskRobotControl::addTaskRobotControl(EcTaskRobotControlBase* pTaskRobotControl)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecTaskRobotControlBaseVector.push_back(pTaskRobotControl);
    m_numTaskRobotControl = m_ecTaskRobotControlBaseVector.size();
    std::cout << "m_ecTaskRobotControlBaseVector.size(): " << m_ecTaskRobotControlBaseVector.size() << std::endl;

    return dwRes;
}

EcTaskRobotControlBase* EcTaskRobotControl::getElement(uint32_t index)
{
    if(index < m_ecTaskRobotControlBaseVector.size())
    {
        return m_ecTaskRobotControlBaseVector[index];
    }
    return nullptr;
}
