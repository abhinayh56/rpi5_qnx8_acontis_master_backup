#include "EcSlaveDenNetE.h"

EcSlaveDenNetE::EcSlaveDenNetE(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseActualPosition("ACTUAL_POSITION", m_taskPath, 0, true),
	// m_dseActualVelocity("ACTUAL_VELOCITY", m_taskPath, 0, true),
	// m_dseActualTorque("ACTUAL_TORQUE", m_taskPath, 0, true),
	// m_dseCommandPosition("COMMAND_POSITION", m_taskPath, 0, true),
	// m_dseCommandVelocity("COMMAND_VELOCITY", m_taskPath, 0, true),
	// m_dseCommandTorque("COMMAND_TORQUE", m_taskPath, 0, true),
	// m_dseTiLimitSwitchPressed("TI_LIMIT_SWITCH_PRESSED", m_taskPath, 0, true),
	// m_dseTiClutchButtonPressed("TI_CLUTCH_BUTTON_PRESSED", m_taskPath, 0, true)
{
}

EcSlaveDenNetE::~EcSlaveDenNetE()
{
}

uint32_t EcSlaveDenNetE::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseActualPosition", m_dseActualPosition);
	registerXmlElementDataStoreValue("dseActualVelocity", m_dseActualVelocity);
	registerXmlElementDataStoreValue("dseActualTorque", m_dseActualTorque);
	registerXmlElementDataStoreValue("dseCommandPosition", m_dseCommandPosition);
	registerXmlElementDataStoreValue("dseCommandVelocity", m_dseCommandVelocity);
	registerXmlElementDataStoreValue("dseCommandTorque", m_dseCommandTorque);
	registerXmlElementDataStoreValue("dseTiLimitSwitchPressed", m_dseTiLimitSwitchPressed);
	registerXmlElementDataStoreValue("dseTiClutchButtonPressed", m_dseTiClutchButtonPressed);
    
    return dwRes;
}

uint32_t EcSlaveDenNetE::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_Word);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_position);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_velocity);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Operation_mode_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_inputs_value);

	m_Cia402PdoTx.statusWord.p_isSupported = &m_txPdo.Status_Word.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.Operation_mode_display.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported = &m_txPdo.Actual_position.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported = &m_txPdo.Actual_velocity.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported = &m_txPdo.Torque_actual_value.isSupported;

	m_Cia402PdoTx.statusWord.p_value = &m_txPdo.Status_Word.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value = &m_txPdo.Operation_mode_display.value;
	m_Cia402PdoTx.actualPosition.p_value = &m_txPdo.Actual_position.value;
	m_Cia402PdoTx.actualVelocity.p_value = &m_txPdo.Actual_velocity.value;
	m_Cia402PdoTx.actualTorque.p_value = &m_txPdo.Torque_actual_value.value;

	return dwRes;
}

uint32_t EcSlaveDenNetE::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_Word);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Position_set_point);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Velocity_set_point);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Operation_mode);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_torque);

	m_Cia402PdoRx.controlWord.p_isSupported = &m_rxPdo.Control_Word.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.Operation_mode.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported = &m_rxPdo.Position_set_point.isSupported;
	m_Cia402PdoRx.targetVelocity.p_isSupported = &m_rxPdo.Velocity_set_point.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported = &m_rxPdo.Target_torque.isSupported;

	m_Cia402PdoRx.controlWord.p_value = &m_rxPdo.Control_Word.value;
	m_Cia402PdoRx.modeOfOperation.p_value = &m_rxPdo.Operation_mode.value;
	m_Cia402PdoRx.targetPosition.p_value = &m_rxPdo.Position_set_point.value;
	m_Cia402PdoRx.targetVelocity.p_value = &m_rxPdo.Velocity_set_point.value;
	m_Cia402PdoRx.targetTorque.p_value = &m_rxPdo.Target_torque.value;

	return dwRes;
}

uint32_t EcSlaveDenNetE::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Status_Word, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_position, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_velocity, pBuffer);
	transferInputPdoObject(m_txPdo.Operation_mode_display, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_inputs_value, pBuffer);

	return dwRes;
}

uint32_t EcSlaveDenNetE::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Control_Word, pBuffer);
	transferOutputPdoObject(m_rxPdo.Position_set_point, pBuffer);
	transferOutputPdoObject(m_rxPdo.Velocity_set_point, pBuffer);
	transferOutputPdoObject(m_rxPdo.Operation_mode, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_torque, pBuffer);

	return dwRes;
}

