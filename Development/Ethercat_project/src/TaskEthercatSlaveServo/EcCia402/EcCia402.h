#ifndef EC_CIA402_H
#define EC_CIA402_H

#include "EcTaskEthercatSlaveServoBase.h"

namespace EcCia402Data
{
    namespace Object
    {
        namespace ModeOfOperation
        {
            enum Bit : int8_t
            {
            };
            namespace Mask
            {
                enum BitMask : int8_t
                {
                };
            }
            namespace Data
            {
                enum BitData : int8_t
                {
                    NONE = 0,
                    PROFILE_POSITION = 1,
                    VELOCITY = 2,
                    PROFILE_VELOCITY = 3,
                    PROFILE_TORQUE = 4,
                    RESERVED = 5,
                    HOMING = 6,
                    INTERPOLATED_POSITION = 7,
                    CYCLIC_SYNC_POSITION = 8,
                    CYCLIC_SYNC_VELOCITY = 9,
                    CYCLIC_SYNC_TORQUE = 10,
                    CYCLIC_SYNC_TORQUE_WITH_COMMUTATION_ANGLE = 11
                };
            }
        }

        namespace ModeOfOperationDisplay
        {
            enum Bit : int8_t
            {
            };
            namespace Mask
            {
                enum BitMask : int8_t
                {
                };
            }
            namespace Data
            {
                enum BitData : int8_t
                {
                    NONE = 0,
                    PROFILE_POSITION = 1,
                    VELOCITY = 2,
                    PROFILE_VELOCITY = 3,
                    PROFILE_TORQUE = 4,
                    RESERVED = 5,
                    HOMING = 6,
                    INTERPOLATED_POSITION = 7,
                    CYCLIC_SYNC_POSITION = 8,
                    CYCLIC_SYNC_VELOCITY = 9,
                    CYCLIC_SYNC_TORQUE = 10,
                    CYCLIC_SYNC_TORQUE_WITH_COMMUTATION_ANGLE = 11
                };
            }
        }

        namespace ControlWord
        {
            enum Bit : uint16_t
            {
                SWITCHED_ON = (1 << 0),
                ENABLE_VOLTAGE = (1 << 1),
                QUICK_STOP = (1 << 2),
                ENABLE_OPERATION = (1 << 3),
                OPERATION_MODE_SPECIFIC_4 = (1 << 4),
                OPERATION_MODE_SPECIFIC_5 = (1 << 5),
                OPERATION_MODE_SPECIFIC_6 = (1 << 6),
                FAULT_RESET = (1 << 7),
                HALT = (1 << 8),
                OPERATION_MODE_SPECIFIC_9 = (1 << 9),
                RESERVED = (1 << 10),
                MANUFACTURER_SPECIFIC_11 = (1 << 11),
                MANUFACTURER_SPECIFIC_12 = (1 << 12),
                MANUFACTURER_SPECIFIC_13 = (1 << 13),
                MANUFACTURER_SPECIFIC_14 = (1 << 14),
                MANUFACTURER_SPECIFIC_15 = (1 << 15)
            };
            namespace Mask
            {
                enum BitMask : uint16_t
                {
                    SHUT_DOWN = Bit::FAULT_RESET | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    SWITCH_ON = Bit::FAULT_RESET | Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    SWITCH_ON_AND_ENABLE_OPERATION = Bit::FAULT_RESET | Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    DISABLE_VOLTAGE = Bit::FAULT_RESET | Bit::ENABLE_VOLTAGE,
                    QUICK_STOP = Bit::FAULT_RESET | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE,
                    DISABLE_OPERATION = Bit::FAULT_RESET | Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    ENABLE_OPERATION = Bit::FAULT_RESET | Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    FAULT_RESET = Bit::FAULT_RESET
                };
            }
            namespace Data
            {
                enum BitData : uint16_t
                {
                    SHUT_DOWN = Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE,
                    SWITCH_ON = Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    SWITCH_ON_AND_ENABLE_OPERATION = Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    DISABLE_VOLTAGE = 0,
                    QUICK_STOP = Bit::ENABLE_VOLTAGE,
                    DISABLE_OPERATION = Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    ENABLE_OPERATION = Bit::ENABLE_OPERATION | Bit::QUICK_STOP | Bit::ENABLE_VOLTAGE | Bit::SWITCHED_ON,
                    FAULT_RESET = Bit::FAULT_RESET
                };
            }
        }

