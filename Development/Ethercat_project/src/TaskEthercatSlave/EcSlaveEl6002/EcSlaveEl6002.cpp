#include "EcSlaveEl6002.h"

EcSlaveEl6002::EcSlaveEl6002(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName),
	// m_dseUartTxDataCh_1("UART_TX_CH_1", m_taskPath, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, true),
	// m_dseUartRxDataCh_1("UART_RX_CH_1", m_taskPath, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, true),
	// m_dseUartTxDataCh_2("UART_TX_CH_2", m_taskPath, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, true),
	// m_dseUartRxDataCh_2("UART_RX_CH_2", m_taskPath, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, true),
	m_uartTxDataCh_1({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
	m_uartRxDataCh_1({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
	m_uartTxDataCh_2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
	m_uartRxDataCh_2({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
	m_ctrlCh_1(0),
	m_ctrlCh_2(0),
	m_statusCh_1(0),
	m_statusCh_2(0)
{
}

EcSlaveEl6002::~EcSlaveEl6002()
{
}

uint32_t EcSlaveEl6002::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreVector("dseUartTxDataCh_1", m_dseUartTxDataCh_1);
    registerXmlElementDataStoreVector("dseUartRxDataCh_1", m_dseUartRxDataCh_1);
    registerXmlElementDataStoreVector("dseUartTxDataCh_2", m_dseUartTxDataCh_2);
    registerXmlElementDataStoreVector("dseUartRxDataCh_2", m_dseUartRxDataCh_2);
    
    return dwRes;
}

uint32_t EcSlaveEl6002::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Status);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_0);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_8);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_9);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_10);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_11);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_12);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_13);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_14);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_15);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_16);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_17);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_18);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_19);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_20);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_1_Data_In_21);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Status);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_0);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_2);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_3);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_4);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_5);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_6);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_7);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_8);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_9);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_10);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_11);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_12);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_13);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_14);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_15);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_16);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_17);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_18);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_19);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_20);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Channel_2_Data_In_21);

	return dwRes;
}

uint32_t EcSlaveEl6002::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Ctrl);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_1_Data_Out_21);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Ctrl);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_0);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_2);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_3);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_4);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_5);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_6);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_7);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_8);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_9);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_10);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_11);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_12);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_13);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_14);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_15);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_16);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_17);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_18);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_19);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_20);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Channel_2_Data_Out_21);

	return dwRes;
}

uint32_t EcSlaveEl6002::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Channel_1_Status, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_0, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_8, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_9, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_10, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_11, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_12, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_13, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_14, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_15, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_16, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_17, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_18, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_19, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_20, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_1_Data_In_21, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Status, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_0, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_1, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_2, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_3, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_4, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_5, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_6, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_7, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_8, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_9, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_10, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_11, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_12, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_13, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_14, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_15, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_16, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_17, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_18, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_19, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_20, pBuffer);
	transferInputPdoObject(m_txPdo.Channel_2_Data_In_21, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEl6002::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Channel_1_Ctrl, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_1_Data_Out_21, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Ctrl, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_0, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_2, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_3, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_4, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_5, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_6, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_7, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_8, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_9, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_10, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_11, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_12, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_13, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_14, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_15, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_16, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_17, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_18, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_19, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_20, pBuffer);
	transferOutputPdoObject(m_rxPdo.Channel_2_Data_Out_21, pBuffer);

	return dwRes;
}

