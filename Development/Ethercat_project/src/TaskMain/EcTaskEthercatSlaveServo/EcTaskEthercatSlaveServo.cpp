#include "EcTaskEthercatSlaveServo.h"

EcTaskEthercatSlaveServo::EcTaskEthercatSlaveServo()
{
	m_ecTaskEthercatSlaveServoBaseVector.reserve(12);
}

EcTaskEthercatSlaveServo::~EcTaskEthercatSlaveServo()
{
}

uint32_t EcTaskEthercatSlaveServo::addAll(std::vector<EcTaskEthercatSlaveServoFactoryNs::TaskData>& taskData)
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < taskData.size(); i++)
    {
        dwRes |= addTaskSlaveServo(EcTaskEthercatSlaveServoFactory::create(taskData[i]));
    }

	return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::cleanup()
{
	std::cout << "Cleanup slaves from memory start\n";
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
    	std::cout << "Cleanup slave " << i + 1 << " from memory\n";
        delete m_ecTaskEthercatSlaveServoBaseVector[i];
    }

    std::cout << "Cleanup slaves from memory complete\n";

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::checkSlave()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        std::cout << "Checking slave presence at " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->checkSlave();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::registerPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
    	std::cout << "Registering TxPdo for " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }

        std::cout << "Registering RxPdo for " << m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveAddress() << ", " <<m_ecTaskEthercatSlaveServoBaseVector[i]->getSlaveName() << std::endl;
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerXml();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerPublisher();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->registerSubscriber();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::transferTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->transferTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::transferRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->transferRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::processTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->processTxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::processRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->processRxPdo();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->publishData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->subscribeData();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->mainProcess();
        if(dwRes != CallbackStatus::SUCCESS)
        {
            return dwRes;
        }
    }

    return dwRes;
}

void EcTaskEthercatSlaveServo::dispTxPdo()
{
    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        m_ecTaskEthercatSlaveServoBaseVector[i]->dispTxPdo();
    }
}

void EcTaskEthercatSlaveServo::dispRxPdo()
{
    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        m_ecTaskEthercatSlaveServoBaseVector[i]->dispRxPdo();
    }
}

uint32_t EcTaskEthercatSlaveServo::addTaskSlaveServo(EcTaskEthercatSlaveServoBase* pSlave)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_ecTaskEthercatSlaveServoBaseVector.push_back(pSlave);
    std::cout << "m_ecTaskEthercatSlaveServoBaseVector.size(): " << m_ecTaskEthercatSlaveServoBaseVector.size() << std::endl;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::faultClear()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->faultClear();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isFault()
{
    bool dwRes = false;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes || m_ecTaskEthercatSlaveServoBaseVector[i]->isFault();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::quickStop()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->quickStop();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isQuickStop()
{
    bool dwRes = false;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes || m_ecTaskEthercatSlaveServoBaseVector[i]->isQuickStop();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::emergencyStop()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->emergencyStop();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isEmergencyStop()
{
    bool dwRes = false;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes || m_ecTaskEthercatSlaveServoBaseVector[i]->isEmergencyStop();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::enable()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->enable();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isEnable()
{
	bool dwRes = true;

	for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
	{
		dwRes = dwRes && m_ecTaskEthercatSlaveServoBaseVector[i]->isEnable();
	}

	return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::disable()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->disable();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isDisable()
{
	bool dwRes = true;

	for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
	{
		dwRes = dwRes && m_ecTaskEthercatSlaveServoBaseVector[i]->isDisable();
	}

	return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setModePosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->setModePosition();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isModePosition()
{
    bool dwRes = true;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes && m_ecTaskEthercatSlaveServoBaseVector[i]->isModePosition();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setModeVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->setModeVelocity();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isModeVelocity()
{
    bool dwRes = true;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes && m_ecTaskEthercatSlaveServoBaseVector[i]->isModeVelocity();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setModeTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->setModeTorque();
    }

    return dwRes;
}

bool EcTaskEthercatSlaveServo::isModeTorque()
{
    bool dwRes = true;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes = dwRes && m_ecTaskEthercatSlaveServoBaseVector[i]->isModeTorque();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setTargetPosition(int32_t targetPosition)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setTargetVelocity(int32_t targetVelocity)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::setTargetTorque(int16_t targetTorque)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

int32_t EcTaskEthercatSlaveServo::getActualPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

int32_t EcTaskEthercatSlaveServo::getActualVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

int16_t EcTaskEthercatSlaveServo::getActualTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->initPosition();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->initVelocity();
    }

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServo::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i = 0; i < m_ecTaskEthercatSlaveServoBaseVector.size(); i++)
    {
        dwRes |= m_ecTaskEthercatSlaveServoBaseVector[i]->initTorque();
    }

    return dwRes;
}

EcTaskEthercatSlaveServoBase* EcTaskEthercatSlaveServo::getElement(uint32_t index)
{
    if(index < m_ecTaskEthercatSlaveServoBaseVector.size())
    {
        return m_ecTaskEthercatSlaveServoBaseVector[index];
    }
    return nullptr;
}
