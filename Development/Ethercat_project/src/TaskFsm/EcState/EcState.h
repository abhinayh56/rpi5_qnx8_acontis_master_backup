#ifndef EC_STATE_H
#define EC_STATE_H

#include <stdint.h>

#include "EcTaskEthercatSlave.h"
#include "EcTaskEthercatSlaveServo.h"
#include "EcTaskRobotControl.h"
#include "EcTaskUser.h"
#include "EcTaskInterface.h"

namespace EcStateData
{
    enum StateId : uint32_t
    {
        STANDBY = 101,
        DISABLING = 102,
        DISABLED = 103,
        ENABLING = 104,
        ENABLED = 105,
        READY = 106,
        JOYSTICK_CONTROL = 107,
        STOP = 108,
        FAULT = 109,
        CLEARING_FAULT = 110,
        E_STOP = 111,
        CLEARING_E_STOP = 112,
        QUICK_STOP = 113,
        CLEARING_QUICK_STOP = 114,
        STATE_X = 115
    };
}

struct CtxtSubroutine
{
    EcTaskEthercatSlave*      p_ecTaskEthercatSlave;
    EcTaskEthercatSlaveServo* p_ecTaskEthercatSlaveServo;
    EcTaskRobotControl*       p_ecTaskRobotControl;
    EcTaskUser*               p_ecTaskUser;
    // EcTaskInterface*          p_ecTaskInterface;
};

#endif // EC_STATE_H
