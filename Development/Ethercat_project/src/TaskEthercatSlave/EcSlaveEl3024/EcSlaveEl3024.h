#ifndef EC_SLAVE_EL3024_H
#define EC_SLAVE_EL3024_H

#include "EcSlaveBase.h"

namespace EcSlaveEl3024Data
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x6000, 17, int16_t, 16> Al_Compact_Channel_1_Value;
		PdoVariable<0x6010, 17, int16_t, 16> Al_Compact_Channel_2_Value;
	};
	#pragma pack(pop)
}

class EcSlaveEl3024 : public EcSlaveBase
{
public:
	EcSlaveEl3024(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl3024();

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
	EcSlaveEl3024Data::TxPdo m_txPdo;

private:
	Data_store_element<int16_t> m_dseAdcInputCh_1;
	Data_store_element<int16_t> m_dseAdcInputCh_2;
};

#endif // EC_SLAVE_EL3024_H
