#include "EcTaskEthercatSlave.h"

EcTaskEthercatSlave::EcTaskEthercatSlave()
{
	m_ecTaskEthercatSlaveBaseVector.reserve(12);
}

EcTaskEthercatSlave::~EcTaskEthercatSlave()
{
}

uint32_t EcTaskEthercatSlave::addAll(std::vector<EcTaskEthercatSlaveFactoryNs::TaskData>& taskData)
{
	uint32_t dwRes = CallbackStatus::SUCCESS;
    
    for(uint32_t i = 0 ; i < taskData.size(); i++)
	{
        dwRes |= addTaskSlave(EcTaskEthercatSlaveFactory::create(taskData[i]));
    }

	return dwRes;
}

uint32_t EcTaskEthercatSlave::cleanup()
{
	std::cout << "Cleanup slaves from memory start\n";
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_ecTaskEthercatSlaveBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

uint32_t EcTaskEthercatSlave::checkSlave()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        std::cout << "Checking slave presence at " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->checkSlave();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::registerPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
    	std::cout << "Registering TxPdo for " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_ecTaskEthercatSlaveBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerXml();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerPublisher();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->registerSubscriber();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::transferTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->transferTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::transferRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->transferRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::processTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->processTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::processRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->processRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->publishData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->subscribeData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlave::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveBaseVector[i]->mainProcess();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercatSlave::dispTxPdo()
{
    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        m_ecTaskEthercatSlaveBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercatSlave::dispRxPdo()
{
    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveBaseVector.size(); i++)
    {
        m_ecTaskEthercatSlaveBaseVector[i]->dispRxPdo();
    }
}

uint32_t EcTaskEthercatSlave::addTaskSlave(EcTaskEthercatSlaveBase* pSlave)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecTaskEthercatSlaveBaseVector.push_back(pSlave);
    std::cout << "m_ecTaskEthercatSlaveBaseVector.size(): " << m_ecTaskEthercatSlaveBaseVector.size() << std::endl;

    return dwRes;
}

EcTaskEthercatSlaveBase* EcTaskEthercatSlave::getElement(uint32_t index)
{
    if(index < m_ecTaskEthercatSlaveBaseVector.size())
    {
        return m_ecTaskEthercatSlaveBaseVector[index];
    }
    return nullptr;
}
