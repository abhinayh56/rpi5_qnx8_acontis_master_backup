#include "EcSlaveEncbrkctrl.h"

EcSlaveEncbrkctrl::EcSlaveEncbrkctrl(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName),
	// m_dseEncoder_1("ENCODER_1", m_taskPath, 0, true),
	// m_dseEncoder_2("ENCODER_2", m_taskPath, 0, true),
	// m_dseBrake_1("BRAKE_1", m_taskPath, true, true),
	// m_dseBrake_2("BRAKE_2", m_taskPath, true, true),
	m_brake_1(true),
	m_brake_2(true)
{
}

EcSlaveEncbrkctrl::~EcSlaveEncbrkctrl()
{
}

uint32_t EcSlaveEncbrkctrl::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseEncoder_1", m_dseEncoder_1);
    registerXmlElementDataStoreValue("dseEncoder_2", m_dseEncoder_2);
    registerXmlElementDataStoreValue("dseBrake_1", m_dseBrake_1);
    registerXmlElementDataStoreValue("dseBrake_2", m_dseBrake_2);
    
    return dwRes;
}

uint32_t EcSlaveEncbrkctrl::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ENC1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ENC2);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.BRK1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.BRK2);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.ENC1, pBuffer);
	transferInputPdoObject(m_txPdo.ENC2, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.BRK1, pBuffer);
	transferOutputPdoObject(m_rxPdo.BRK2, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	if(m_brake_1 == true)
	{
		m_rxPdo.BRK1.value = 1;
	}
	else
	{
		m_rxPdo.BRK1.value = 0;
	}

	if(m_brake_2 == true)
	{
		m_rxPdo.BRK2.value = 1;
	}
	else
	{
		m_rxPdo.BRK2.value = 0;
	}

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseEncoder_1.publish();
	m_dseEncoder_2.publish();
	m_dseBrake_1.publish();
	m_dseBrake_2.publish();

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseBrake_1.subscribe();
	m_dseBrake_2.subscribe();

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseEncoder_1.set(m_txPdo.ENC1.value);
	m_dseEncoder_2.set(m_txPdo.ENC2.value);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseBrake_1.get(m_brake_1);
	m_dseBrake_2.get(m_brake_2);

	return dwRes;
}

uint32_t EcSlaveEncbrkctrl::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

void EcSlaveEncbrkctrl::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"ENC1: " << m_txPdo.ENC1.value << ", "
	"ENC2: " << m_txPdo.ENC2.value
	<< std::endl;
}

void EcSlaveEncbrkctrl::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"BRK1: " << m_rxPdo.BRK1.value << ", "
	"BRK2: " << m_rxPdo.BRK2.value
	<< std::endl;
}