uint32_t EcSlaveEl6002::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_statusCh_1 = m_txPdo.Channel_1_Status.value;
	m_statusCh_2 = m_txPdo.Channel_2_Status.value;

	m_uartRxDataCh_1 = 
	{
		m_txPdo.Channel_1_Data_In_0.value,
		m_txPdo.Channel_1_Data_In_1.value,
		m_txPdo.Channel_1_Data_In_2.value,
		m_txPdo.Channel_1_Data_In_3.value,
		m_txPdo.Channel_1_Data_In_4.value,
		m_txPdo.Channel_1_Data_In_5.value,
		m_txPdo.Channel_1_Data_In_6.value,
		m_txPdo.Channel_1_Data_In_7.value,
		m_txPdo.Channel_1_Data_In_8.value,
		m_txPdo.Channel_1_Data_In_9.value,
		m_txPdo.Channel_1_Data_In_10.value,
		m_txPdo.Channel_1_Data_In_11.value,
		m_txPdo.Channel_1_Data_In_12.value,
		m_txPdo.Channel_1_Data_In_13.value,
		m_txPdo.Channel_1_Data_In_14.value,
		m_txPdo.Channel_1_Data_In_15.value,
		m_txPdo.Channel_1_Data_In_16.value,
		m_txPdo.Channel_1_Data_In_17.value,
		m_txPdo.Channel_1_Data_In_18.value,
		m_txPdo.Channel_1_Data_In_19.value,
		m_txPdo.Channel_1_Data_In_20.value,
		m_txPdo.Channel_1_Data_In_21.value
	};

	m_uartRxDataCh_2 = 
	{
		m_txPdo.Channel_2_Data_In_0.value,
		m_txPdo.Channel_2_Data_In_1.value,
		m_txPdo.Channel_2_Data_In_2.value,
		m_txPdo.Channel_2_Data_In_3.value,
		m_txPdo.Channel_2_Data_In_4.value,
		m_txPdo.Channel_2_Data_In_5.value,
		m_txPdo.Channel_2_Data_In_6.value,
		m_txPdo.Channel_2_Data_In_7.value,
		m_txPdo.Channel_2_Data_In_8.value,
		m_txPdo.Channel_2_Data_In_9.value,
		m_txPdo.Channel_2_Data_In_10.value,
		m_txPdo.Channel_2_Data_In_11.value,
		m_txPdo.Channel_2_Data_In_12.value,
		m_txPdo.Channel_2_Data_In_13.value,
		m_txPdo.Channel_2_Data_In_14.value,
		m_txPdo.Channel_2_Data_In_15.value,
		m_txPdo.Channel_2_Data_In_16.value,
		m_txPdo.Channel_2_Data_In_17.value,
		m_txPdo.Channel_2_Data_In_18.value,
		m_txPdo.Channel_2_Data_In_19.value,
		m_txPdo.Channel_2_Data_In_20.value,
		m_txPdo.Channel_2_Data_In_21.value
	};

	return dwRes;
}

uint32_t EcSlaveEl6002::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Channel_1_Ctrl.value        = m_ctrlCh_1;
	m_rxPdo.Channel_1_Data_Out_0.value  = m_uartTxDataCh_1[0];
	m_rxPdo.Channel_1_Data_Out_1.value  = m_uartTxDataCh_1[1];
	m_rxPdo.Channel_1_Data_Out_2.value  = m_uartTxDataCh_1[2];
	m_rxPdo.Channel_1_Data_Out_3.value  = m_uartTxDataCh_1[3];
	m_rxPdo.Channel_1_Data_Out_4.value  = m_uartTxDataCh_1[4];
	m_rxPdo.Channel_1_Data_Out_5.value  = m_uartTxDataCh_1[5];
	m_rxPdo.Channel_1_Data_Out_6.value  = m_uartTxDataCh_1[6];
	m_rxPdo.Channel_1_Data_Out_7.value  = m_uartTxDataCh_1[7];
	m_rxPdo.Channel_1_Data_Out_8.value  = m_uartTxDataCh_1[8];
	m_rxPdo.Channel_1_Data_Out_9.value  = m_uartTxDataCh_1[9];
	m_rxPdo.Channel_1_Data_Out_10.value = m_uartTxDataCh_1[10];
	m_rxPdo.Channel_1_Data_Out_11.value = m_uartTxDataCh_1[11];
	m_rxPdo.Channel_1_Data_Out_12.value = m_uartTxDataCh_1[12];
	m_rxPdo.Channel_1_Data_Out_13.value = m_uartTxDataCh_1[13];
	m_rxPdo.Channel_1_Data_Out_14.value = m_uartTxDataCh_1[14];
	m_rxPdo.Channel_1_Data_Out_15.value = m_uartTxDataCh_1[15];
	m_rxPdo.Channel_1_Data_Out_16.value = m_uartTxDataCh_1[16];
	m_rxPdo.Channel_1_Data_Out_17.value = m_uartTxDataCh_1[17];
	m_rxPdo.Channel_1_Data_Out_18.value = m_uartTxDataCh_1[18];
	m_rxPdo.Channel_1_Data_Out_19.value = m_uartTxDataCh_1[19];
	m_rxPdo.Channel_1_Data_Out_20.value = m_uartTxDataCh_1[20];
	m_rxPdo.Channel_1_Data_Out_21.value = m_uartTxDataCh_1[21];

	m_rxPdo.Channel_2_Ctrl.value        = m_ctrlCh_2;
	m_rxPdo.Channel_2_Data_Out_0.value  = m_uartTxDataCh_2[0];
	m_rxPdo.Channel_2_Data_Out_1.value  = m_uartTxDataCh_2[1];
	m_rxPdo.Channel_2_Data_Out_2.value  = m_uartTxDataCh_2[2];
	m_rxPdo.Channel_2_Data_Out_3.value  = m_uartTxDataCh_2[3];
	m_rxPdo.Channel_2_Data_Out_4.value  = m_uartTxDataCh_2[4];
	m_rxPdo.Channel_2_Data_Out_5.value  = m_uartTxDataCh_2[5];
	m_rxPdo.Channel_2_Data_Out_6.value  = m_uartTxDataCh_2[6];
	m_rxPdo.Channel_2_Data_Out_7.value  = m_uartTxDataCh_2[7];
	m_rxPdo.Channel_2_Data_Out_8.value  = m_uartTxDataCh_2[8];
	m_rxPdo.Channel_2_Data_Out_9.value  = m_uartTxDataCh_2[9];
	m_rxPdo.Channel_2_Data_Out_10.value = m_uartTxDataCh_2[10];
	m_rxPdo.Channel_2_Data_Out_11.value = m_uartTxDataCh_2[11];
	m_rxPdo.Channel_2_Data_Out_12.value = m_uartTxDataCh_2[12];
	m_rxPdo.Channel_2_Data_Out_13.value = m_uartTxDataCh_2[13];
	m_rxPdo.Channel_2_Data_Out_14.value = m_uartTxDataCh_2[14];
	m_rxPdo.Channel_2_Data_Out_15.value = m_uartTxDataCh_2[15];
	m_rxPdo.Channel_2_Data_Out_16.value = m_uartTxDataCh_2[16];
	m_rxPdo.Channel_2_Data_Out_17.value = m_uartTxDataCh_2[17];
	m_rxPdo.Channel_2_Data_Out_18.value = m_uartTxDataCh_2[18];
	m_rxPdo.Channel_2_Data_Out_19.value = m_uartTxDataCh_2[19];
	m_rxPdo.Channel_2_Data_Out_20.value = m_uartTxDataCh_2[20];
	m_rxPdo.Channel_2_Data_Out_21.value = m_uartTxDataCh_2[21];

	return dwRes;
}

