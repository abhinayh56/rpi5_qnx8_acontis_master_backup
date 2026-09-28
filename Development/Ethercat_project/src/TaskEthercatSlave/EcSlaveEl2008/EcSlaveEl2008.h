#ifndef EC_SLAVE_EL2008_H
#define EC_SLAVE_EL2008_H

#include "EcSlaveBase.h"

namespace EcSlaveEl2008Data
{
	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x7000, 1, bool, 1> Channel_1;
        PdoVariable<0x7010, 1, bool, 1> Channel_2;
        PdoVariable<0x7020, 1, bool, 1> Channel_3;
        PdoVariable<0x7030, 1, bool, 1> Channel_4;
        PdoVariable<0x7040, 1, bool, 1> Channel_5;
        PdoVariable<0x7050, 1, bool, 1> Channel_6;
        PdoVariable<0x7060, 1, bool, 1> Channel_7;
        PdoVariable<0x7070, 1, bool, 1> Channel_8;
	};
	#pragma pack(pop)
}

class EcSlaveEl2008 : public EcSlaveBase
{
public:
	EcSlaveEl2008(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEl2008();

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
	EcSlaveEl2008Data::RxPdo m_rxPdo;
	uint8_t m_rxPdoValue = 0;
	
private:
	Data_store_element<bool> m_dseOutputCh_1;
	Data_store_element<bool> m_dseOutputCh_2;
	Data_store_element<bool> m_dseOutputCh_3;
	Data_store_element<bool> m_dseOutputCh_4;
	Data_store_element<bool> m_dseOutputCh_5;
	Data_store_element<bool> m_dseOutputCh_6;
	Data_store_element<bool> m_dseOutputCh_7;
	Data_store_element<bool> m_dseOutputCh_8;
};

#endif // EC_SLAVE_EL2008_H
