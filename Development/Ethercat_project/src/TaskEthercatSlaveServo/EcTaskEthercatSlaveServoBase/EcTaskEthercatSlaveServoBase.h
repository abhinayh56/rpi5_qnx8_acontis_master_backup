#ifndef EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H
#define EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H

#include "EcSlaveBase.h"

class EcTaskEthercatSlaveServoBase : public EcSlaveBase
{
public:
    EcTaskEthercatSlaveServoBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

    virtual ~EcTaskEthercatSlaveServoBase();

    virtual uint32_t faultClear() = 0;

    virtual bool isFault() = 0;

    virtual uint32_t quickStop() = 0;

    virtual bool isQuickStop() = 0;

    virtual uint32_t emergencyStop() = 0;

    virtual bool isEmergencyStop() = 0;

    virtual uint32_t enable() = 0;

    virtual bool isEnable() = 0;

    virtual uint32_t disable() = 0;

    virtual bool isDisable() = 0;

    virtual uint32_t setModePosition() = 0;

    virtual bool isModePosition() = 0;

    virtual uint32_t setModeVelocity() = 0;

    virtual bool isModeVelocity() = 0;
    
    virtual uint32_t setModeTorque() = 0;

    virtual bool isModeTorque() = 0;

    virtual int32_t getActualPositionIU() = 0;

    virtual int32_t getActualVelocityIU() = 0;

    virtual int16_t getActualTorqueIU() = 0;

    virtual uint32_t setTargetPositionIU(int32_t targetPosition) = 0;

    virtual uint32_t setTargetVelocityIU(int32_t targetVelocity) = 0;

    virtual uint32_t setTargetTorqueIU(int16_t targetTorque) = 0;

    virtual uint32_t initPosition() = 0;

    virtual uint32_t initVelocity() = 0;

    virtual uint32_t initTorque() = 0;

    virtual double getActualPosition();

    virtual double getActualVelocity();

    virtual double getActualTorque();

    virtual void setTargetPosition(double targetPosition);

    virtual void setTargetVelocity(double targetVelocity);

    virtual void setTargetTorque(double targetTorque);

    virtual uint32_t setOffsetPosition(int32_t offsetPosition);

    virtual uint32_t setOffsetVelocity(int32_t offsetVelocity);

    virtual uint32_t setOffsetTorque(int16_t offsetTorque);

    virtual uint32_t getOffsetPosition();

    virtual uint32_t getOffsetVelocity();

    virtual int16_t getOffsetTorque();

    virtual void setFactorPosition(double factorPosition);

    virtual void setFactorVelocity(double factorVelocity);

    virtual void setFactorTorque(double factorTorque);

    virtual double getFactorPosition();

    virtual double getFactorVelocity();

    virtual double getFactorTorque();

protected:
    double m_actualPosition;
    double m_actualVelocity;
    double m_actualTorque;
    
    double m_targetPosition;
    double m_targetVelocity;
    double m_targetTorque;

    int32_t m_offsetPosition = 0;
    int32_t m_offsetVelocity = 0;
    int16_t m_offsetTorque = 0;

    double m_factorPosition = 1.0;
    double m_factorVelocity = 1.0;
    double m_factorTorque = 1.0;
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_BASE_H
