#include "EcSlaveSscIoModule.h"

EcSlaveSscIoModule::EcSlaveSscIoModule(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName),
	// m_dseFootpedalMasterClutch("MASTER_CLUTCH", m_taskPath, false, true),
	// m_dseFootpedalCameraClutch("CAMERA_CLUTCH", m_taskPath, false, true),
	// m_dseFootpedalToggleLeft("TOGGLE_LEFT", m_taskPath, false, true),
	// m_dseFootpedalToggleRight("TOGGLE_RIGHT", m_taskPath, false, true),
	// m_dseFootpedalCutLeft("CUT_LEFT", m_taskPath, false, true),
	// m_dseFootpedalCutRight("CUT_RIGHT", m_taskPath, false, true),
	// m_dseFootpedalCoagLeft("COAG_LEFT", m_taskPath, false, true),
	// m_dseFootpedalCoagRight("COAG_RIGHT", m_taskPath, false, true),
	// m_dseButtonEStop("E_STOP", m_taskPath, false, true),
	// m_dseButtonClearFault("CLEAR_FAULT", m_taskPath, false, true),
	// m_dseQualityLeft("QUALITY_left", m_taskPath, 0, true),
	// m_dseQualityRight("QUALITY_RIGHT", m_taskPath, 0, true),
	// m_dseGripValueRight("GRIP_VALUE_RIGHT", m_taskPath, 0, true),
	// m_dseGripValueLeft("GRIP_VALUE_LEFT", m_taskPath, 0, true),
	// m_dsePoseLeft("POSE_LEFT", m_taskPath, {0, 0, 0, 1.0, 0, 0, 0}, true),
	// m_dsePoseRight("POSE_RIGHT", m_taskPath, {0, 0, 0, 1.0, 0, 0, 0}, true),
	// m_dseHeadTracking("HEAD_TRACKING", m_taskPath, false, true),
	// m_dseFingerClutchLeft("FINGER_CLUTCH_LEFT", m_taskPath, false, true),
	// m_dseFingerClutchRight("FINGER_CLUTCH_RIGHT", m_taskPath, false, true),
	// m_dsePalmSensorLeft("PALM_SENSOR_LEFT", m_taskPath, false, true),
	// m_dsePalmSensorRight("PALM_SENSOR_RIGHT", m_taskPath, false, true),
	m_footpedalMasterClutch(false),
	m_footpedalCameraClutch(false),
	m_footpedalToggleLeft(false),
	m_footpedalToggleRight(false),
	m_footpedalCutLeft(false),
	m_footpedalCutRight(false),
	m_footpedalCoagLeft(false),
	m_footpedalCoagRight(false),
	m_buttonEStop(false),
	m_buttonClearFault(false),
	m_qualityLeft(255),
	m_qualityRight(255),
	m_gripValueRight(0),
	m_gripValueLeft(0),
	m_poseLeft({0, 0, 0, 1.0, 0, 0, 0}),
	m_poseRight({0, 0, 0, 1.0, 0, 0, 0}),
	m_headTracking(false),
	m_fingerClutchLeft(false),
	m_fingerClutchRight(false),
	m_palmSensorLeft(false),
	m_palmSensorRight(false)
{
}

uint32_t EcSlaveSscIoModule::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	registerXmlElementDataStoreValue("dseFootpedalMasterClutch", m_dseFootpedalMasterClutch);
	registerXmlElementDataStoreValue("dseFootpedalCameraClutch", m_dseFootpedalCameraClutch);
	registerXmlElementDataStoreValue("dseFootpedalToggleLeft", m_dseFootpedalToggleLeft);
	registerXmlElementDataStoreValue("dseFootpedalToggleRight", m_dseFootpedalToggleRight);
	registerXmlElementDataStoreValue("dseFootpedalCutLeft", m_dseFootpedalCutLeft);
	registerXmlElementDataStoreValue("dseFootpedalCutRight", m_dseFootpedalCutRight);
	registerXmlElementDataStoreValue("dseFootpedalCoagLeft", m_dseFootpedalCoagLeft);
	registerXmlElementDataStoreValue("dseFootpedalCoagRight", m_dseFootpedalCoagRight);
	registerXmlElementDataStoreValue("dseButtonEStop", m_dseButtonEStop);
	registerXmlElementDataStoreValue("dseButtonClearFault", m_dseButtonClearFault);
	registerXmlElementDataStoreValue("dseQualityLeft", m_dseQualityLeft);
	registerXmlElementDataStoreValue("dseQualityRight", m_dseQualityRight);
	registerXmlElementDataStoreValue("dseGripValueRight", m_dseGripValueRight);
	registerXmlElementDataStoreValue("dseGripValueLeft", m_dseGripValueLeft);
	registerXmlElementDataStoreVector("dsePoseLeft", m_dsePoseLeft);
	registerXmlElementDataStoreVector("dsePoseRight", m_dsePoseRight);
	registerXmlElementDataStoreValue("dseHeadTracking", m_dseHeadTracking);
	registerXmlElementDataStoreValue("dseFingerClutchLeft", m_dseFingerClutchLeft);
	registerXmlElementDataStoreValue("dseFingerClutchRight", m_dseFingerClutchRight);
	registerXmlElementDataStoreValue("dsePalmSensorLeft", m_dsePalmSensorLeft);
	registerXmlElementDataStoreValue("dsePalmSensorRight", m_dsePalmSensorRight);
    
    return dwRes;
}