        namespace StatusWord
        {
            enum Bit : uint16_t
            {
                READY_TO_SWITCH_ON = (1 << 0),
                SWITCHED_ON = (1 << 1),
                OPERATION_ENABLED = (1 << 2),
                FAULT = (1 << 3),
                VOLTAGE_ENABLED = (1 << 4),
                QUICK_STOP = (1 << 5),
                SWITCH_ON_DISABLED = (1 << 6),
                WARNING = (1 << 7),
                MANUFACTURER_SPECIFIC_8 = (1 << 8),
                REMOTE = (1 << 9),
                TARGET_REACHED = (1 << 10),
                INTERNAL_LIMIT_ACTIVE = (1 << 11),
                OPERATION_MODE_SPECIFIC_12 = (1 << 12),
                OPERATION_MODE_SPECIFIC_13 = (1 << 13),
                MANUFACTURER_SPECIFIC_14 = (1 << 14),
                MANUFACTURER_SPECIFIC_15 = (1 << 15)
            };
            namespace Mask
            {
                enum BitMask : uint16_t
                {
                    NOT_READY_TO_SWITCH_ON = Bit::SWITCH_ON_DISABLED | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    SWITCH_ON_DISABLED = Bit::SWITCH_ON_DISABLED | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    READY_TO_SWITCH_ON = Bit::SWITCH_ON_DISABLED | Bit::QUICK_STOP | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    SWITCHED_ON = Bit::SWITCH_ON_DISABLED | Bit::QUICK_STOP | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    OPERATION_ENABLED = Bit::SWITCH_ON_DISABLED | Bit::QUICK_STOP | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    QUICK_STOP_ACTIVE = Bit::SWITCH_ON_DISABLED | Bit::QUICK_STOP | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    FAULT_REACTION_ACTIVE = Bit::SWITCH_ON_DISABLED | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    FAULT = Bit::SWITCH_ON_DISABLED | Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                };
            }
            namespace Data
            {
                enum BitData : uint16_t
                {
                    NOT_READY_TO_SWITCH_ON = 0,
                    SWITCH_ON_DISABLED = Bit::SWITCH_ON_DISABLED,
                    READY_TO_SWITCH_ON = Bit::QUICK_STOP | Bit::READY_TO_SWITCH_ON,
                    SWITCHED_ON = Bit::QUICK_STOP | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    OPERATION_ENABLED = Bit::QUICK_STOP | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    QUICK_STOP_ACTIVE = Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    FAULT_REACTION_ACTIVE = Bit::FAULT | Bit::OPERATION_ENABLED | Bit::SWITCHED_ON | Bit::READY_TO_SWITCH_ON,
                    FAULT = Bit::FAULT
                };
            }
        }
    }

    struct StatusWord
    {
        uint16_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ModeOfOperationDisplay
    {
        int8_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ActualPosition
    {
        int32_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ActualVelocity
    {
        int32_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ActualTorque
    {
        int16_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ControlWord
    {
        uint16_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct ModeOfOperation
    {
        int8_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct TargetPosition
    {
        int32_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct TargetVelocity
    {
        int32_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct TargetTorque
    {
        int16_t *p_value = nullptr;
        bool *p_isSupported = nullptr;
    };

    struct PdoTx
    {
        StatusWord statusWord;
        ModeOfOperationDisplay modeOfOperationDisplay;
        ActualPosition actualPosition;
        ActualVelocity actualVelocity;
        ActualTorque actualTorque;
    };

    struct PdoRx
    {
        ControlWord controlWord;
        ModeOfOperation modeOfOperation;
        TargetPosition targetPosition;
        TargetVelocity targetVelocity;
        TargetTorque targetTorque;
    };
}

class EcCia402 : public EcTaskEthercatSlaveServoBase
{
public:
    EcCia402(uint16_t taskId, const std::string &taskName, const std::string &taskPath, uint16_t slaveAddr, const std::string &slaveName);

    virtual ~EcCia402();

    virtual uint32_t faultClear() override;

    virtual bool isFault() override;

    virtual uint32_t quickStop() override;

    virtual bool isQuickStop() override;

    virtual uint32_t emergencyStop() override;

    virtual bool isEmergencyStop() override;

    virtual uint32_t enable() override;

    virtual bool isEnable() override;

    virtual uint32_t disable() override;

    virtual bool isDisable() override;

    virtual uint32_t setModePosition() override;

    virtual bool isModePosition() override;

    virtual uint32_t setModeVelocity() override;

    virtual bool isModeVelocity() override;

    virtual uint32_t setModeTorque() override;

    virtual bool isModeTorque() override;

    virtual int32_t getActualPositionIU() override;

    virtual int32_t getActualVelocityIU() override;

    virtual int16_t getActualTorqueIU() override;

    virtual uint32_t setTargetPositionIU(int32_t targetPosition) override;

    virtual uint32_t setTargetVelocityIU(int32_t targetVelocity) override;

    virtual uint32_t setTargetTorqueIU(int16_t targetTorque) override;

    virtual uint32_t initPosition() override;

    virtual uint32_t initVelocity() override;

    virtual uint32_t initTorque() override;

protected:
    EcCia402Data::PdoTx m_Cia402PdoTx;
    EcCia402Data::PdoRx m_Cia402PdoRx;
};

#endif // EC_CIA402_H
