#ifndef EC_SLAVE_RFIDSLAVE_H
#define EC_SLAVE_RFIDSLAVE_H

#include "EcSlaveBase.h"

namespace EcSlaveRfidslaveData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t, 16> Device_ID;
		PdoVariable<0x0006,  2, uint16_t, 16> Second;
		PdoVariable<0x0006,  3, uint16_t, 16> Minute;
		PdoVariable<0x0006,  4, uint16_t, 16> Hour;
		PdoVariable<0x0006,  5, uint16_t, 16> Day;
		PdoVariable<0x0006,  6, uint16_t, 16> Month;
		PdoVariable<0x0006,  7, uint16_t, 16> Year;
		PdoVariable<0x0006,  8, uint16_t, 16> Roll_Offset;
		PdoVariable<0x0006,  9, uint16_t, 16> Pitch_Offset;
		PdoVariable<0x0006, 10, uint16_t, 16> Yaw_Offset;
		PdoVariable<0x0006, 11, uint16_t, 16> Grip_Offset;
		PdoVariable<0x0006, 12, uint16_t, 16> No_of_Usages;
		PdoVariable<0x0006, 13, uint16_t, 16> Max_Usages;
		PdoVariable<0x0006, 14, uint16_t, 16> Digital_Inputs;
		PdoVariable<0x0006, 15, uint16_t, 16> Grip_Counts;
		PdoVariable<0x0006, 16, uint16_t, 16> System_Number;
		PdoVariable<0x0006, 17, uint16_t, 16> Device_UID;
		PdoVariable<0x0006, 18, uint16_t, 16> Spare_Bytes;
		PdoVariable<0x0006, 19, uint16_t, 16> MFG_Day;
		PdoVariable<0x0006, 20, uint16_t, 16> MFG_Month;
		PdoVariable<0x0006, 21, uint16_t, 16> MFG_Year;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> Acknowledge;
		PdoVariable<0x0005,  2, uint16_t, 16> Second;
		PdoVariable<0x0005,  3, uint16_t, 16> Minute;
		PdoVariable<0x0005,  4, uint16_t, 16> Hour;
		PdoVariable<0x0005,  5, uint16_t, 16> Day;
		PdoVariable<0x0005,  6, uint16_t, 16> Month;
		PdoVariable<0x0005,  7, uint16_t, 16> Year;
		PdoVariable<0x0005,  8, uint16_t, 16> No_of_Usages;
		PdoVariable<0x0005,  9, uint16_t, 16> Digital_Outputs;
		PdoVariable<0x0005, 10, uint16_t, 16> Grip_Counts;
		PdoVariable<0x0005, 11, uint16_t, 16> System_Number;
		PdoVariable<0x0005, 12, uint16_t, 16> Led_Red;
		PdoVariable<0x0005, 13, uint16_t, 16> Led_Green;
		PdoVariable<0x0005, 14, uint16_t, 16> LED_Blue;
		PdoVariable<0x0005, 15, uint16_t, 16> Spare_Bytes;
		PdoVariable<0x0005, 16, uint16_t, 16> Roll_Offset;
		PdoVariable<0x0005, 17, uint16_t, 16> Pitch_Offset;
		PdoVariable<0x0005, 18, uint16_t, 16> Yaw_Offset;
		PdoVariable<0x0005, 19, uint16_t, 16> Grip_Offset;
	};
	#pragma pack(pop)
}

class EcSlaveRfidslave : public EcSlaveBase
{
public:
	EcSlaveRfidslave(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveRfidslave();

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

protected:
	EcSlaveRfidslaveData::TxPdo m_txPdo;
	EcSlaveRfidslaveData::RxPdo m_rxPdo;

private:
	Data_store_element<uint16_t> m_dseCurrentDeviceID;
	Data_store_element<uint16_t> m_dseCurrentTimeSecond;
	Data_store_element<uint16_t> m_dseCurrentTimeMinute;
	Data_store_element<uint16_t> m_dseCurrentTimeHour;
	Data_store_element<uint16_t> m_dseCurrentTimeDay;
	Data_store_element<uint16_t> m_dseCurrentTimeMonth;
	Data_store_element<uint16_t> m_dseCurrentTimeYear;
	Data_store_element<double>   m_dseCurrentOffsetRoll;
	Data_store_element<double>   m_dseCurrentOffsetPitch;
	Data_store_element<double>   m_dseCurrentOffsetYaw;
	Data_store_element<double>   m_dseCurrentOffsetGrip;
	Data_store_element<uint16_t> m_dseCurrentNoOfUsages;
	Data_store_element<uint16_t> m_dseCurrentMaxUsages;
	Data_store_element<bool>     m_dseCurrentSterileBarrierStatus;
	Data_store_element<bool>     m_dseCurrentInstrumentStatus;
	Data_store_element<uint16_t> m_dseCurrentGripCounts;
	Data_store_element<uint16_t> m_dseCurrentSystemNumber;
	Data_store_element<uint16_t> m_dseCurrentDeviceUid;
	Data_store_element<uint16_t> m_dseCurrentSpareBytes;
	Data_store_element<uint16_t> m_dseCurrentMfgDay;
	Data_store_element<uint16_t> m_dseCurrentMfgMonth;
	Data_store_element<uint16_t> m_dseCurrentMfgYear;

	Data_store_element<uint16_t> m_dseNewAcknowledge;
	Data_store_element<uint16_t> m_dseNewTimeSecond;
	Data_store_element<uint16_t> m_dseNewTimeMinute;
	Data_store_element<uint16_t> m_dseNewTimeHour;
	Data_store_element<uint16_t> m_dseNewTimeDay;
	Data_store_element<uint16_t> m_dseNewTimeMonth;
	Data_store_element<uint16_t> m_dseNewTimeYear;
	Data_store_element<uint16_t> m_dseNewNoOfUsages;
	Data_store_element<uint16_t> m_dseNewDigitalOutputs;
	Data_store_element<uint16_t> m_dseNewGripCounts;
	Data_store_element<uint16_t> m_dseNewSystemNumber;
	Data_store_element<bool>     m_dseNewLedRed;
	Data_store_element<bool>     m_dseNewLedGreen;
	Data_store_element<bool>     m_dseNewLEDBlue;
	Data_store_element<uint16_t> m_dseNewSpareBytes;
	Data_store_element<double>   m_dseNewOffsetRoll;
	Data_store_element<double>   m_dseNewOffsetPitch;
	Data_store_element<double>   m_dseNewOffsetYaw;
	Data_store_element<double>   m_dseNewOffsetGrip;
	
	uint16_t m_newAcknowledge;
	uint16_t m_newTimeSecond;
	uint16_t m_newTimeMinute;
	uint16_t m_newTimeHour;
	uint16_t m_newTimeDay;
	uint16_t m_newTimeMonth;
	uint16_t m_newTimeYear;
	uint16_t m_newNoOfUsages;
	uint16_t m_newDigitalOutputs;
	uint16_t m_newGripCounts;
	uint16_t m_newSystemNumber;
	bool     m_newLedRed;
	bool     m_newLedGreen;
	bool     m_newLEDBlue;
	uint16_t m_newSpareBytes;
	double   m_newOffsetRoll;
	double   m_newOffsetPitch;
	double   m_newOffsetYaw;
	double   m_newOffsetGrip;
};

#endif // EC_SLAVE_RFIDSLAVE_H