EcSlaveSscIoModule::~EcSlaveSscIoModule()
{
}

uint32_t EcSlaveSscIoModule::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.footpedal_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.footpedal_1);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.QUALITY_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.QUALITY_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.GRIP_VAL_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.GRIP_VAL_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.X_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Y_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Z_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q1_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q2_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q3_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q4_LEFT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.X_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Y_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Z_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q1_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q2_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q3_RIGHT);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Q4_RIGHT);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.IPC_TX_1);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.IPC_RX_1);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.footpedal_1, pBuffer);
	transferInputPdoObject(m_txPdo.footpedal_1, pBuffer);
	transferInputPdoObject(m_txPdo.QUALITY_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.QUALITY_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.GRIP_VAL_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.GRIP_VAL_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.X_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Y_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Z_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q1_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q2_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q3_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.Q4_LEFT, pBuffer);
	transferInputPdoObject(m_txPdo.X_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Y_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Z_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q1_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q2_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q3_RIGHT, pBuffer);
	transferInputPdoObject(m_txPdo.Q4_RIGHT, pBuffer);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.IPC_TX_1, pBuffer);
	transferOutputPdoObject(m_rxPdo.IPC_RX_1, pBuffer);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_footpedalMasterClutch =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_7) == EcSlaveSscIoModuleData::Bit::BIT_7);
	m_footpedalCameraClutch =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_6) == EcSlaveSscIoModuleData::Bit::BIT_6);
	m_footpedalToggleLeft   =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_5) == EcSlaveSscIoModuleData::Bit::BIT_5);
	m_footpedalToggleRight  =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_4) == EcSlaveSscIoModuleData::Bit::BIT_4);
	m_footpedalCutLeft      =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_1) == EcSlaveSscIoModuleData::Bit::BIT_1);
	m_footpedalCutRight     =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_0) == EcSlaveSscIoModuleData::Bit::BIT_0);
	m_footpedalCoagLeft     =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_3) == EcSlaveSscIoModuleData::Bit::BIT_3);
	m_footpedalCoagRight    =  ((m_txPdo.footpedal_1.value & EcSlaveSscIoModuleData::Bit::BIT_2) == EcSlaveSscIoModuleData::Bit::BIT_2);
	m_buttonEStop           =  ((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_3) == EcSlaveSscIoModuleData::Bit::BIT_3);
	m_buttonClearFault      =  ((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_4) == EcSlaveSscIoModuleData::Bit::BIT_4);
	m_headTracking          = !((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_2) == EcSlaveSscIoModuleData::Bit::BIT_2);
	m_fingerClutchLeft      = !((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_0) == EcSlaveSscIoModuleData::Bit::BIT_0);
	m_fingerClutchRight     = !((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_1) == EcSlaveSscIoModuleData::Bit::BIT_1);
	m_palmSensorLeft        = !((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_5) == EcSlaveSscIoModuleData::Bit::BIT_5);
	m_palmSensorRight       = !((m_txPdo.footpedal_2.value & EcSlaveSscIoModuleData::Bit::BIT_6) == EcSlaveSscIoModuleData::Bit::BIT_6);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveSscIoModule::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseFootpedalMasterClutch.publish();
	m_dseFootpedalCameraClutch.publish();
	m_dseFootpedalToggleLeft.publish();
	m_dseFootpedalToggleRight.publish();
	m_dseFootpedalCutLeft.publish();
	m_dseFootpedalCutRight.publish();
	m_dseFootpedalCoagLeft.publish();
	m_dseFootpedalCoagRight.publish();
	m_dseButtonEStop.publish();
	m_dseButtonClearFault.publish();
	m_dseQualityLeft.publish();
	m_dseQualityRight.publish();
	m_dseGripValueRight.publish();
	m_dseGripValueLeft.publish();
	m_dsePoseLeft.publish();
	m_dsePoseRight.publish();
	m_dseHeadTracking.publish();
	m_dseFingerClutchLeft.publish();
	m_dseFingerClutchRight.publish();
	m_dsePalmSensorLeft.publish();
	m_dsePalmSensorRight.publish();

	return dwRes;
}

