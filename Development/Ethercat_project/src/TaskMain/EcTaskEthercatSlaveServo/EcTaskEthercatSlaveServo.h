#ifndef EC_TASK_ETHERCAT_SLAVE_SERVO_H
#define EC_TASK_ETHERCAT_SLAVE_SERVO_H

#include <vector>
#include "EcTaskEthercatSlaveServoBase.h"
#include "EcTaskEthercatSlaveServoFactory.h"

class EcTaskEthercatSlaveServo
{
public:
    EcTaskEthercatSlaveServo();

    ~EcTaskEthercatSlaveServo();

    uint32_t addAll(std::vector<EcTaskEthercatSlaveServoFactoryNs::TaskData>& taskData);

    uint32_t cleanup();

    uint32_t checkSlave();

    uint32_t registerPdo();
    
	uint32_t registerXml();
    
    uint32_t registerPublisher();

	uint32_t registerSubscriber();

    uint32_t transferTxPdo();

    uint32_t transferRxPdo();

    uint32_t processTxPdo();

    uint32_t processRxPdo();

    uint32_t publishData();

    uint32_t subscribeData();

    uint32_t mainProcess();

    void dispTxPdo();

    void dispRxPdo();

    uint32_t faultClear();

    bool isFault();

    uint32_t quickStop();

    bool isQuickStop();

    uint32_t emergencyStop();

    bool isEmergencyStop();

    uint32_t enable();

    bool isEnable();

    uint32_t disable();

    bool isDisable();

    uint32_t setModePosition();

    bool isModePosition();

    uint32_t setModeVelocity();

    bool isModeVelocity();
    
    uint32_t setModeTorque();

    bool isModeTorque();

    uint32_t setTargetPosition(int32_t targetPosition);

    uint32_t setTargetVelocity(int32_t targetVelocity);

    uint32_t setTargetTorque(int16_t targetTorque);

    int32_t getActualPosition();

    int32_t getActualVelocity();

    int16_t getActualTorque();

    uint32_t initPosition();

    uint32_t initVelocity();

    uint32_t initTorque();

    EcTaskEthercatSlaveServoBase* getElement(uint32_t index);

private:
    std::vector<EcTaskEthercatSlaveServoBase*> m_ecTaskEthercatSlaveServoBaseVector;
        
    uint32_t addTaskSlaveServo(EcTaskEthercatSlaveServoBase* pSlave);
};

#endif // EC_TASK_ETHERCAT_SLAVE_SERVO_H
