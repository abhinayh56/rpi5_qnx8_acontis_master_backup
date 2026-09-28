#include "EcSlaveRfidslave.h"

EcSlaveRfidslave::EcSlaveRfidslave(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
	EcSlaveBase(taskId, taskName, taskPath, slaveAddr, slaveName),

	// m_dseCurrentDeviceID("CURRENT_DEVICE_ID", m_taskPath, 0, false),
	// m_dseCurrentTimeSecond("CURRENT_TIME_SECOND", m_taskPath, 0, false),
	// m_dseCurrentTimeMinute("CURRENT_TIME_MINUTE", m_taskPath, 0, false),
	// m_dseCurrentTimeHour("CURRENT_TIME_HOUR", m_taskPath, 0, false),
	// m_dseCurrentTimeDay("CURRENT_TIME_DAY", m_taskPath, 0, false),
	// m_dseCurrentTimeMonth("CURRENT_TIME_MONTH", m_taskPath, 0, false),
	// m_dseCurrentTimeYear("CURRENT_TIME_YEAR", m_taskPath, 0, false),
	// m_dseCurrentOffsetRoll("CURRENT_OFFSET_ROLL", m_taskPath, 0, false),
	// m_dseCurrentOffsetPitch("CURRENT_OFFSET_PITCH", m_taskPath, 0, false),
	// m_dseCurrentOffsetYaw("CURRENT_OFFSET_YAW", m_taskPath, 0, false),
	// m_dseCurrentOffsetGrip("CURRENT_OFFSET_GRIP", m_taskPath, 0, false),
	// m_dseCurrentNoOfUsages("CURRENT_NO_OF_USAGES", m_taskPath, 0, false),
	// m_dseCurrentMaxUsages("CURRENT_MAX_USAGES", m_taskPath, 0, false),
	// m_dseCurrentSterileBarrierStatus("CURRENT_STERILE_BARRIER_MOUNTED", m_taskPath, false, false),
	// m_dseCurrentInstrumentStatus("CURRENT_INSTRUMENT_MOUNTED", m_taskPath, false, false),
	// m_dseCurrentGripCounts("CURRENT_GRIP_COUNTS", m_taskPath, 0, false),
	// m_dseCurrentSystemNumber("CURRENT_SYSTEM_NUMBER", m_taskPath, 0, false),
	// m_dseCurrentDeviceUid("CURRENT_DEVICE_UID", m_taskPath, 0, false),
	// m_dseCurrentSpareBytes("CURRENT_SPARE_BYTE", m_taskPath, 0, false),
	// m_dseCurrentMfgDay("CURRENT_MFG_DAY", m_taskPath, 0, false),
	// m_dseCurrentMfgMonth("CURRENT_MFG_MONTH", m_taskPath, 0, false),
	// m_dseCurrentMfgYear("CURRENT_MFG_YEAR", m_taskPath, 0, false),

	// m_dseNewAcknowledge("NEW_ACKNOWLEDGE", m_taskPath, 0, true),
	// m_dseNewTimeSecond("NEW_TIME_SECOND", m_taskPath, 0, true),
	// m_dseNewTimeMinute("NEW_TIME_MINUTE", m_taskPath, 0, true),
	// m_dseNewTimeHour("NEW_TIME_HOUR", m_taskPath, 0, true),
	// m_dseNewTimeDay("NEW_TIME_DAY", m_taskPath, 0, true),
	// m_dseNewTimeMonth("NEW_TIME_MONTH", m_taskPath, 0, true),
	// m_dseNewTimeYear("NEW_TIME_YEAR", m_taskPath, 0, true),
	// m_dseNewNoOfUsages("NEW_NO_OF_USAGES", m_taskPath, 0, true),
	// m_dseNewDigitalOutputs("NEW_DIGITAL_OUTPUT", m_taskPath, 0, true),
	// m_dseNewGripCounts("NEW_GRIP_COUNTS", m_taskPath, 0, true),
	// m_dseNewSystemNumber("NEW_SYSTEM_NUMBER", m_taskPath, 0, true),
	// m_dseNewLedRed("NEW_LED_RED", m_taskPath, 0, true),
	// m_dseNewLedGreen("NEW_LED_GREEN", m_taskPath, 0, true),
	// m_dseNewLEDBlue("NEW_LED_BLUE", m_taskPath, 0, true),
	// m_dseNewSpareBytes("NEW_SPARE_BYTE", m_taskPath, 0, true),
	// m_dseNewOffsetRoll("NEW_OFFSET_ROLL", m_taskPath, 0, true),
	// m_dseNewOffsetPitch("NEW_OFFSET_PITCH", m_taskPath, 0, true),
	// m_dseNewOffsetYaw("NEW_OFFSET_YAW", m_taskPath, 0, true),
	// m_dseNewOffsetGrip("NEW_OFFSET_GRIP", m_taskPath, 0, true),

	m_newAcknowledge(0),
	m_newTimeSecond(0),
	m_newTimeMinute(0),
	m_newTimeHour(0),
	m_newTimeDay(0),
	m_newTimeMonth(0),
	m_newTimeYear(0),
	m_newNoOfUsages(0),
	m_newDigitalOutputs(0),
	m_newGripCounts(0),
	m_newSystemNumber(0),
	m_newLedRed(false),
	m_newLedGreen(false),
	m_newLEDBlue(false),
	m_newSpareBytes(0),
	m_newOffsetRoll(0),
	m_newOffsetPitch(0),
	m_newOffsetYaw(0),
	m_newOffsetGrip(0)
{
}

EcSlaveRfidslave::~EcSlaveRfidslave()
{
}

uint32_t EcSlaveRfidslave::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

	registerXmlElementDataStoreValue("dseCurrentDeviceID", m_dseCurrentDeviceID);
	registerXmlElementDataStoreValue("dseCurrentTimeSecond", m_dseCurrentTimeSecond);
	registerXmlElementDataStoreValue("dseCurrentTimeMinute", m_dseCurrentTimeMinute);
	registerXmlElementDataStoreValue("dseCurrentTimeHour", m_dseCurrentTimeHour);
	registerXmlElementDataStoreValue("dseCurrentTimeDay", m_dseCurrentTimeDay);
	registerXmlElementDataStoreValue("dseCurrentTimeMonth", m_dseCurrentTimeMonth);
	registerXmlElementDataStoreValue("dseCurrentTimeYear", m_dseCurrentTimeYear);
	registerXmlElementDataStoreValue("dseCurrentOffsetRoll", m_dseCurrentOffsetRoll);
	registerXmlElementDataStoreValue("dseCurrentOffsetPitch", m_dseCurrentOffsetPitch);
	registerXmlElementDataStoreValue("dseCurrentOffsetYaw", m_dseCurrentOffsetYaw);
	registerXmlElementDataStoreValue("dseCurrentOffsetGrip", m_dseCurrentOffsetGrip);
	registerXmlElementDataStoreValue("dseCurrentNoOfUsages", m_dseCurrentNoOfUsages);
	registerXmlElementDataStoreValue("dseCurrentMaxUsages", m_dseCurrentMaxUsages);
	registerXmlElementDataStoreValue("dseCurrentSterileBarrierStatus", m_dseCurrentSterileBarrierStatus);
	registerXmlElementDataStoreValue("dseCurrentInstrumentStatus", m_dseCurrentInstrumentStatus);
	registerXmlElementDataStoreValue("dseCurrentGripCounts", m_dseCurrentGripCounts);
	registerXmlElementDataStoreValue("dseCurrentSystemNumber", m_dseCurrentSystemNumber);
	registerXmlElementDataStoreValue("dseCurrentDeviceUid", m_dseCurrentDeviceUid);
	registerXmlElementDataStoreValue("dseCurrentSpareBytes", m_dseCurrentSpareBytes);
	registerXmlElementDataStoreValue("dseCurrentMfgDay", m_dseCurrentMfgDay);
	registerXmlElementDataStoreValue("dseCurrentMfgMonth", m_dseCurrentMfgMonth);
	registerXmlElementDataStoreValue("dseCurrentMfgYear", m_dseCurrentMfgYear);
	registerXmlElementDataStoreValue("dseNewAcknowledge", m_dseNewAcknowledge);
	registerXmlElementDataStoreValue("dseNewTimeSecond", m_dseNewTimeSecond);
	registerXmlElementDataStoreValue("dseNewTimeMinute", m_dseNewTimeMinute);
	registerXmlElementDataStoreValue("dseNewTimeHour", m_dseNewTimeHour);
	registerXmlElementDataStoreValue("dseNewTimeDay", m_dseNewTimeDay);
	registerXmlElementDataStoreValue("dseNewTimeMonth", m_dseNewTimeMonth);
	registerXmlElementDataStoreValue("dseNewTimeYear", m_dseNewTimeYear);
	registerXmlElementDataStoreValue("dseNewNoOfUsages", m_dseNewNoOfUsages);
	registerXmlElementDataStoreValue("dseNewDigitalOutputs", m_dseNewDigitalOutputs);
	registerXmlElementDataStoreValue("dseNewGripCounts", m_dseNewGripCounts);
	registerXmlElementDataStoreValue("dseNewSystemNumber", m_dseNewSystemNumber);
	registerXmlElementDataStoreValue("dseNewLedRed", m_dseNewLedRed);
	registerXmlElementDataStoreValue("dseNewLedGreen", m_dseNewLedGreen);
	registerXmlElementDataStoreValue("dseNewLEDBlue", m_dseNewLEDBlue);
	registerXmlElementDataStoreValue("dseNewSpareBytes", m_dseNewSpareBytes);
	registerXmlElementDataStoreValue("dseNewOffsetRoll", m_dseNewOffsetRoll);
	registerXmlElementDataStoreValue("dseNewOffsetPitch", m_dseNewOffsetPitch);
	registerXmlElementDataStoreValue("dseNewOffsetYaw", m_dseNewOffsetYaw);
	registerXmlElementDataStoreValue("dseNewOffsetGrip", m_dseNewOffsetGrip);
    
    return dwRes;
}

