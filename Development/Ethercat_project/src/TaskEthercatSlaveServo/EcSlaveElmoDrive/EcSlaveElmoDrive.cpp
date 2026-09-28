#include "EcSlaveElmoDrive.h"

EcSlaveElmoDrive::EcSlaveElmoDrive(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseActualPosition("ACTUAL_POSITION", m_taskPath, 0, true),
	// m_dseActualVelocity("ACTUAL_VELOCITY", m_taskPath, 0, true),
	// m_dseActualTorque("ACTUAL_TORQUE", m_taskPath, 0, true),
	// m_dseCommandPosition("COMMAND_POSITION", m_taskPath, 0, true),
	// m_dseCommandVelocity("COMMAND_VELOCITY", m_taskPath, 0, true),
	// m_dseCommandTorque("COMMAND_TORQUE", m_taskPath, 0, true)
{
}

EcSlaveElmoDrive::~EcSlaveElmoDrive()
{
}

uint32_t EcSlaveElmoDrive::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseActualPosition", m_dseActualPosition);
	registerXmlElementDataStoreValue("dseActualVelocity", m_dseActualVelocity);
	registerXmlElementDataStoreValue("dseActualTorque", m_dseActualTorque);
	registerXmlElementDataStoreValue("dseCommandPosition", m_dseCommandPosition);
	registerXmlElementDataStoreValue("dseCommandVelocity", m_dseCommandVelocity);
	registerXmlElementDataStoreValue("dseCommandTorque", m_dseCommandTorque);
    
    return dwRes;
}

uint32_t EcSlaveElmoDrive::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_word);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Mode_of_operation_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Position_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Velocity_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Current_actual_value);

	m_Cia402PdoTx.statusWord.p_isSupported = &m_txPdo.Status_word.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.Mode_of_operation_display.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported = &m_txPdo.Position_actual_value.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported = &m_txPdo.Velocity_actual_value.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported = &m_txPdo.Torque_actual_value.isSupported;

	m_Cia402PdoTx.statusWord.p_value = &m_txPdo.Status_word.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value = &m_txPdo.Mode_of_operation_display.value;
	m_Cia402PdoTx.actualPosition.p_value = &m_txPdo.Position_actual_value.value;
	m_Cia402PdoTx.actualVelocity.p_value = &m_txPdo.Velocity_actual_value.value;
	m_Cia402PdoTx.actualTorque.p_value = &m_txPdo.Torque_actual_value.value;

	return dwRes;
}

uint32_t EcSlaveElmoDrive::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Control_word);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Mode_of_operation);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_Torque);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_Position);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Velocity_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_Outputs);

	m_Cia402PdoRx.controlWord.p_isSupported = &m_rxPdo.Control_word.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.Mode_of_operation.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported = &m_rxPdo.Target_Position.isSupported;
	// m_Cia402PdoRx.targetVelocity.p_isSupported = &m_rxPdo.TARGET_VEL.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported = &m_rxPdo.Target_Torque.isSupported;

	m_Cia402PdoRx.controlWord.p_value = &m_rxPdo.Control_word.value;
	m_Cia402PdoRx.modeOfOperation.p_value = &m_rxPdo.Mode_of_operation.value;
	m_Cia402PdoRx.targetPosition.p_value = &m_rxPdo.Target_Position.value;
	// m_Cia402PdoRx.targetVelocity.p_value = &m_rxPdo.TARGET_VEL.value;
	m_Cia402PdoRx.targetTorque.p_value = &m_rxPdo.Target_Torque.value;

	return dwRes;
}

uint32_t EcSlaveElmoDrive::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Status_word, pBuffer);
	transferInputPdoObject(m_txPdo.Mode_of_operation_display, pBuffer);
	transferInputPdoObject(m_txPdo.Position_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Velocity_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_txPdo.Current_actual_value, pBuffer);

	return dwRes;
}

uint32_t EcSlaveElmoDrive::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Control_word, pBuffer);
	transferOutputPdoObject(m_rxPdo.Mode_of_operation, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_Torque, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_Position, pBuffer);
	transferOutputPdoObject(m_rxPdo.Velocity_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_Outputs, pBuffer);

	return dwRes;
}

uint32_t EcSlaveElmoDrive::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_actualPosition = static_cast<double>(m_txPdo.Position_actual_value.value - m_offsetPosition) / m_factorPosition;
	m_actualVelocity = static_cast<double>(m_txPdo.Velocity_actual_value.value - m_offsetVelocity) / m_factorVelocity;
	m_actualTorque = static_cast<double>(m_txPdo.Torque_actual_value.value - m_offsetTorque) / m_factorTorque;

	return dwRes;
}

uint32_t EcSlaveElmoDrive::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Target_Position.value = static_cast<int32_t>(m_targetPosition * m_factorPosition) + m_offsetPosition;
//	m_rxPdo.Target_velocity.value = static_cast<int32_t>(m_targetVelocity * m_factorVelocity) + m_offsetVelocity;
	m_rxPdo.Target_Torque.value = static_cast<int16_t>(m_targetTorque * m_factorTorque) + m_offsetTorque;

	return dwRes;
}

uint32_t EcSlaveElmoDrive::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.publish();
	m_dseActualVelocity.publish();
	m_dseActualTorque.publish();
	m_dseCommandPosition.publish();
	m_dseCommandVelocity.publish();
	m_dseCommandTorque.publish();

	return dwRes;
}

uint32_t EcSlaveElmoDrive::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.subscribe();
	m_dseCommandVelocity.subscribe();
	m_dseCommandTorque.subscribe();

	return dwRes;
}

uint32_t EcSlaveElmoDrive::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.set(m_actualPosition);
	m_dseActualVelocity.set(m_actualVelocity);
	m_dseActualTorque.set(m_actualTorque);

	return dwRes;
}

uint32_t EcSlaveElmoDrive::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.get(m_targetPosition);
	m_dseCommandVelocity.get(m_targetVelocity);
	m_dseCommandTorque.get(m_targetTorque);

	return dwRes;
}

uint32_t EcSlaveElmoDrive::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

void EcSlaveElmoDrive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Status_word: " << m_txPdo.Status_word.value << ", "
	"Mode_of_operation_display: " << (uint16_t)m_txPdo.Mode_of_operation_display.value << ", "
	"Position_actual_value: " << m_txPdo.Position_actual_value.value << ", "
	"Velocity_actual_value: " << m_txPdo.Velocity_actual_value.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_actual_value.value << ", "
	"Digital_Inputs: " << m_txPdo.Digital_Inputs.value << ", "
	"Current_actual_value: " << m_txPdo.Current_actual_value.value
	<< std::endl;
}

void EcSlaveElmoDrive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_word: " << m_rxPdo.Control_word.value << ", "
	"Mode_of_operation: " << (uint16_t)m_rxPdo.Mode_of_operation.value << ", "
	"Target_Torque: " << m_rxPdo.Target_Torque.value << ", "
	"Target_Position: " << m_rxPdo.Target_Position.value << ", "
	"Velocity_Offset: " << m_rxPdo.Velocity_Offset.value << ", "
	"Digital_Outputs: " << m_rxPdo.Digital_Outputs.value
	<< std::endl;
}

uint32_t EcSlaveElmoDrive::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.set(m_actualPosition);
    
    return dwRes;
}

uint32_t EcSlaveElmoDrive::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandVelocity.set(m_actualVelocity);

    return dwRes;
}

uint32_t EcSlaveElmoDrive::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandTorque.set(m_actualTorque);

    return dwRes;
}
