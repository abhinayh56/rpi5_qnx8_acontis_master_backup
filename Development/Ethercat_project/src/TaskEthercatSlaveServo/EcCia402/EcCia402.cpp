#include "EcCia402.h"

EcCia402::EcCia402(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName) :
    EcTaskEthercatSlaveServoBase(taskId, taskName, taskPath, slaveAddr, slaveName)
{
}

EcCia402::~EcCia402()
{
}

uint32_t EcCia402::faultClear()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT)
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::FAULT_RESET;
    }
    else if ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT_REACTION_ACTIVE)
    {
    }

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT) ||
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT_REACTION_ACTIVE))
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }
    else
    {
        dwRes = CallbackStatus::SUCCESS;
    }

    return dwRes;
}

bool EcCia402::isFault()
{
    return (
               ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT_REACTION_ACTIVE) ||
               ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT))
               ? 1 : 0;
}

uint32_t EcCia402::quickStop()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::Data::BitData::OPERATION_ENABLED))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::QUICK_STOP;
    }

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::QUICK_STOP_ACTIVE))
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isQuickStop()
{
    return ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::QUICK_STOP_ACTIVE) ? 1 : 0;
}

uint32_t EcCia402::emergencyStop()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;

    // if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::Data::BitData::OPERATION_ENABLED)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT_REACTION_ACTIVE)
    // )
    // {
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::FAULT_RESET;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::QUICK_STOP_ACTIVE)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCHED_ON)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::READY_TO_SWITCH_ON)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED)
    // )
    // {
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::NOT_READY_TO_SWITCH_ON)
    // )
    // {
    // }

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED))
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isEmergencyStop()
{
    return ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED) ? 1 : 0;
}

uint32_t EcCia402::enable()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::NOT_READY_TO_SWITCH_ON))
    {
    }
    else if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::SHUT_DOWN;
    }
    else if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::READY_TO_SWITCH_ON))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::SWITCH_ON;
    }
    else if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCHED_ON))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::ENABLE_OPERATION;
    }
    else if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::FAULT_RESET;
    }
    else if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::QUICK_STOP_ACTIVE))
    {
        *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    }

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::Data::BitData::OPERATION_ENABLED))
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isEnable()
{
    return ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::Data::BitData::OPERATION_ENABLED) ? 1 : 0;
}

uint32_t EcCia402::disable()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;

    // if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::OPERATION_ENABLED) == EcCia402Data::Object::StatusWord::Data::BitData::OPERATION_ENABLED)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT_REACTION_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT_REACTION_ACTIVE)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::FAULT) == EcCia402Data::Object::StatusWord::Data::BitData::FAULT)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::QUICK_STOP_ACTIVE) == EcCia402Data::Object::StatusWord::Data::BitData::QUICK_STOP_ACTIVE)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCHED_ON) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCHED_ON)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::READY_TO_SWITCH_ON)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }
    // else if
    // (
    //     ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::NOT_READY_TO_SWITCH_ON) == EcCia402Data::Object::StatusWord::Data::BitData::NOT_READY_TO_SWITCH_ON)
    // )
    // {
    //     *m_Cia402PdoRx.controlWord.p_value = EcCia402Data::Object::ControlWord::Data::BitData::DISABLE_VOLTAGE;
    // }

    if (
        ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED))
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isDisable()
{
    return ((*m_Cia402PdoTx.statusWord.p_value & EcCia402Data::Object::StatusWord::Mask::BitMask::SWITCH_ON_DISABLED) == EcCia402Data::Object::StatusWord::Data::BitData::SWITCH_ON_DISABLED) ? 1 : 0;
}

uint32_t EcCia402::setModePosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.modeOfOperation.p_value = EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_POSITION;

    if (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_POSITION)
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isModePosition()
{
    return (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_POSITION) ? 1 : 0;
}

uint32_t EcCia402::setModeVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.modeOfOperation.p_value = EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_VELOCITY;

    if (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_VELOCITY)
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isModeVelocity()
{
    return (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_VELOCITY) ? 1 : 0;
}

uint32_t EcCia402::setModeTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.modeOfOperation.p_value = EcCia402Data::Object::ModeOfOperation::Data::BitData::CYCLIC_SYNC_TORQUE;

    if (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_TORQUE)
    {
        dwRes = CallbackStatus::SUCCESS;
    }
    else
    {
        dwRes = CallbackStatus::IN_PROGRESS;
    }

    return dwRes;
}

bool EcCia402::isModeTorque()
{
    return (*m_Cia402PdoTx.modeOfOperationDisplay.p_value == EcCia402Data::Object::ModeOfOperationDisplay::Data::BitData::CYCLIC_SYNC_TORQUE) ? 1 : 0;
}

int32_t EcCia402::getActualPositionIU()
{
    return *m_Cia402PdoTx.actualPosition.p_value;
}

int32_t EcCia402::getActualVelocityIU()
{
    return *m_Cia402PdoTx.actualVelocity.p_value;
}

int16_t EcCia402::getActualTorqueIU()
{
    return *m_Cia402PdoTx.actualTorque.p_value;
}

uint32_t EcCia402::setTargetPositionIU(int32_t targetPosition)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.targetPosition.p_value = targetPosition;

    return dwRes;
}

uint32_t EcCia402::setTargetVelocityIU(int32_t targetVelocity)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.targetVelocity.p_value = targetVelocity;

    return dwRes;
}

uint32_t EcCia402::setTargetTorqueIU(int16_t targetTorque)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    *m_Cia402PdoRx.targetTorque.p_value = targetTorque;

    return dwRes;
}

uint32_t EcCia402::initPosition()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcCia402::initVelocity()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcCia402::initTorque()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}