uint32_t EcSlaveRfidslave::registerTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Device_ID);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Second);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Minute);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Hour);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Day);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Month);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Year);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Roll_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Pitch_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Yaw_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Grip_Offset);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.No_of_Usages);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Max_Usages);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Digital_Inputs);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Grip_Counts);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.System_Number);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Device_UID);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.Spare_Bytes);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Day);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Month);
	dwRes |= lookupInputPdoObject(m_slaveAddr, m_txPdo.MFG_Year);

	return dwRes;
}

uint32_t EcSlaveRfidslave::registerRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Acknowledge);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Second);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Minute);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Hour);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Day);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Month);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Year);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.No_of_Usages);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Digital_Outputs);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Grip_Counts);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.System_Number);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Led_Red);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Led_Green);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.LED_Blue);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Spare_Bytes);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Roll_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Pitch_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Yaw_Offset);
	dwRes |= lookupOutputPdoObject(m_slaveAddr, m_rxPdo.Grip_Offset);

	return dwRes;
}

uint32_t EcSlaveRfidslave::transferTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageInputPtr();

	transferInputPdoObject(m_txPdo.Device_ID, pBuffer);
	transferInputPdoObject(m_txPdo.Second, pBuffer);
	transferInputPdoObject(m_txPdo.Minute, pBuffer);
	transferInputPdoObject(m_txPdo.Hour, pBuffer);
	transferInputPdoObject(m_txPdo.Day, pBuffer);
	transferInputPdoObject(m_txPdo.Month, pBuffer);
	transferInputPdoObject(m_txPdo.Year, pBuffer);
	transferInputPdoObject(m_txPdo.Roll_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Pitch_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Yaw_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.Grip_Offset, pBuffer);
	transferInputPdoObject(m_txPdo.No_of_Usages, pBuffer);
	transferInputPdoObject(m_txPdo.Max_Usages, pBuffer);
	transferInputPdoObject(m_txPdo.Digital_Inputs, pBuffer);
	transferInputPdoObject(m_txPdo.Grip_Counts, pBuffer);
	transferInputPdoObject(m_txPdo.System_Number, pBuffer);
	transferInputPdoObject(m_txPdo.Device_UID, pBuffer);
	transferInputPdoObject(m_txPdo.Spare_Bytes, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Day, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Month, pBuffer);
	transferInputPdoObject(m_txPdo.MFG_Year, pBuffer);

	return dwRes;
}

