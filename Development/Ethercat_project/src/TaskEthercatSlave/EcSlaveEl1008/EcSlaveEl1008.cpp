#include "EcSlaveEl1008.h"

EcSlaveEl1008::EcSlaveEl1008(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName)
{
}

EcSlaveEl1008::~EcSlaveEl1008()
{
}

uint32_t EcSlaveEl1008::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
	
    registerXmlElementDataStoreValue("dseInputCh_1", m_dseInputCh_1);
    registerXmlElementDataStoreValue("dseInputCh_2", m_dseInputCh_2);
    registerXmlElementDataStoreValue("dseInputCh_3", m_dseInputCh_3);
    registerXmlElementDataStoreValue("dseInputCh_4", m_dseInputCh_4);
    registerXmlElementDataStoreValue("dseInputCh_5", m_dseInputCh_5);
    registerXmlElementDataStoreValue("dseInputCh_6", m_dseInputCh_6);
    registerXmlElementDataStoreValue("dseInputCh_7", m_dseInputCh_7);
    registerXmlElementDataStoreValue("dseInputCh_8", m_dseInputCh_8);

	// registerXmlAttributeVector("dataVectorStringA", m_dataVectorString);
	// registerXmlAttributeVector("dataVectorboolA",   m_dataVectorbool);
	// registerXmlAttributeVector("dataVectorFloatA",  m_dataVectorFloat);
	// registerXmlAttributeVector("dataVectorDoubleA", m_dataVectorDouble);
	// registerXmlAttributeVector("dataVectorIntS8A",  m_dataVectorIntS8);
	// registerXmlAttributeVector("dataVectorIntS16A", m_dataVectorIntS16);
	// registerXmlAttributeVector("dataVectorIntS32A", m_dataVectorIntS32);
	// registerXmlAttributeVector("dataVectorIntS64A", m_dataVectorIntS64);
	// registerXmlAttributeVector("dataVectorIntU8A",  m_dataVectorIntU8);
	// registerXmlAttributeVector("dataVectorIntU16A", m_dataVectorIntU16);
	// registerXmlAttributeVector("dataVectorIntU32A", m_dataVectorIntU32);
	// registerXmlAttributeVector("dataVectorIntU64A", m_dataVectorIntU64);

	registerXmlElementVector("dataVectorStringE", m_dataVectorString);
	registerXmlElementVector("dataVectorboolE",   m_dataVectorbool);
	registerXmlElementVector("dataVectorFloatE",  m_dataVectorFloat);
	registerXmlElementVector("dataVectorDoubleE", m_dataVectorDouble);
	registerXmlElementVector("dataVectorIntS8E",  m_dataVectorIntS8);
	registerXmlElementVector("dataVectorIntS16E", m_dataVectorIntS16);
	registerXmlElementVector("dataVectorIntS32E", m_dataVectorIntS32);
	registerXmlElementVector("dataVectorIntS64E", m_dataVectorIntS64);
	registerXmlElementVector("dataVectorIntU8E",  m_dataVectorIntU8);
	registerXmlElementVector("dataVectorIntU16E", m_dataVectorIntU16);
	registerXmlElementVector("dataVectorIntU32E", m_dataVectorIntU32);
	registerXmlElementVector("dataVectorIntU64E", m_dataVectorIntU64);
    
    return dwRes;
}

uint32_t EcSlaveEl1008::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_8);

	return dwRes;
}

uint32_t EcSlaveEl1008::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Channel_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_8, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEl1008::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseInputCh_1.publish();
	m_dseInputCh_2.publish();
	m_dseInputCh_3.publish();
	m_dseInputCh_4.publish();
	m_dseInputCh_5.publish();
	m_dseInputCh_6.publish();
	m_dseInputCh_7.publish();
	m_dseInputCh_8.publish();

	return dwRes;
}

uint32_t EcSlaveEl1008::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseInputCh_1.set(m_txPdo.Channel_1.value);
	m_dseInputCh_2.set(m_txPdo.Channel_2.value);
	m_dseInputCh_3.set(m_txPdo.Channel_3.value);
	m_dseInputCh_4.set(m_txPdo.Channel_4.value);
	m_dseInputCh_5.set(m_txPdo.Channel_5.value);
	m_dseInputCh_6.set(m_txPdo.Channel_6.value);
	m_dseInputCh_7.set(m_txPdo.Channel_7.value);
	m_dseInputCh_8.set(m_txPdo.Channel_8.value);

	return dwRes;
}

