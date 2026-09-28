#ifndef EC_ALL_PDO_REGISTER_H
#define EC_ALL_PDO_REGISTER_H

#include "EcMaster.h"
#include <iostream>

inline uint32_t lookupAllPdoObjectByAddress(const uint32_t& slaveAddress, EC_T_CFG_SLAVE_INFO& slaveInfo)
{
	std::cout << "-----------\n";

	EC_T_DWORD result = ecatGetCfgSlaveInfo(true, slaveAddress, &slaveInfo);

	// check for the slave with the name slaveName
	if (result != EC_E_NOERROR)
	{
		std::cout << "\tEtherCAT Slave at address " << slaveAddress << " Not FOUND " << std::endl;
		return 1<<10;
	}

	std::cout << "\tEtherCAT Slave  " << slaveInfo.abyDeviceName << " at address " << slaveAddress << "  FOUND " << std::endl;

	return 0;
}

inline uint32_t lookupAllPdoObjectByName(const std::string& slaveName, EC_T_CFG_SLAVE_INFO& slaveInfo)
{
	std::cout << "-----------\n";

	EC_T_DWORD numSlaves = ecatGetNumConfiguredSlaves();

	bool slaveFound = false;

	for (EC_T_DWORD slaveIndex = 0; slaveIndex < numSlaves; ++slaveIndex)
	{
		EC_T_CFG_SLAVE_INFO tempSlaveInfo;

		EC_T_WORD fixedAddr;
		EC_T_DWORD result = ecatGetSlaveFixedAddr(slaveIndex, &fixedAddr);
		EC_T_DWORD slaveId = ecatGetSlaveId(fixedAddr);

		result = ecatGetCfgSlaveInfo(true, slaveId, &tempSlaveInfo);

		// check for the slave with the name slaveName
		if ((result == EC_E_NOERROR) && (slaveName == tempSlaveInfo.abyDeviceName))
		{
			slaveInfo = tempSlaveInfo;
			slaveFound = true;
			break;
		}
	}

	std::cout << "\tEtherCAT Slave  " << slaveName << (slaveFound ? "FOUND" : "NOT FOUND") << std::endl;

	return slaveFound ? 0 : (1<<10);
}

#endif // EC_ALL_PDO_REGISTER_H
