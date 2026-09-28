#include "EcSlaveBase.h"

EcSlaveBase::EcSlaveBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcTaskEthercatSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName)
{
}

EcSlaveBase::~EcSlaveBase()
{
}

uint32_t EcSlaveBase::checkSlave()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= ecatGetCfgSlaveInfo(true, m_slaveAddr, &m_slaveInfo);
	if (dwRes != CallbackStatus::SUCCESS)
	{
		std::cout << "EtherCAT Slave at address " << m_slaveAddr << " Not FOUND " << std::endl;
		present = false;
		return 1 << 10;
	}

	std::cout << "EtherCAT Slave  " << m_slaveInfo.abyDeviceName << " at address " << m_slaveAddr << "  FOUND " << std::endl;
	present = true;

	return 0;
}