uint32_t EcSlaveRfidslave::transferRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	EC_T_BYTE* pBuffer = ecatGetProcessImageOutputPtr();

	transferOutputPdoObject(m_rxPdo.Acknowledge, pBuffer);
	transferOutputPdoObject(m_rxPdo.Second, pBuffer);
	transferOutputPdoObject(m_rxPdo.Minute, pBuffer);
	transferOutputPdoObject(m_rxPdo.Hour, pBuffer);
	transferOutputPdoObject(m_rxPdo.Day, pBuffer);
	transferOutputPdoObject(m_rxPdo.Month, pBuffer);
	transferOutputPdoObject(m_rxPdo.Year, pBuffer);
	transferOutputPdoObject(m_rxPdo.No_of_Usages, pBuffer);
	transferOutputPdoObject(m_rxPdo.Digital_Outputs, pBuffer);
	transferOutputPdoObject(m_rxPdo.Grip_Counts, pBuffer);
	transferOutputPdoObject(m_rxPdo.System_Number, pBuffer);
	transferOutputPdoObject(m_rxPdo.Led_Red, pBuffer);
	transferOutputPdoObject(m_rxPdo.Led_Green, pBuffer);
	transferOutputPdoObject(m_rxPdo.LED_Blue, pBuffer);
	transferOutputPdoObject(m_rxPdo.Spare_Bytes, pBuffer);
	transferOutputPdoObject(m_rxPdo.Roll_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Pitch_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Yaw_Offset, pBuffer);
	transferOutputPdoObject(m_rxPdo.Grip_Offset, pBuffer);

	return dwRes;
}

