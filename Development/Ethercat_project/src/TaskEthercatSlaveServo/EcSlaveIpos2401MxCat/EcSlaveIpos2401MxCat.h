#ifndef EC_SLAVE_IPOS_2401_MX_CAT_H
#define EC_SLAVE_IPOS_2401_MX_CAT_H

#include "EcCia402.h"

namespace EcSlaveIpos2401MxCatData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6041, 0, uint16_t, 16> Statusword;
		PdoVariable<0x6064, 0,  int32_t, 32> Position_actual_value;
		PdoVariable<0x606C, 0,  int32_t, 32> Velocity_actual_value;
		PdoVariable<0x6061, 0,   int8_t,  8> Modes_of_operation_display;
		PdoVariable<0x6077, 0,  int16_t, 16> Torque_current_actual_value;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x6040, 0, uint16_t, 16> Controlword;
        PdoVariable<0x607A, 0,  int32_t, 32> Target_position;
        PdoVariable<0x60FF, 0,  int32_t, 32> Target_velocity;
        PdoVariable<0x6060, 0,   int8_t,  8> Modes_of_operation;
        PdoVariable<0x6071, 0,  int16_t, 16> Target_torque;
	};
	#pragma pack(pop)
}

class EcSlaveIpos2401MxCat : public EcCia402
{
public:
	EcSlaveIpos2401MxCat(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveIpos2401MxCat();

	virtual uint32_t registerXml() override;

	virtual uint32_t registerTxPdo() override;

	virtual uint32_t registerRxPdo() override;

	virtual uint32_t transferTxPdo() override;

	virtual uint32_t transferRxPdo() override;

	virtual uint32_t processTxPdo() override;

	virtual uint32_t processRxPdo() override;

	virtual uint32_t registerPublisher() override;

	virtual uint32_t registerSubscriber() override;

	virtual uint32_t publishData() override;

	virtual uint32_t subscribeData() override;

	virtual uint32_t mainProcess() override;

	virtual void dispTxPdo() override;

	virtual void dispRxPdo() override;

	virtual bool isModePosition() override;
	
	virtual bool isModeVelocity() override;
	
	virtual bool isModeTorque() override;

	virtual uint32_t initPosition() override;

	virtual uint32_t initVelocity() override;

	virtual uint32_t initTorque() override;

protected:
	EcSlaveIpos2401MxCatData::TxPdo m_txPdo;
	EcSlaveIpos2401MxCatData::RxPdo m_rxPdo;

private:
	Data_store_element<double> m_dseActualPosition;
	Data_store_element<double> m_dseActualVelocity;
	Data_store_element<double> m_dseActualTorque;
	Data_store_element<double> m_dseCommandPosition;
	Data_store_element<double> m_dseCommandVelocity;
	Data_store_element<double> m_dseCommandTorque;
};

#endif // EC_SLAVE_IPOS_2401_MX_CAT_H