uint32_t EcSlaveSscIoModule::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveSscIoModule::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseFootpedalMasterClutch.set(m_footpedalMasterClutch);
	m_dseFootpedalCameraClutch.set(m_footpedalCameraClutch);
	m_dseFootpedalToggleLeft.set(m_footpedalToggleLeft);
	m_dseFootpedalToggleRight.set(m_footpedalToggleRight);
	m_dseFootpedalCutLeft.set(m_footpedalCutLeft);
	m_dseFootpedalCutRight.set(m_footpedalCutRight);
	m_dseFootpedalCoagLeft.set(m_footpedalCoagLeft);
	m_dseFootpedalCoagRight.set(m_footpedalCoagRight);
	m_dseButtonEStop.set(m_buttonEStop);
	m_dseButtonClearFault.set(m_buttonClearFault);
	m_dseQualityLeft.set(m_txPdo.QUALITY_LEFT.value);
	m_dseQualityRight.set(m_txPdo.QUALITY_RIGHT.value);
	m_dseGripValueRight.set(m_txPdo.GRIP_VAL_RIGHT.value);
	m_dseGripValueLeft.set(m_txPdo.GRIP_VAL_LEFT.value);
	m_dsePoseLeft.set({m_txPdo.X_LEFT.value, m_txPdo.Y_LEFT.value, m_txPdo.Z_LEFT.value, m_txPdo.Q1_LEFT.value, m_txPdo.Q2_LEFT.value, m_txPdo.Q3_LEFT.value, m_txPdo.Q4_LEFT.value});
	m_dsePoseRight.set({m_txPdo.X_RIGHT.value, m_txPdo.Y_RIGHT.value, m_txPdo.Z_RIGHT.value, m_txPdo.Q1_RIGHT.value, m_txPdo.Q2_RIGHT.value, m_txPdo.Q3_RIGHT.value, m_txPdo.Q4_RIGHT.value});
	m_dseHeadTracking.set(m_headTracking);
	m_dseFingerClutchLeft.set(m_fingerClutchLeft);
	m_dseFingerClutchRight.set(m_fingerClutchRight);
	m_dsePalmSensorLeft.set(m_palmSensorLeft);
	m_dsePalmSensorRight.set(m_palmSensorRight);

	return dwRes;
}

uint32_t EcSlaveSscIoModule::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveSscIoModule::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

void EcSlaveSscIoModule::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Device_ID: " << m_txPdo.footpedal_1.value << ", "
	"Second: " << m_txPdo.footpedal_1.value << ", "
	"Minute: " << m_txPdo.QUALITY_LEFT.value << ", "
	"Hour: " << m_txPdo.QUALITY_RIGHT.value << ", "
	"Day: " << m_txPdo.GRIP_VAL_RIGHT.value << ", "
	"Month: " << m_txPdo.GRIP_VAL_LEFT.value << ", "
	"Year: " << m_txPdo.X_LEFT.value << ", "
	"Roll_Offset: " << m_txPdo.Y_LEFT.value << ", "
	"Pitch_Offset: " << m_txPdo.Z_LEFT.value << ", "
	"Yaw_Offset: " << m_txPdo.Q1_LEFT.value << ", "
	"Grip_Offset: " << m_txPdo.Q2_LEFT.value << ", "
	"No_of_Usages: " << m_txPdo.Q3_LEFT.value << ", "
	"Max_Usages: " << m_txPdo.Q4_LEFT.value << ", "
	"Digital_Inputs: " << m_txPdo.X_RIGHT.value << ", "
	"Grip_Counts: " << m_txPdo.Y_RIGHT.value << ", "
	"System_Number: " << m_txPdo.Z_RIGHT.value << ", "
	"Device_UID: " << m_txPdo.Q1_RIGHT.value << ", "
	"Spare_Bytes: " << m_txPdo.Q2_RIGHT.value << ", "
	"MFG_Day: " << m_txPdo.Q3_RIGHT.value << ", "
	"MFG_Year: " << m_txPdo.Q4_RIGHT.value
	<< std::endl;
}

void EcSlaveSscIoModule::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_rxPdo.IPC_TX_1.value << ", "
	"Grip_Offset: " << m_rxPdo.IPC_RX_1.value
	<< std::endl;
}