uint32_t EcSlaveDenNetE::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_actualPosition = static_cast<double>(m_txPdo.Actual_position.value - m_offsetPosition) / m_factorPosition;
	m_actualVelocity = static_cast<double>(m_txPdo.Actual_velocity.value - m_offsetVelocity) / m_factorVelocity;
	m_actualTorque = static_cast<double>(m_txPdo.Torque_actual_value.value - m_offsetTorque) / m_factorTorque;

	m_tiLimitSwitchPressed = ((m_txPdo.Digital_inputs_value.value & (1 << 0)) == 0);
	m_tiClutchButtonPressed = ((m_txPdo.Digital_inputs_value.value & (1 << 1)) == (1 << 1));

	return dwRes;
}

uint32_t EcSlaveDenNetE::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Position_set_point.value = static_cast<int32_t>(m_targetPosition * m_factorPosition) + m_offsetPosition;
	m_rxPdo.Velocity_set_point.value = static_cast<int32_t>(m_targetVelocity * m_factorVelocity) + m_offsetVelocity;
	m_rxPdo.Target_torque.value = static_cast<int16_t>(m_targetTorque * m_factorTorque) + m_offsetTorque;

	return dwRes;
}

uint32_t EcSlaveDenNetE::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.publish();
	m_dseActualVelocity.publish();
	m_dseActualTorque.publish();
	m_dseCommandPosition.publish();
	m_dseCommandVelocity.publish();
	m_dseCommandTorque.publish();
	m_dseTiLimitSwitchPressed.publish();
	m_dseTiClutchButtonPressed.publish();

	return dwRes;
}

uint32_t EcSlaveDenNetE::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.subscribe();
	m_dseCommandVelocity.subscribe();
	m_dseCommandTorque.subscribe();

	return dwRes;
}

uint32_t EcSlaveDenNetE::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.set(m_actualPosition);
	// std::cout << "m_actualPosition : " << m_actualPosition << std::endl;
	m_dseActualVelocity.set(m_actualVelocity);
	m_dseActualTorque.set(m_actualTorque);
	m_dseTiLimitSwitchPressed.set(m_tiLimitSwitchPressed);
	m_dseTiClutchButtonPressed.set(m_tiClutchButtonPressed);

	return dwRes;
}

uint32_t EcSlaveDenNetE::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.get(m_targetPosition_temp);
	m_dseCommandVelocity.get(m_targetVelocity);
	m_dseCommandTorque.get(m_targetTorque);

	return dwRes;
}

uint32_t EcSlaveDenNetE::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;
	
	// m_targetPosition = m_targetPosition_temp;

	double max_delta_position = 1.0;
	double delta_position = m_targetPosition_temp - m_actualPosition;

	if(delta_position > max_delta_position)
	{
		delta_position = max_delta_position;
	}
	
	if(delta_position < -max_delta_position)
	{
		delta_position = -max_delta_position;
	}

	m_targetPosition = m_actualPosition + delta_position;
	
	return dwRes;
}

void EcSlaveDenNetE::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Status_Word: " << m_txPdo.Status_Word.value << ", "
	"Actual_position: " << m_txPdo.Actual_position.value << ", "
	"Actual_velocity: " << m_txPdo.Actual_velocity.value << ", "
	"Operation_mode_display: " << (uint16_t)m_txPdo.Operation_mode_display.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_actual_value.value
	<< std::endl;
}

void EcSlaveDenNetE::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_Word: " << m_rxPdo.Control_Word.value << ", "
	"Position_set_point: " << m_rxPdo.Position_set_point.value << ", "
	"Position_set_point: " << m_rxPdo.Velocity_set_point.value << ", "
	"Operation_mode: " << (uint16_t)m_rxPdo.Operation_mode.value << ", "
	"Target_torque: " << m_rxPdo.Target_torque.value
	<< std::endl;
}

uint32_t EcSlaveDenNetE::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.set(m_actualPosition);

    return dwRes;
}

uint32_t EcSlaveDenNetE::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandVelocity.set(m_actualVelocity);

    return dwRes;
}

uint32_t EcSlaveDenNetE::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandTorque.set(m_actualTorque);

    return dwRes;
}
