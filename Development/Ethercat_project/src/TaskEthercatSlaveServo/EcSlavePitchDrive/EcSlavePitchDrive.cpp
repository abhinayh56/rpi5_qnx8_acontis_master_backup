#include "EcSlavePitchDrive.h"

EcSlavePitchDrive::EcSlavePitchDrive(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcCia402(taskId, taskName, taskPath, slaveAddr, slaveName)
	// m_dseActualPosition("ACTUAL_POSITION", m_taskPath, 0, true),
	// m_dseActualVelocity("ACTUAL_VELOCITY", m_taskPath, 0, true),
	// m_dseActualTorque("ACTUAL_TORQUE", m_taskPath, 0, true),
	// m_dseCommandPosition("COMMAND_POSITION", m_taskPath, 0, true),
	// m_dseCommandVelocity("COMMAND_VELOCITY", m_taskPath, 0, true),
	// m_dseCommandTorque("COMMAND_TORQUE", m_taskPath, 0, true)
{
}

EcSlavePitchDrive::~EcSlavePitchDrive()
{
}

uint32_t EcSlavePitchDrive::registerXml()
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

uint32_t EcSlavePitchDrive::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_POS);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.STATUS_WD);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_TOR);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.OPMODE_DISP);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ERROR_CODE);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.DIG_IN);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ACT_VEL);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.ADC_VAL);

	m_Cia402PdoTx.statusWord.p_isSupported = &m_txPdo.STATUS_WD.isSupported;
	m_Cia402PdoTx.modeOfOperationDisplay.p_isSupported = &m_txPdo.OPMODE_DISP.isSupported;
	m_Cia402PdoTx.actualPosition.p_isSupported = &m_txPdo.ACT_POS.isSupported;
	m_Cia402PdoTx.actualVelocity.p_isSupported = &m_txPdo.ACT_VEL.isSupported;
	m_Cia402PdoTx.actualTorque.p_isSupported = &m_txPdo.ACT_TOR.isSupported;

	m_Cia402PdoTx.statusWord.p_value = &m_txPdo.STATUS_WD.value;
	m_Cia402PdoTx.modeOfOperationDisplay.p_value = &m_txPdo.OPMODE_DISP.value;
	m_Cia402PdoTx.actualPosition.p_value = &m_txPdo.ACT_POS.value;
	m_Cia402PdoTx.actualVelocity.p_value = &m_txPdo.ACT_VEL.value;
	m_Cia402PdoTx.actualTorque.p_value = &m_txPdo.ACT_TOR.value;

	return dwRes;
}

uint32_t EcSlavePitchDrive::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_POSE);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.CONTROL_WD);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_TORQ);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.OP_MODE);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.ERROR_CLEAR);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.DIG_OUT);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.TARGET_VEL);

	m_Cia402PdoRx.controlWord.p_isSupported = &m_rxPdo.CONTROL_WD.isSupported;
	m_Cia402PdoRx.modeOfOperation.p_isSupported = &m_rxPdo.OP_MODE.isSupported;
	m_Cia402PdoRx.targetPosition.p_isSupported = &m_rxPdo.TARGET_POSE.isSupported;
	m_Cia402PdoRx.targetVelocity.p_isSupported = &m_rxPdo.TARGET_VEL.isSupported;
	m_Cia402PdoRx.targetTorque.p_isSupported = &m_rxPdo.TARGET_TORQ.isSupported;

	m_Cia402PdoRx.controlWord.p_value = &m_rxPdo.CONTROL_WD.value;
	m_Cia402PdoRx.modeOfOperation.p_value = &m_rxPdo.OP_MODE.value;
	m_Cia402PdoRx.targetPosition.p_value = &m_rxPdo.TARGET_POSE.value;
	m_Cia402PdoRx.targetVelocity.p_value = &m_rxPdo.TARGET_VEL.value;
	m_Cia402PdoRx.targetTorque.p_value = &m_rxPdo.TARGET_TORQ.value;

	return dwRes;
}

uint32_t EcSlavePitchDrive::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.ACT_POS, pBuffer);
	transferInputPdoObject(m_txPdo.STATUS_WD, pBuffer);
	transferInputPdoObject(m_txPdo.ACT_TOR, pBuffer);
	transferInputPdoObject(m_txPdo.OPMODE_DISP, pBuffer);
	transferInputPdoObject(m_txPdo.ERROR_CODE, pBuffer);
	transferInputPdoObject(m_txPdo.DIG_IN, pBuffer);
	transferInputPdoObject(m_txPdo.ACT_VEL, pBuffer);
	transferInputPdoObject(m_txPdo.ADC_VAL, pBuffer);

	return dwRes;
}