uint32_t EcSlaveRfidslave::processTxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

uint32_t EcSlaveRfidslave::processRxPdo()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_rxPdo.Acknowledge.value     = m_newAcknowledge;
	m_rxPdo.Second.value          = m_newTimeSecond;
	m_rxPdo.Minute.value          = m_newTimeMinute;
	m_rxPdo.Hour.value            = m_newTimeHour;
	m_rxPdo.Day.value             = m_newTimeDay;
	m_rxPdo.Month.value           = m_newTimeMonth;
	m_rxPdo.Year.value            = m_newTimeYear;
	m_rxPdo.No_of_Usages.value    = m_newNoOfUsages;
	m_rxPdo.Digital_Outputs.value = m_newDigitalOutputs;
	m_rxPdo.Grip_Counts.value     = m_newGripCounts;
	m_rxPdo.System_Number.value   = m_newSystemNumber;
	m_rxPdo.Led_Red.value         = m_newLedRed;
	m_rxPdo.Led_Green.value       = m_newLedGreen;
	m_rxPdo.LED_Blue.value        = m_newLEDBlue;
	m_rxPdo.Spare_Bytes.value     = m_newSpareBytes;
	m_rxPdo.Roll_Offset.value     = m_newOffsetRoll;
	m_rxPdo.Pitch_Offset.value    = m_newOffsetPitch;
	m_rxPdo.Yaw_Offset.value      = m_newOffsetYaw;
	m_rxPdo.Grip_Offset.value     = m_newOffsetGrip;

	return dwRes;
}

uint32_t EcSlaveRfidslave::registerPublisher()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCurrentDeviceID.publish();
	m_dseCurrentTimeSecond.publish();
	m_dseCurrentTimeMinute.publish();
	m_dseCurrentTimeHour.publish();
	m_dseCurrentTimeDay.publish();
	m_dseCurrentTimeMonth.publish();
	m_dseCurrentTimeYear.publish();
	m_dseCurrentOffsetRoll.publish();
	m_dseCurrentOffsetPitch.publish();
	m_dseCurrentOffsetYaw.publish();
	m_dseCurrentOffsetGrip.publish();
	m_dseCurrentNoOfUsages.publish();
	m_dseCurrentMaxUsages.publish();
	m_dseCurrentSterileBarrierStatus.publish();
	m_dseCurrentInstrumentStatus.publish();
	m_dseCurrentGripCounts.publish();
	m_dseCurrentSystemNumber.publish();
	m_dseCurrentDeviceUid.publish();
	m_dseCurrentSpareBytes.publish();
	m_dseCurrentMfgDay.publish();
	m_dseCurrentMfgMonth.publish();
	m_dseCurrentMfgYear.publish();

	m_dseNewAcknowledge.publish();
	m_dseNewTimeSecond.publish();
	m_dseNewTimeMinute.publish();
	m_dseNewTimeHour.publish();
	m_dseNewTimeDay.publish();
	m_dseNewTimeMonth.publish();
	m_dseNewTimeYear.publish();
	m_dseNewNoOfUsages.publish();
	m_dseNewDigitalOutputs.publish();
	m_dseNewGripCounts.publish();
	m_dseNewSystemNumber.publish();
	m_dseNewLedRed.publish();
	m_dseNewLedGreen.publish();
	m_dseNewLEDBlue.publish();
	m_dseNewSpareBytes.publish();
	m_dseNewOffsetRoll.publish();
	m_dseNewOffsetPitch.publish();
	m_dseNewOffsetYaw.publish();
	m_dseNewOffsetGrip.publish();

	return dwRes;
}

