#ifndef EC_SLAVE_EL1008_H
#define EC_SLAVE_EL1008_H

#include "EcSlaveBase.h"

namespace EcSlaveEl1008Data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6000, 1, bool, 1> Channel_1;
		PdoVariable<0x6010, 1, bool, 1> Channel_2;
		PdoVariable<0x6020, 1, bool, 1> Channel_3;
		PdoVariable<0x6030, 1, bool, 1> Channel_4;
		PdoVariable<0x6040, 1, bool, 1> Channel_5;
		PdoVariable<0x6050, 1, bool, 1> Channel_6;
		PdoVariable<0x6060, 1, bool, 1> Channel_7;
		PdoVariable<0x6070, 1, bool, 1> Channel_8;
	};
	#pragma pack(pop)
}

class EcSlaveEl1008 : public EcSlaveBase
{
public:
	EcSlaveEl1008(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);
	
	~EcSlaveEl1008();

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
	EcSlaveEl1008Data::TxPdo m_txPdo;

private:
	Data_store_element<bool> m_dseInputCh_1;
	Data_store_element<bool> m_dseInputCh_2;
	Data_store_element<bool> m_dseInputCh_3;
	Data_store_element<bool> m_dseInputCh_4;
	Data_store_element<bool> m_dseInputCh_5;
	Data_store_element<bool> m_dseInputCh_6;
	Data_store_element<bool> m_dseInputCh_7;
	Data_store_element<bool> m_dseInputCh_8;

	std::vector<std::string> m_dataVectorString;
	std::vector<bool>        m_dataVectorbool;
	std::vector<float>       m_dataVectorFloat;
	std::vector<double>      m_dataVectorDouble;
	std::vector<int8_t>      m_dataVectorIntS8;
	std::vector<int16_t>     m_dataVectorIntS16;
	std::vector<int32_t>     m_dataVectorIntS32;
	std::vector<int64_t>     m_dataVectorIntS64;
	std::vector<uint8_t>     m_dataVectorIntU8;
	std::vector<uint16_t>    m_dataVectorIntU16;
	std::vector<uint32_t>    m_dataVectorIntU32;
	std::vector<uint64_t>    m_dataVectorIntU64;


};

#endif // EC_SLAVE_EL1008_H
