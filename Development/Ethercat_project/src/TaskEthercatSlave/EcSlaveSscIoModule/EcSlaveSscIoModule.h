#ifndef EC_SLAVE_SSC_IO_MODULE_H
#define EC_SLAVE_SSC_IO_MODULE_H

#include "EcSlaveBase.h"
#include <vector>

namespace EcSlaveSscIoModuleData
{
	enum Bit : uint16_t
	{
		BIT_0  = (1 << 0),
		BIT_1  = (1 << 1),
		BIT_2  = (1 << 2),
		BIT_3  = (1 << 3),
		BIT_4  = (1 << 4),
		BIT_5  = (1 << 5),
		BIT_6  = (1 << 6),
		BIT_7  = (1 << 7),
		BIT_8  = (1 << 8),
		BIT_9  = (1 << 9),
		BIT_10 = (1 << 10),
		BIT_11 = (1 << 11),
		BIT_12 = (1 << 12),
		BIT_13 = (1 << 13),
		BIT_14 = (1 << 14),
		BIT_15 = (1 << 15)
	};

	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint16_t, 16> footpedal_1;
		PdoVariable<0x0006,  2, uint16_t, 16> footpedal_2;
		PdoVariable<0x0006,  3, uint16_t, 16> QUALITY_LEFT;
		PdoVariable<0x0006,  4, uint16_t, 16> QUALITY_RIGHT;
		PdoVariable<0x0006,  5, uint16_t, 16> GRIP_VAL_RIGHT;
		PdoVariable<0x0006,  6, uint16_t, 16> GRIP_VAL_LEFT;
		PdoVariable<0x0006,  7,    float, 32> X_LEFT;
		PdoVariable<0x0006,  8,    float, 32> Y_LEFT;
		PdoVariable<0x0006,  9,    float, 32> Z_LEFT;
		PdoVariable<0x0006, 10,    float, 32> Q1_LEFT;
		PdoVariable<0x0006, 11,    float, 32> Q2_LEFT;
		PdoVariable<0x0006, 12,    float, 32> Q3_LEFT;
		PdoVariable<0x0006, 13,    float, 32> Q4_LEFT;
		PdoVariable<0x0006, 14,    float, 32> X_RIGHT;
		PdoVariable<0x0006, 15,    float, 32> Y_RIGHT;
		PdoVariable<0x0006, 16,    float, 32> Z_RIGHT;
		PdoVariable<0x0006, 17,    float, 32> Q1_RIGHT;
		PdoVariable<0x0006, 18,    float, 32> Q2_RIGHT;
		PdoVariable<0x0006, 19,    float, 32> Q3_RIGHT;
		PdoVariable<0x0006, 20,    float, 32> Q4_RIGHT;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> IPC_TX_1;
		PdoVariable<0x0005,  2, uint16_t, 16> IPC_RX_1;
	};
	#pragma pack(pop)
}

class EcSlaveSscIoModule : public EcSlaveBase
{
public:
	EcSlaveSscIoModule(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveSscIoModule();

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
	EcSlaveSscIoModuleData::TxPdo m_txPdo;
	EcSlaveSscIoModuleData::RxPdo m_rxPdo;

private:
	Data_store_element<bool>               m_dseFootpedalMasterClutch;
	Data_store_element<bool>               m_dseFootpedalCameraClutch;
	Data_store_element<bool>               m_dseFootpedalToggleLeft;
	Data_store_element<bool>               m_dseFootpedalToggleRight;
	Data_store_element<bool>               m_dseFootpedalCutLeft;
	Data_store_element<bool>               m_dseFootpedalCutRight;
	Data_store_element<bool>               m_dseFootpedalCoagLeft;
	Data_store_element<bool>               m_dseFootpedalCoagRight;
	Data_store_element<bool>               m_dseButtonEStop;
	Data_store_element<bool>               m_dseButtonClearFault;
	Data_store_element<uint16_t>           m_dseQualityLeft;
	Data_store_element<uint16_t>           m_dseQualityRight;
	Data_store_element<uint16_t>           m_dseGripValueRight;
	Data_store_element<uint16_t>           m_dseGripValueLeft;
	Data_store_element<std::vector<float>> m_dsePoseLeft;
	Data_store_element<std::vector<float>> m_dsePoseRight;
	Data_store_element<bool>               m_dseHeadTracking;
	Data_store_element<bool>               m_dseFingerClutchLeft;
	Data_store_element<bool>               m_dseFingerClutchRight;
	Data_store_element<bool>               m_dsePalmSensorLeft;
	Data_store_element<bool>               m_dsePalmSensorRight;
	
	bool               m_footpedalMasterClutch;
	bool               m_footpedalCameraClutch;
	bool               m_footpedalToggleLeft;
	bool               m_footpedalToggleRight;
	bool               m_footpedalCutLeft;
	bool               m_footpedalCutRight;
	bool               m_footpedalCoagLeft;
	bool               m_footpedalCoagRight;
	bool               m_buttonEStop;
	bool               m_buttonClearFault;
	uint16_t           m_qualityLeft;
	uint16_t           m_qualityRight;
	uint16_t           m_gripValueRight;
	uint16_t           m_gripValueLeft;
	std::vector<float> m_poseLeft;
	std::vector<float> m_poseRight;
	bool               m_headTracking;
	bool               m_fingerClutchLeft;
	bool               m_fingerClutchRight;
	bool               m_palmSensorLeft;
	bool               m_palmSensorRight;
};

#endif // EC_SLAVE_SSC_IO_MODULE_H
