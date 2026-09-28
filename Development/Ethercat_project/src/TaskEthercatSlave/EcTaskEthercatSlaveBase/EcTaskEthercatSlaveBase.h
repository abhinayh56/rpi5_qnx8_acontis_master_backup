#ifndef EC_TASK_ETHERCAT_SLAVE_BASE_H
#define EC_TASK_ETHERCAT_SLAVE_BASE_H

#include "ITC.h"
#include "CallbackStatus.h"
#include "EcTaskBase.h"

class EcTaskEthercatSlaveBase : public EcTaskBase
{
public:
	EcTaskEthercatSlaveBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	virtual ~EcTaskEthercatSlaveBase();

	virtual uint32_t checkSlave();

	virtual uint32_t registerTxPdo();

	virtual uint32_t registerRxPdo();

	virtual uint32_t transferTxPdo();

	virtual uint32_t transferRxPdo();

	virtual uint32_t processTxPdo();

	virtual uint32_t processRxPdo();

	virtual void dispTxPdo();

	virtual void dispRxPdo();

	virtual uint16_t getSlaveAddress();

	virtual std::string &getSlaveName();

protected:
	uint16_t m_slaveAddr;
	std::string m_slaveName;
};

#endif // EC_TASK_ETHERCAT_SLAVE_BASE_H