uint32_t EcSlavePitchDrive::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.TARGET_POSE, pBuffer);
	transferOutputPdoObject(m_rxPdo.CONTROL_WD, pBuffer);
	transferOutputPdoObject(m_rxPdo.TARGET_TORQ, pBuffer);
	transferOutputPdoObject(m_rxPdo.OP_MODE, pBuffer);
	transferOutputPdoObject(m_rxPdo.ERROR_CLEAR, pBuffer);
	transferOutputPdoObject(m_rxPdo.DIG_OUT, pBuffer);
	transferOutputPdoObject(m_rxPdo.TARGET_VEL, pBuffer);

	return dwRes;
}

uint32_t EcSlavePitchDrive::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_actualPosition = static_cast<double>(m_txPdo.ACT_POS.value - m_offsetPosition) / m_factorPosition;
	m_actualVelocity = static_cast<double>(m_txPdo.ACT_VEL.value - m_offsetVelocity) / m_factorVelocity;
	m_actualTorque = static_cast<double>(m_txPdo.ACT_TOR.value - m_offsetTorque) / m_factorTorque;

	return dwRes;
}

uint32_t EcSlavePitchDrive::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.TARGET_POSE.value = static_cast<int32_t>(m_targetPosition * m_factorPosition) + m_offsetPosition;
	m_rxPdo.TARGET_VEL.value = static_cast<int32_t>(m_targetVelocity * m_factorVelocity) + m_offsetVelocity;
	m_rxPdo.TARGET_TORQ.value = static_cast<int16_t>(m_targetTorque * m_factorTorque) + m_offsetTorque;

	return dwRes;
}

uint32_t EcSlavePitchDrive::registerPublisher()
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

uint32_t EcSlavePitchDrive::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.subscribe();
	m_dseCommandVelocity.subscribe();
	m_dseCommandTorque.subscribe();

	return dwRes;
}

uint32_t EcSlavePitchDrive::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseActualPosition.set(m_actualPosition);
	m_dseActualVelocity.set(m_actualVelocity);
	m_dseActualTorque.set(m_actualTorque);

	return dwRes;
}

uint32_t EcSlavePitchDrive::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.get(m_targetPosition);
	m_dseCommandVelocity.get(m_targetVelocity);
	m_dseCommandTorque.get(m_targetTorque);

	return dwRes;
}

uint32_t EcSlavePitchDrive::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	// if(m_path == "/task/ethercat/servo/motor_1_mact")
	// {
	// 	std::cout << "m_targetPosition: " << m_targetPosition * 57.295779513082320876798154814105 << ", m_actualPosition: " << m_actualPosition * 57.295779513082320876798154814105 << std::endl;
	// }

	return dwRes;
}

void EcSlavePitchDrive::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"ACT_POS: " << m_txPdo.ACT_POS.value << ", "
	"STATUS_WD: " << m_txPdo.STATUS_WD.value << ", "
	"ACT_TOR: " << m_txPdo.ACT_TOR.value << ", "
	"OPMODE_DISP: " << (uint16_t)m_txPdo.OPMODE_DISP.value << ", "
	"ERROR_CODE: " << m_txPdo.ERROR_CODE.value << ", "
	"DIG_IN: " << (uint16_t)m_txPdo.DIG_IN.value << ", "
	"ACT_VEL: " << m_txPdo.ACT_VEL.value << ", "
	"ADC_VAL: " << m_txPdo.ADC_VAL.value
	<< std::endl;
}

void EcSlavePitchDrive::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"TARGET_POSE: " << m_rxPdo.TARGET_POSE.value << ", "
	"CONTROL_WD: " << m_rxPdo.CONTROL_WD.value << ", "
	"TARGET_TORQ: " << m_rxPdo.TARGET_TORQ.value << ", "
	"OP_MODE: " << (uint16_t)m_rxPdo.OP_MODE.value << ", "
	"ERROR_CLEAR: " << m_rxPdo.ERROR_CLEAR.value << ", "
	"DIG_OUT: " << (uint16_t)m_rxPdo.DIG_OUT.value << ", "
	"TARGET_VEL: " << m_rxPdo.TARGET_VEL.value
	<< std::endl;
}

uint32_t EcSlavePitchDrive::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandPosition.set(m_actualPosition);
    
    return dwRes;
}

uint32_t EcSlavePitchDrive::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandVelocity.set(m_actualVelocity);

    return dwRes;
}

uint32_t EcSlavePitchDrive::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCommandTorque.set(m_actualTorque);

    return dwRes;
}