uint32_t EcSlaveRfidslave::registerSubscriber()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseNewAcknowledge.subscribe();
	m_dseNewTimeSecond.subscribe();
	m_dseNewTimeMinute.subscribe();
	m_dseNewTimeHour.subscribe();
	m_dseNewTimeDay.subscribe();
	m_dseNewTimeMonth.subscribe();
	m_dseNewTimeYear.subscribe();
	m_dseNewNoOfUsages.subscribe();
	m_dseNewDigitalOutputs.subscribe();
	m_dseNewGripCounts.subscribe();
	m_dseNewSystemNumber.subscribe();
	m_dseNewLedRed.subscribe();
	m_dseNewLedGreen.subscribe();
	m_dseNewLEDBlue.subscribe();
	m_dseNewSpareBytes.subscribe();
	m_dseNewOffsetRoll.subscribe();
	m_dseNewOffsetPitch.subscribe();
	m_dseNewOffsetYaw.subscribe();
	m_dseNewOffsetGrip.subscribe();

	return dwRes;
}

uint32_t EcSlaveRfidslave::publishData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseCurrentDeviceID.set(m_txPdo.Device_ID.value);
	m_dseCurrentTimeSecond.set(m_txPdo.Second.value);
	m_dseCurrentTimeMinute.set(m_txPdo.Minute.value);
	m_dseCurrentTimeHour.set(m_txPdo.Hour.value);
	m_dseCurrentTimeDay.set(m_txPdo.Day.value);
	m_dseCurrentTimeMonth.set(m_txPdo.Month.value);
	m_dseCurrentTimeYear.set(m_txPdo.Year.value);
	m_dseCurrentOffsetRoll.set(m_txPdo.Roll_Offset.value);
	m_dseCurrentOffsetPitch.set(m_txPdo.Pitch_Offset.value);
	m_dseCurrentOffsetYaw.set(m_txPdo.Yaw_Offset.value);
	m_dseCurrentOffsetGrip.set(m_txPdo.Grip_Offset.value);
	m_dseCurrentNoOfUsages.set(m_txPdo.No_of_Usages.value);
	m_dseCurrentMaxUsages.set(m_txPdo.Max_Usages.value);
	m_dseCurrentSterileBarrierStatus.set(!((m_txPdo.Digital_Inputs.value & (1 << 0)) == (1 << 0)));
	m_dseCurrentInstrumentStatus.set(!((m_txPdo.Digital_Inputs.value & (1 << 1)) == (1 << 1)));
	m_dseCurrentGripCounts.set(m_txPdo.Grip_Counts.value);
	m_dseCurrentSystemNumber.set(m_txPdo.System_Number.value);
	m_dseCurrentDeviceUid.set(m_txPdo.Device_UID.value);
	m_dseCurrentSpareBytes.set(m_txPdo.Spare_Bytes.value);
	m_dseCurrentMfgDay.set(m_txPdo.MFG_Day.value);
	m_dseCurrentMfgMonth.set(m_txPdo.MFG_Month.value);
	m_dseCurrentMfgYear.set(m_txPdo.MFG_Year.value);

	return dwRes;
}

