#include "EcTaskEthercatSlaveBase.h"

EcTaskEthercatSlaveBase::EcTaskEthercatSlaveBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
    EcTaskBase(taskId, taskName, taskPath),
    m_slaveAddr(slaveAddr),
    m_slaveName(slaveName)
{
}

EcTaskEthercatSlaveBase::~EcTaskEthercatSlaveBase()
{
}

uint32_t EcTaskEthercatSlaveBase::checkSlave()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::registerTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::registerRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::transferTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::transferRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::processTxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveBase::processRxPdo()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

void EcTaskEthercatSlaveBase::dispTxPdo()
{
}

void EcTaskEthercatSlaveBase::dispRxPdo()
{
}

uint16_t EcTaskEthercatSlaveBase::getSlaveAddress()
{
    return m_slaveAddr;
}

std::string &EcTaskEthercatSlaveBase::getSlaveName()
{
    return m_slaveName;
}
