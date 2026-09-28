#ifndef EC_SLAVE_BASE_H
#define EC_SLAVE_BASE_H

#include "EcMaster.h"
#include <vector>
#include <string>
#include "EcPdoVariable.h"
#include "EcInputPdoRegister.h"
#include "EcOutputPdoRegister.h"
#include "EcInputPdoTransfer.h"
#include "EcOutputPdoTransfer.h"
#include "EcAllPdoRegister.h"
#include "EcAllPdoTransfer.h"
#include "EcTaskEthercatSlaveBase.h"

class EcSlaveBase : public EcTaskEthercatSlaveBase
{
public:
	EcSlaveBase(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	virtual ~EcSlaveBase();

	virtual uint32_t checkSlave() override;

protected:
	EC_T_CFG_SLAVE_INFO m_slaveInfo;

	bool present = false;
};

#endif // EC_SLAVE_BASE_H
