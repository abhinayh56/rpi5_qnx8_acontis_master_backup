#include "EcSlaveEl3024.h"

EcSlaveEl3024::EcSlaveEl3024(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseAdcInputCh_1("ADC_INPUT_CH_1", m_taskPath, 0, true),
	// m_dseAdcInputCh_2("ADC_INPUT_CH_2", m_taskPath, 0, true)
{
}

EcSlaveEl3024::~EcSlaveEl3024()
{
}

uint32_t EcSlaveEl3024::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseAdcInputCh_1", m_dseAdcInputCh_1);
    registerXmlElementDataStoreValue("dseAdcInputCh_2", m_dseAdcInputCh_2);
    
    return dwRes;
}

uint32_t EcSlaveEl3024::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Al_Compact_Channel_1_Value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Al_Compact_Channel_2_Value);

	return dwRes;
}

uint32_t EcSlaveEl3024::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Al_Compact_Channel_1_Value, pBuffer);
	transferInputPdoObject(m_txPdo.Al_Compact_Channel_2_Value, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEl3024::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseAdcInputCh_1.publish();
	m_dseAdcInputCh_2.publish();

	return dwRes;
}

uint32_t EcSlaveEl3024::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseAdcInputCh_1.set(m_txPdo.Al_Compact_Channel_1_Value.value);
	m_dseAdcInputCh_2.set(m_txPdo.Al_Compact_Channel_2_Value.value);

	return dwRes;
}

uint32_t EcSlaveEl3024::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl3024::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

void EcSlaveEl3024::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_txPdo.Al_Compact_Channel_1_Value.value << ", "
	"Grip_Offset: " << m_txPdo.Al_Compact_Channel_2_Value.value
	<< std::endl;
}

void EcSlaveEl3024::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | "
	<< std::endl;
}