uint32_t EcSlaveEl6002::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseUartRxDataCh_1.publish();
	m_dseUartRxDataCh_2.publish();
	m_dseUartTxDataCh_1.publish();
	m_dseUartTxDataCh_2.publish();

	return dwRes;
}

uint32_t EcSlaveEl6002::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseUartTxDataCh_1.subscribe();
	m_dseUartTxDataCh_2.subscribe();

	return dwRes;
}

uint32_t EcSlaveEl6002::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseUartRxDataCh_1.set(m_uartRxDataCh_1);
	m_dseUartRxDataCh_2.set(m_uartRxDataCh_2);

	return dwRes;
}

uint32_t EcSlaveEl6002::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseUartTxDataCh_1.get(m_uartTxDataCh_1);
	m_dseUartTxDataCh_2.get(m_uartTxDataCh_2);

	return dwRes;
}

uint32_t EcSlaveEl6002::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	// m_ctrlCh_1
	// m_statusCh_1

	// m_ctrlCh_2
	// m_statusCh_2

	return dwRes;
}

void EcSlaveEl6002::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1 (Status, Data_In_0_21): " << m_txPdo.Channel_1_Status.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_0.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_1.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_2.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_3.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_4.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_5.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_6.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_7.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_8.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_9.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_10.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_11.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_12.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_13.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_14.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_15.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_16.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_17.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_18.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_19.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_20.value << ", " <<
	(uint16_t)m_txPdo.Channel_1_Data_In_21.value << ", " <<
	"Channel_2 (Status, Data_In_0_21): " << m_txPdo.Channel_2_Status.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_0.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_1.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_2.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_3.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_4.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_5.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_6.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_7.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_8.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_9.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_10.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_11.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_12.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_13.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_14.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_15.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_16.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_17.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_18.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_19.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_20.value << ", " <<
	(uint16_t)m_txPdo.Channel_2_Data_In_21.value
	<< std::endl;
}

void EcSlaveEl6002::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Channel_1 (Ctrl, Data_Out_0_21): " << m_rxPdo.Channel_1_Ctrl.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_0.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_1.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_2.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_3.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_4.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_5.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_6.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_7.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_8.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_9.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_10.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_11.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_12.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_13.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_14.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_15.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_16.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_17.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_18.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_19.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_20.value << ", " <<
	(uint16_t)m_rxPdo.Channel_1_Data_Out_21.value << ", " <<
	"Channel_2 (Ctrl, Data_Out_0_21): " << m_rxPdo.Channel_2_Ctrl.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_0.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_1.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_2.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_3.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_4.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_5.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_6.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_7.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_8.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_9.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_10.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_11.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_12.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_13.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_14.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_15.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_16.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_17.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_18.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_19.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_20.value << ", " <<
	(uint16_t)m_rxPdo.Channel_2_Data_Out_21.value
	<< std::endl;
}
