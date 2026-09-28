#include "EcSlaveDenXcrE.h"

EcSlaveDenXcrE::EcSlaveDenXcrE(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseActualPosition("ACTUAL_POSITION", m_taskPath, 0, true),
	// m_dseActualVelocity("ACTUAL_VELOCITY", m_taskPath, 0, true),
	// m_dseActualTorque("ACTUAL_TORQUE", m_taskPath, 0, true),
	// m_dseCommandPosition("COMMAND_POSITION", m_taskPath, 0, true),
	// m_dseCommandVelocity("COMMAND_VELOCITY", m_taskPath, 0, true),
	// m_dseCommandTorque("COMMAND_TORQUE", m_taskPath, 0, true)
{
}

EcSlaveDenXcrE::~EcSlaveDenXcrE()
{
}

uint32_t EcSlaveDenXcrE::registerXml()
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

uint32_t EcSlaveDenXcrE::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Status_Word);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_position);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Actual_velocity);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Operation_mode_display);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Torque_actual_value);

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

uint32_t EcSlaveDenXcrE::registerRxPdo()
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

uint32_t EcSlaveDenXcrE::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Status_Word, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_position, pBuffer);
	transferInputPdoObject(m_txPdo.Actual_velocity, pBuffer);
	transferInputPdoObject(m_txPdo.Operation_mode_display, pBuffer);
	transferInputPdoObject(m_txPdo.Torque_actual_value, pBuffer);

	return dwRes;
}

uint32_t EcSlaveDenXcrE::transferRxPdo()
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

uint32_t EcSlaveDenXcrE::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_actualPosition = static_cast<double>(m_txPdo.Actual_position.value - m_offsetPosition) / m_factorPosition;
	m_actualVelocity = static_cast<double>(m_txPdo.Actual_velocity.value - m_offsetVelocity) / m_factorVelocity;
	m_actualTorque = static_cast<double>(m_txPdo.Torque_actual_value.value - m_offsetTorque) / m_factorTorque;

	return dwRes;
}

uint32_t EcSlaveDenXcrE::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Position_set_point.value = static_cast<int32_t>(m_targetPosition * m_factorPosition) + m_offsetPosition;
	m_rxPdo.Velocity_set_point.value = static_cast<int32_t>(m_targetVelocity * m_factorVelocity) + m_offsetVelocity;
	m_rxPdo.Target_torque.value = static_cast<int16_t>(m_targetTorque * m_factorTorque) + m_offsetTorque;

	return dwRes;
}

uint32_t EcSlaveDenXcrE::registerPublisher()
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

uint32_t EcSlaveDenXcrE::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.subscribe();
	m_dseCommandVelocity.subscribe();
	m_dseCommandTorque.subscribe();

	return dwRes;
}

uint32_t EcSlaveDenXcrE::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.set(m_actualPosition);
	m_dseActualVelocity.set(m_actualVelocity);
	m_dseActualTorque.set(m_actualTorque);

	return dwRes;
}

uint32_t EcSlaveDenXcrE::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.get(m_targetPosition);
	m_dseCommandVelocity.get(m_targetVelocity);
	m_dseCommandTorque.get(m_targetTorque);

	return dwRes;
}

uint32_t EcSlaveDenXcrE::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	// if(m_slaveAddr == 1020)
	// {
	// 	std::cout << double(int(m_actualPosition * 57.295779513082320876798154814105 * 100)) * 0.01  << ", ";
	// }
	// if(m_slaveAddr == 1021)
	// {
	// 	std::cout << double(int(m_actualPosition * 57.295779513082320876798154814105 * 100)) * 0.01  << ", ";
	// }
	// if(m_slaveAddr == 1022)
	// {
	// 	std::cout << double(int(m_actualPosition * 57.295779513082320876798154814105 * 100)) * 0.01  << std::endl;
	// }

	return dwRes;
}

void EcSlaveDenXcrE::dispTxPdo()
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

void EcSlaveDenXcrE::dispRxPdo()
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

uint32_t EcSlaveDenXcrE::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.set(m_actualPosition);

    return dwRes;
}

uint32_t EcSlaveDenXcrE::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandVelocity.set(m_actualVelocity);

    return dwRes;
}

uint32_t EcSlaveDenXcrE::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandTorque.set(m_actualTorque);

    return dwRes;
}
