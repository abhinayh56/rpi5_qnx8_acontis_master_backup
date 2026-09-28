#ifndef EC_SLAVE_ENCBRKCTRL_H
#define EC_SLAVE_ENCBRKCTRL_H

#include "EcSlaveBase.h"

namespace EcSlaveEncbrkctrlData
{
	#pragma pack(push, 1)
	struct TxPdo
	{
		PdoVariable<0x0006,  1, uint32_t, 32> ENC1;
		PdoVariable<0x0006,  2, uint32_t, 32> ENC2;
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct RxPdo
	{
        PdoVariable<0x0005,  1, uint16_t, 16> BRK1;
		PdoVariable<0x0005,  2, uint16_t, 16> BRK2;
	};
	#pragma pack(pop)
}

class EcSlaveEncbrkctrl : public EcSlaveBase
{
public:
	EcSlaveEncbrkctrl(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

	~EcSlaveEncbrkctrl();

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
	EcSlaveEncbrkctrlData::TxPdo m_txPdo;
	EcSlaveEncbrkctrlData::RxPdo m_rxPdo;

private:
	Data_store_element<uint32_t> m_dseEncoder_1;
	Data_store_element<uint32_t> m_dseEncoder_2;
	Data_store_element<bool> m_dseBrake_1;
	Data_store_element<bool> m_dseBrake_2;

	bool m_brake_1;
	bool m_brake_2;
};

#endif // EC_SLAVE_ENCBRKCTRL_H