uint32_t EcSlaveEl1008::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveEl1008::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	// std::cout << "---\n";
	// std::cout << "m_dataVectorString : "; for(uint32_t i = 0; i < m_dataVectorString.size()-1; i++){std::cout << m_dataVectorString[i] << ", ";}std::cout << m_dataVectorString[m_dataVectorString.size()-1] << std::endl;
	// std::cout << "m_dataVectorbool   : "; for(uint32_t i = 0; i < m_dataVectorbool.size()-1;   i++){std::cout << m_dataVectorbool[i]   << ", ";}std::cout << m_dataVectorbool[  m_dataVectorbool.size()-1  ] << std::endl;
	// std::cout << "m_dataVectorFloat  : "; for(uint32_t i = 0; i < m_dataVectorFloat.size()-1;  i++){std::cout << m_dataVectorFloat[i]  << ", ";}std::cout << m_dataVectorFloat[ m_dataVectorFloat.size()-1 ] << std::endl;
	// std::cout << "m_dataVectorDouble : "; for(uint32_t i = 0; i < m_dataVectorDouble.size()-1; i++){std::cout << m_dataVectorDouble[i] << ", ";}std::cout << m_dataVectorDouble[m_dataVectorDouble.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntS8  : "; for(uint32_t i = 0; i < m_dataVectorIntS8.size()-1;  i++){std::cout << (int16_t)m_dataVectorIntS8[i]  << ", ";}std::cout << (int16_t)m_dataVectorIntS8[ m_dataVectorIntS8.size()-1 ] << std::endl;
	// std::cout << "m_dataVectorIntS16 : "; for(uint32_t i = 0; i < m_dataVectorIntS16.size()-1; i++){std::cout << m_dataVectorIntS16[i] << ", ";}std::cout << m_dataVectorIntS16[m_dataVectorIntS16.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntS32 : "; for(uint32_t i = 0; i < m_dataVectorIntS32.size()-1; i++){std::cout << m_dataVectorIntS32[i] << ", ";}std::cout << m_dataVectorIntS32[m_dataVectorIntS32.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntS64 : "; for(uint32_t i = 0; i < m_dataVectorIntS64.size()-1; i++){std::cout << m_dataVectorIntS64[i] << ", ";}std::cout << m_dataVectorIntS64[m_dataVectorIntS64.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntU8  : "; for(uint32_t i = 0; i < m_dataVectorIntU8.size()-1;  i++){std::cout << (uint16_t)m_dataVectorIntU8[i]  << ", ";}std::cout << (uint16_t)m_dataVectorIntU8[ m_dataVectorIntU8.size()-1 ] << std::endl;
	// std::cout << "m_dataVectorIntU16 : "; for(uint32_t i = 0; i < m_dataVectorIntU16.size()-1; i++){std::cout << m_dataVectorIntU16[i] << ", ";}std::cout << m_dataVectorIntU16[m_dataVectorIntU16.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntU32 : "; for(uint32_t i = 0; i < m_dataVectorIntU32.size()-1; i++){std::cout << m_dataVectorIntU32[i] << ", ";}std::cout << m_dataVectorIntU32[m_dataVectorIntU32.size()-1] << std::endl;
	// std::cout << "m_dataVectorIntU64 : "; for(uint32_t i = 0; i < m_dataVectorIntU64.size()-1; i++){std::cout << m_dataVectorIntU64[i] << ", ";}std::cout << m_dataVectorIntU64[m_dataVectorIntU64.size()-1] << std::endl;

	return dwRes;
}

void EcSlaveEl1008::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1: " << m_txPdo.Channel_1.value << ", "
	"Channel_2: " << m_txPdo.Channel_2.value << ", "
	"Channel_3: " << m_txPdo.Channel_3.value << ", "
	"Channel_4: " << m_txPdo.Channel_4.value << ", "
	"Channel_5: " << m_txPdo.Channel_5.value << ", "
	"Channel_6: " << m_txPdo.Channel_6.value << ", "
	"Channel_7: " << m_txPdo.Channel_7.value << ", "
	"Channel_8: " << m_txPdo.Channel_8.value
	<< std::endl;
}

void EcSlaveEl1008::dispRxPdo()
{
}
