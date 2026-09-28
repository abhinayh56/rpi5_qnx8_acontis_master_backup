#ifndef EC_SLAVE_EL6002_H
#define EC_SLAVE_EL6002_H

#include "EcSlaveBase.h"
#include <vector>

namespace EcSlaveEl6002Data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6001,  1, uint16_t, 16> Channel_1_Status;
		PdoVariable<0x6000, 17,  uint8_t,  8> Channel_1_Data_In_0;
		PdoVariable<0x6000, 18,  uint8_t,  8> Channel_1_Data_In_1;
		PdoVariable<0x6000, 19,  uint8_t,  8> Channel_1_Data_In_2;
		PdoVariable<0x6000, 20,  uint8_t,  8> Channel_1_Data_In_3;
		PdoVariable<0x6000, 21,  uint8_t,  8> Channel_1_Data_In_4;
		PdoVariable<0x6000, 22,  uint8_t,  8> Channel_1_Data_In_5;
		PdoVariable<0x6000, 23,  uint8_t,  8> Channel_1_Data_In_6;
		PdoVariable<0x6000, 24,  uint8_t,  8> Channel_1_Data_In_7;
		PdoVariable<0x6000, 25,  uint8_t,  8> Channel_1_Data_In_8;
		PdoVariable<0x6000, 26,  uint8_t,  8> Channel_1_Data_In_9;
		PdoVariable<0x6000, 27,  uint8_t,  8> Channel_1_Data_In_10;
		PdoVariable<0x6000, 28,  uint8_t,  8> Channel_1_Data_In_11;
		PdoVariable<0x6000, 29,  uint8_t,  8> Channel_1_Data_In_12;
		PdoVariable<0x6000, 30,  uint8_t,  8> Channel_1_Data_In_13;
		PdoVariable<0x6000, 31,  uint8_t,  8> Channel_1_Data_In_14;
		PdoVariable<0x6000, 32,  uint8_t,  8> Channel_1_Data_In_15;
		PdoVariable<0x6000, 33,  uint8_t,  8> Channel_1_Data_In_16;
		PdoVariable<0x6000, 34,  uint8_t,  8> Channel_1_Data_In_17;
		PdoVariable<0x6000, 35,  uint8_t,  8> Channel_1_Data_In_18;
		PdoVariable<0x6000, 36,  uint8_t,  8> Channel_1_Data_In_19;
		PdoVariable<0x6000, 37,  uint8_t,  8> Channel_1_Data_In_20;
		PdoVariable<0x6000, 38,  uint8_t,  8> Channel_1_Data_In_21;

		PdoVariable<0x6011,  1, uint16_t, 16> Channel_2_Status;
		PdoVariable<0x6010, 17,  uint8_t,  8> Channel_2_Data_In_0;
		PdoVariable<0x6010, 18,  uint8_t,  8> Channel_2_Data_In_1;
		PdoVariable<0x6010, 19,  uint8_t,  8> Channel_2_Data_In_2;
		PdoVariable<0x6010, 20,  uint8_t,  8> Channel_2_Data_In_3;
		PdoVariable<0x6010, 21,  uint8_t,  8> Channel_2_Data_In_4;
		PdoVariable<0x6010, 22,  uint8_t,  8> Channel_2_Data_In_5;
		PdoVariable<0x6010, 23,  uint8_t,  8> Channel_2_Data_In_6;
		PdoVariable<0x6010, 24,  uint8_t,  8> Channel_2_Data_In_7;
		PdoVariable<0x6010, 25,  uint8_t,  8> Channel_2_Data_In_8;
		PdoVariable<0x6010, 26,  uint8_t,  8> Channel_2_Data_In_9;
		PdoVariable<0x6010, 27,  uint8_t,  8> Channel_2_Data_In_10;
		PdoVariable<0x6010, 28,  uint8_t,  8> Channel_2_Data_In_11;
		PdoVariable<0x6010, 29,  uint8_t,  8> Channel_2_Data_In_12;
		PdoVariable<0x6010, 30,  uint8_t,  8> Channel_2_Data_In_13;
		PdoVariable<0x6010, 31,  uint8_t,  8> Channel_2_Data_In_14;
		PdoVariable<0x6010, 32,  uint8_t,  8> Channel_2_Data_In_15;
		PdoVariable<0x6010, 33,  uint8_t,  8> Channel_2_Data_In_16;
		PdoVariable<0x6010, 34,  uint8_t,  8> Channel_2_Data_In_17;
		PdoVariable<0x6010, 35,  uint8_t,  8> Channel_2_Data_In_18;
		PdoVariable<0x6010, 36,  uint8_t,  8> Channel_2_Data_In_19;
		PdoVariable<0x6010, 37,  uint8_t,  8> Channel_2_Data_In_20;
		PdoVariable<0x6010, 38,  uint8_t,  8> Channel_2_Data_In_21;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
		PdoVariable<0x7001,  1, uint16_t, 16> Channel_1_Ctrl;
		PdoVariable<0x7000, 17,  uint8_t,  8> Channel_1_Data_Out_0;
		PdoVariable<0x7000, 18,  uint8_t,  8> Channel_1_Data_Out_1;
		PdoVariable<0x7000, 19,  uint8_t,  8> Channel_1_Data_Out_2;
		PdoVariable<0x7000, 20,  uint8_t,  8> Channel_1_Data_Out_3;
		PdoVariable<0x7000, 21,  uint8_t,  8> Channel_1_Data_Out_4;
		PdoVariable<0x7000, 22,  uint8_t,  8> Channel_1_Data_Out_5;
		PdoVariable<0x7000, 23,  uint8_t,  8> Channel_1_Data_Out_6;
		PdoVariable<0x7000, 24,  uint8_t,  8> Channel_1_Data_Out_7;
		PdoVariable<0x7000, 25,  uint8_t,  8> Channel_1_Data_Out_8;
		PdoVariable<0x7000, 26,  uint8_t,  8> Channel_1_Data_Out_9;
		PdoVariable<0x7000, 27,  uint8_t,  8> Channel_1_Data_Out_10;
		PdoVariable<0x7000, 28,  uint8_t,  8> Channel_1_Data_Out_11;
		PdoVariable<0x7000, 29,  uint8_t,  8> Channel_1_Data_Out_12;
		PdoVariable<0x7000, 30,  uint8_t,  8> Channel_1_Data_Out_13;
		PdoVariable<0x7000, 31,  uint8_t,  8> Channel_1_Data_Out_14;
		PdoVariable<0x7000, 32,  uint8_t,  8> Channel_1_Data_Out_15;
		PdoVariable<0x7000, 33,  uint8_t,  8> Channel_1_Data_Out_16;
		PdoVariable<0x7000, 34,  uint8_t,  8> Channel_1_Data_Out_17;
		PdoVariable<0x7000, 35,  uint8_t,  8> Channel_1_Data_Out_18;
		PdoVariable<0x7000, 36,  uint8_t,  8> Channel_1_Data_Out_19;
		PdoVariable<0x7000, 37,  uint8_t,  8> Channel_1_Data_Out_20;
		PdoVariable<0x7000, 38,  uint8_t,  8> Channel_1_Data_Out_21;

		PdoVariable<0x7011,  1, uint16_t, 16> Channel_2_Ctrl;
		PdoVariable<0x7010, 17,  uint8_t,  8> Channel_2_Data_Out_0;
		PdoVariable<0x7010, 18,  uint8_t,  8> Channel_2_Data_Out_1;
		PdoVariable<0x7010, 19,  uint8_t,  8> Channel_2_Data_Out_2;
		PdoVariable<0x7010, 20,  uint8_t,  8> Channel_2_Data_Out_3;
		PdoVariable<0x7010, 21,  uint8_t,  8> Channel_2_Data_Out_4;
		PdoVariable<0x7010, 22,  uint8_t,  8> Channel_2_Data_Out_5;
		PdoVariable<0x7010, 23,  uint8_t,  8> Channel_2_Data_Out_6;
		PdoVariable<0x7010, 24,  uint8_t,  8> Channel_2_Data_Out_7;
		PdoVariable<0x7010, 25,  uint8_t,  8> Channel_2_Data_Out_8;
		PdoVariable<0x7010, 26,  uint8_t,  8> Channel_2_Data_Out_9;
		PdoVariable<0x7010, 27,  uint8_t,  8> Channel_2_Data_Out_10;
		PdoVariable<0x7010, 28,  uint8_t,  8> Channel_2_Data_Out_11;
		PdoVariable<0x7010, 29,  uint8_t,  8> Channel_2_Data_Out_12;
		PdoVariable<0x7010, 30,  uint8_t,  8> Channel_2_Data_Out_13;
		PdoVariable<0x7010, 31,  uint8_t,  8> Channel_2_Data_Out_14;
		PdoVariable<0x7010, 32,  uint8_t,  8> Channel_2_Data_Out_15;
		PdoVariable<0x7010, 33,  uint8_t,  8> Channel_2_Data_Out_16;
		PdoVariable<0x7010, 34,  uint8_t,  8> Channel_2_Data_Out_17;
		PdoVariable<0x7010, 35,  uint8_t,  8> Channel_2_Data_Out_18;
		PdoVariable<0x7010, 36,  uint8_t,  8> Channel_2_Data_Out_19;
		PdoVariable<0x7010, 37,  uint8_t,  8> Channel_2_Data_Out_20;
		PdoVariable<0x7010, 38,  uint8_t,  8> Channel_2_Data_Out_21;
	};
	#pragma pack(pop)
}

class EcSlaveEl6002 : public EcSlaveBase
{
public:
	EcSlaveEl6002(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl6002();

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
	EcSlaveEl6002Data::TxPdo m_txPdo;
	EcSlaveEl6002Data::RxPdo m_rxPdo;

private:
	Data_store_element<std::vector<uint8_t>> m_dseUartTxDataCh_1;
	Data_store_element<std::vector<uint8_t>> m_dseUartRxDataCh_1;
	Data_store_element<std::vector<uint8_t>> m_dseUartTxDataCh_2;
	Data_store_element<std::vector<uint8_t>> m_dseUartRxDataCh_2;

	std::vector<uint8_t> m_uartTxDataCh_1;
	std::vector<uint8_t> m_uartRxDataCh_1;
	std::vector<uint8_t> m_uartTxDataCh_2;
	std::vector<uint8_t> m_uartRxDataCh_2;

	uint8_t m_ctrlCh_1;
	uint8_t m_ctrlCh_2;
	uint8_t m_statusCh_1;
	uint8_t m_statusCh_2;
};

#endif // EC_SLAVE_EL6002_H
