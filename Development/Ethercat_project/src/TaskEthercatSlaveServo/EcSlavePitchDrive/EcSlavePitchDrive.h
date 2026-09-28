#ifndef EC_SLAVE_PITCH_DRIVE_H
#define EC_SLAVE_PITCH_DRIVE_H

#include "EcCia402.h"

namespace EcSlavePitchDriveData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6064, 0,  int32_t, 32> ACT_POS;
		PdoVariable<0x6041, 0, uint16_t, 16> STATUS_WD;
		PdoVariable<0x6077, 0,  int16_t, 16> ACT_TOR;
		PdoVariable<0x6061, 0,   int8_t,  8> OPMODE_DISP;
		PdoVariable<0x603F, 0, uint16_t, 16> ERROR_CODE;
		PdoVariable<0x3002, 0,  uint8_t,  8> DIG_IN;
		PdoVariable<0x606C, 0,  int32_t, 32> ACT_VEL;
		PdoVariable<0x3007, 0, uint16_t, 16> ADC_VAL;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
		PdoVariable<0x607A, 0,  int32_t, 32> TARGET_POSE;
		PdoVariable<0x6040, 0, uint16_t, 16> CONTROL_WD;
		PdoVariable<0x6071, 0,  int16_t, 16> TARGET_TORQ;
		PdoVariable<0x6060, 0,   int8_t,  8> OP_MODE;
		PdoVariable<0x3004, 0, uint16_t, 16> ERROR_CLEAR;
		PdoVariable<0x3001, 0,  uint8_t,  8> DIG_OUT;
		PdoVariable<0x60FF, 0,  int32_t, 32> TARGET_VEL;
	};
	#pragma pack(pop)
}

class EcSlavePitchDrive : public EcCia402
{
public:
	EcSlavePitchDrive(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlavePitchDrive();

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

	virtual uint32_t initPosition() override;

	virtual uint32_t initVelocity() override;

	virtual uint32_t initTorque() override;

protected:
	EcSlavePitchDriveData::TxPdo m_txPdo;
	EcSlavePitchDriveData::RxPdo m_rxPdo;

private:
	Data_store_element<double> m_dseActualPosition;
	Data_store_element<double> m_dseActualVelocity;
	Data_store_element<double> m_dseActualTorque;
	Data_store_element<double> m_dseCommandPosition;
	Data_store_element<double> m_dseCommandVelocity;
	Data_store_element<double> m_dseCommandTorque;
};

#endif // EC_SLAVE_PITCH_DRIVE_H
