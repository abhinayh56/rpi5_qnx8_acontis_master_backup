#include "EcTaskEthercatSlaveServoBase.h"

EcTaskEthercatSlaveServoBase::EcTaskEthercatSlaveServoBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
    EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName),
    m_actualPosition(0),
    m_actualVelocity(0),
    m_actualTorque(0),
    m_targetPosition(0),
    m_targetVelocity(0),
    m_targetTorque(0),
    m_offsetPosition(0),
    m_offsetVelocity(0),
    m_offsetTorque(0),
    m_factorPosition(1.0),
    m_factorVelocity(1.0),
    m_factorTorque(1.0)
{
}

EcTaskEthercatSlaveServoBase::~EcTaskEthercatSlaveServoBase()
{
}

double EcTaskEthercatSlaveServoBase::getActualPosition()
{
    return m_actualPosition;
}

double EcTaskEthercatSlaveServoBase::getActualVelocity()
{
    return m_actualVelocity;
}

double EcTaskEthercatSlaveServoBase::getActualTorque()
{
    return m_actualTorque;
}

void EcTaskEthercatSlaveServoBase::setTargetPosition(double targetPosition)
{
    m_targetPosition = targetPosition;
}

void EcTaskEthercatSlaveServoBase::setTargetVelocity(double targetVelocity)
{
    m_targetVelocity = targetVelocity;
}

void EcTaskEthercatSlaveServoBase::setTargetTorque(double targetTorque)
{
    m_targetTorque = targetTorque;
}

uint32_t EcTaskEthercatSlaveServoBase::setOffsetPosition(int32_t offsetPosition)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_offsetPosition = offsetPosition;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServoBase::setOffsetVelocity(int32_t offsetVelocity)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_offsetVelocity = offsetVelocity;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServoBase::setOffsetTorque(int16_t offsetTorque)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_offsetTorque = offsetTorque;

    return dwRes;
}

uint32_t EcTaskEthercatSlaveServoBase::getOffsetPosition()
{
    return m_offsetPosition;
}

uint32_t EcTaskEthercatSlaveServoBase::getOffsetVelocity()
{
    return m_offsetVelocity;
}

int16_t EcTaskEthercatSlaveServoBase::getOffsetTorque()
{
    return m_offsetTorque;
}

void EcTaskEthercatSlaveServoBase::setFactorPosition(double factorPosition)
{
    m_factorPosition = factorPosition;
}

void EcTaskEthercatSlaveServoBase::setFactorVelocity(double factorVelocity)
{
    m_factorVelocity = factorVelocity;
}

void EcTaskEthercatSlaveServoBase::setFactorTorque(double factorTorque)
{
    m_factorTorque = factorTorque;
}

double EcTaskEthercatSlaveServoBase::getFactorPosition()
{
    return m_factorPosition;
}

double EcTaskEthercatSlaveServoBase::getFactorVelocity()
{
    return m_factorVelocity;
}

double EcTaskEthercatSlaveServoBase::getFactorTorque()
{
    return m_factorTorque;
}
