#include "EcSlaveIpos2401MxCat.h"

EcSlaveIpos2401MxCat::EcSlaveIpos2401MxCat(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseActualPosition("ACTUAL_POSITION", m_taskPath, 0, true),
	// m_dseActualVelocity("ACTUAL_VELOCITY", m_taskPath, 0, true),
	// m_dseActualTorque("ACTUAL_TORQUE", m_taskPath, 0, true),
	// m_dseCommandPosition("COMMAND_POSITION", m_taskPath, 0, true),
	// m_dseCommandVelocity("COMMAND_VELOCITY", m_taskPath, 0, true),
	// m_dseCommandTorque("COMMAND_TORQUE", m_taskPath, 0, true)
{
}

EcSlaveIpos2401MxCat::~EcSlaveIpos2401MxCat()
{
}

uint32_t EcSlaveIpos2401MxCat::registerXml()
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

uint32_t EcSlaveIpos2401MxCat::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Statusword);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Position_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Velocity_actual_value);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Modes_of_operation_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_current_actual_value);

	m_Cia402PdoTx.statusWord.p_isSupported             = &m_txPdo.Statusword.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.Modes_of_operation_display.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported         = &m_txPdo.Position_actual_value.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported         = &m_txPdo.Velocity_actual_value.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported           = &m_txPdo.Torque_current_actual_value.isSupported;

	m_Cia402PdoTx.statusWord.p_value                   = &m_txPdo.Statusword.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value       = &m_txPdo.Modes_of_operation_display.value;
	m_Cia402PdoTx.actualPosition.p_value               = &m_txPdo.Position_actual_value.value;
	m_Cia402PdoTx.actualVelocity.p_value               = &m_txPdo.Velocity_actual_value.value;
	m_Cia402PdoTx.actualTorque.p_value                 = &m_txPdo.Torque_current_actual_value.value;

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Controlword);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_position);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_velocity);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Modes_of_operation);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Target_torque);

	m_Cia402PdoRx.controlWord.p_isSupported     = &m_rxPdo.Controlword.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.Modes_of_operation.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported  = &m_rxPdo.Target_position.isSupported;
	m_Cia402PdoRx.targetVelocity.p_isSupported  = &m_rxPdo.Target_velocity.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported    = &m_rxPdo.Target_torque.isSupported;

	m_Cia402PdoRx.controlWord.p_value           = &m_rxPdo.Controlword.value;
	m_Cia402PdoRx.modeOfOperation.p_value       = &m_rxPdo.Modes_of_operation.value;
	m_Cia402PdoRx.targetPosition.p_value        = &m_rxPdo.Target_position.value;
	m_Cia402PdoRx.targetVelocity.p_value        = &m_rxPdo.Target_velocity.value;
	m_Cia402PdoRx.targetTorque.p_value          = &m_rxPdo.Target_torque.value;

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Statusword, pBuffer);
	transferInputPdoObject(m_txPdo.Position_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Velocity_actual_value, pBuffer);
	transferInputPdoObject(m_txPdo.Modes_of_operation_display, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_current_actual_value, pBuffer);

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Controlword, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_position, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_velocity, pBuffer);
	transferOutputPdoObject(m_rxPdo.Modes_of_operation, pBuffer);
	transferOutputPdoObject(m_rxPdo.Target_torque, pBuffer);

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_actualPosition = static_cast<double>(m_txPdo.Position_actual_value.value - m_offsetPosition) / m_factorPosition;
	m_actualVelocity = static_cast<double>(m_txPdo.Velocity_actual_value.value - m_offsetVelocity) / m_factorVelocity;
	m_actualTorque = static_cast<double>(m_txPdo.Torque_current_actual_value.value - m_offsetTorque) / m_factorTorque;

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Target_position.value = static_cast<int32_t>(m_targetPosition * m_factorPosition) + m_offsetPosition;
	m_rxPdo.Target_velocity.value = static_cast<int32_t>(m_targetVelocity * m_factorVelocity) + m_offsetVelocity;
	m_rxPdo.Target_torque.value = static_cast<int16_t>(m_targetTorque * m_factorTorque) + m_offsetTorque;

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::registerPublisher()
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

uint32_t EcSlaveIpos2401MxCat::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.subscribe();
	m_dseCommandVelocity.subscribe();
	m_dseCommandTorque.subscribe();

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.set(m_actualPosition);
	m_dseActualVelocity.set(m_actualVelocity);
	m_dseActualTorque.set(m_actualTorque);

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.get(m_targetPosition);
	m_dseCommandVelocity.get(m_targetVelocity);
	m_dseCommandTorque.get(m_targetTorque);

	return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;
	
	// if(m_path == "/task/ethercat/servo/motor_6_ipos2401")
	// {
	// 	std::cout << "m_targetPosition: " << m_targetPosition * 57.295779513082320876798154814105 << ", m_actualPosition: " << m_actualPosition * 57.295779513082320876798154814105 << std::endl;
	// }

	return dwRes;
}

void EcSlaveIpos2401MxCat::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Status_Word: " << m_txPdo.Statusword.value << ", "
	"Actual_position: " << m_txPdo.Position_actual_value.value << ", "
	"Actual_velocity: " << m_txPdo.Velocity_actual_value.value << ", "
	"Operation_mode_display: " << (uint16_t)m_txPdo.Modes_of_operation_display.value << ", "
	"Torque_actual_value: " << m_txPdo.Torque_current_actual_value.value
	<< std::endl;
}

void EcSlaveIpos2401MxCat::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Control_Word: " << m_rxPdo.Controlword.value << ", "
	"Position_set_point: " << m_rxPdo.Target_position.value << ", "
	"Position_set_point: " << m_rxPdo.Target_velocity.value << ", "
	"Operation_mode: " << (uint16_t)m_rxPdo.Modes_of_operation.value << ", "
	"Target_torque: " << m_rxPdo.Target_torque.value
	<< std::endl;
}

bool EcSlaveIpos2401MxCat::isModePosition()
{
	if(isEnable())
	{
		return m_txPdo.Modes_of_operation_display.value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_POSITION;
	}
	else
	{
		return m_rxPdo.Modes_of_operation.value == EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_POSITION;
	}
}

bool EcSlaveIpos2401MxCat::isModeVelocity()
{
	if(isEnable())
	{
		return m_txPdo.Modes_of_operation_display.value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_VELOCITY;
	}
	else
	{
		return m_rxPdo.Modes_of_operation.value == EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_VELOCITY;
	}
}

bool EcSlaveIpos2401MxCat::isModeTorque()
{
	if(isEnable())
	{
		return m_txPdo.Modes_of_operation_display.value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_TORQUE;
	}
	else
	{
		return m_rxPdo.Modes_of_operation.value == EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_TORQUE;
	}
}

uint32_t EcSlaveIpos2401MxCat::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.set(m_actualPosition);
    
    return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandVelocity.set(m_actualVelocity);

    return dwRes;
}

uint32_t EcSlaveIpos2401MxCat::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandTorque.set(m_actualTorque);

    return dwRes;
}