uint32_t EcSlaveRfidslave::subscribeData()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	m_dseNewAcknowledge.get(m_newAcknowledge);
	m_dseNewTimeSecond.get(m_newTimeSecond);
	m_dseNewTimeMinute.get(m_newTimeMinute);
	m_dseNewTimeHour.get(m_newTimeHour);
	m_dseNewTimeDay.get(m_newTimeDay);
	m_dseNewTimeMonth.get(m_newTimeMonth);
	m_dseNewTimeYear.get(m_newTimeYear);
	m_dseNewNoOfUsages.get(m_newNoOfUsages);
	m_dseNewDigitalOutputs.get(m_newDigitalOutputs);
	m_dseNewGripCounts.get(m_newGripCounts);
	m_dseNewSystemNumber.get(m_newSystemNumber);
	m_dseNewLedRed.get(m_newLedRed);
	m_dseNewLedGreen.get(m_newLedGreen);
	m_dseNewLEDBlue.get(m_newLEDBlue);
	m_dseNewSpareBytes.get(m_newSpareBytes);
	m_dseNewOffsetRoll.get(m_newOffsetRoll);
	m_dseNewOffsetPitch.get(m_newOffsetPitch);
	m_dseNewOffsetYaw.get(m_newOffsetYaw);
	m_dseNewOffsetGrip.get(m_newOffsetGrip);

	return dwRes;
}

uint32_t EcSlaveRfidslave::mainProcess()
{
	uint32_t dwRes = CallbackStatus::SUCCESS;

	return dwRes;
}

void EcSlaveRfidslave::dispTxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Device_ID: " << m_txPdo.Device_ID.value << ", "
	"Second: " << m_txPdo.Second.value << ", "
	"Minute: " << m_txPdo.Minute.value << ", "
	"Hour: " << m_txPdo.Hour.value << ", "
	"Day: " << m_txPdo.Day.value << ", "
	"Month: " << m_txPdo.Month.value << ", "
	"Year: " << m_txPdo.Year.value << ", "
	"Roll_Offset: " << m_txPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_txPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_txPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_txPdo.Grip_Offset.value << ", "
	"No_of_Usages: " << m_txPdo.No_of_Usages.value << ", "
	"Max_Usages: " << m_txPdo.Max_Usages.value << ", "
	"Digital_Inputs: " << m_txPdo.Digital_Inputs.value << ", "
	"Grip_Counts: " << m_txPdo.Grip_Counts.value << ", "
	"System_Number: " << m_txPdo.System_Number.value << ", "
	"Device_UID: " << m_txPdo.Device_UID.value << ", "
	"Spare_Bytes: " << m_txPdo.Spare_Bytes.value << ", "
	"MFG_Day: " << m_txPdo.MFG_Day.value << ", "
	"MFG_Month: " << m_txPdo.MFG_Month.value << ", "
	"MFG_Year: " << m_txPdo.MFG_Year.value
	<< std::endl;
}

void EcSlaveRfidslave::dispRxPdo()
{
	std::cout <<
	"SLAVE_ADDR: " << m_slaveAddr << " | " <<
	"SLAVE_NAME: " << m_slaveName << " | " <<
	"Acknowledge: " << m_rxPdo.Acknowledge.value << ", "
	"Second: " << m_rxPdo.Second.value << ", "
	"Minute: " << m_rxPdo.Minute.value << ", "
	"Hour: " << m_rxPdo.Hour.value << ", "
	"Day: " << m_rxPdo.Day.value << ", "
	"Month: " << m_rxPdo.Month.value << ", "
	"Year: " << m_rxPdo.Year.value << ", "
	"No_of_Usages: " << m_rxPdo.No_of_Usages.value << ", "
	"Digital_Outputs: " << m_rxPdo.Digital_Outputs.value << ", "
	"Grip_Counts: " << m_rxPdo.Grip_Counts.value << ", "
	"System_Number: " << m_rxPdo.System_Number.value << ", "
	"Led_Red: " << m_rxPdo.Led_Red.value << ", "
	"Led_Green: " << m_rxPdo.Led_Green.value << ", "
	"LED_Blue: " << m_rxPdo.LED_Blue.value << ", "
	"Spare_Bytes: " << m_rxPdo.Spare_Bytes.value << ", "
	"Roll_Offset: " << m_rxPdo.Roll_Offset.value << ", "
	"Pitch_Offset: " << m_rxPdo.Pitch_Offset.value << ", "
	"Yaw_Offset: " << m_rxPdo.Yaw_Offset.value << ", "
	"Grip_Offset: " << m_rxPdo.Grip_Offset.value
	<< std::endl;
}
