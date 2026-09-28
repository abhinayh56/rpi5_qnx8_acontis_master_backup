#include "EcUser_4.h"

EcUser_4::EcUser_4(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskUserBase(taskId, taskName, taskPath),
    m_dseActualPosition_1("ACTUAL_POSITION", "/group_2/task/ethercat/servo/motor_5_den_xcr_e", 0, false),
    m_dseActualPosition_2("ACTUAL_POSITION", "/group_2/task/ethercat/servo/motor_6_den_xcr_e", 0, false),
    m_dseActualPosition_3("ACTUAL_POSITION", "/group_2/task/ethercat/servo/motor_7_den_xcr_e", 0, false),
    m_dseActualTorque_1("ACTUAL_TORQUE", "/group_2/task/ethercat/servo/motor_5_den_xcr_e", 0, false),
    m_dseActualTorque_2("ACTUAL_TORQUE", "/group_2/task/ethercat/servo/motor_6_den_xcr_e", 0, false),
    m_dseActualTorque_3("ACTUAL_TORQUE", "/group_2/task/ethercat/servo/motor_7_den_xcr_e", 0, false),
    
    m_dseCommandTorque_1("COMMAND_TORQUE", "/group_2/task/ethercat/servo/motor_5_den_xcr_e", 0, true),
    m_dseCommandTorque_2("COMMAND_TORQUE", "/group_2/task/ethercat/servo/motor_6_den_xcr_e", 0, true),
    m_dseCommandTorque_3("COMMAND_TORQUE", "/group_2/task/ethercat/servo/motor_7_den_xcr_e", 0, true),

    m_actualPosition_1(0),
    m_actualPosition_2(0),
    m_actualPosition_3(0),
    m_actualTorque_1(0),
    m_actualTorque_2(0),
    m_actualTorque_3(0),

    m_commandTorque_1(0),
    m_commandTorque_2(0),
    m_commandTorque_3(0)
{
}

EcUser_4::~EcUser_4()
{
}

uint32_t EcUser_4::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcUser_4::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseCommandTorque_1.publish();
    m_dseCommandTorque_2.publish();
    m_dseCommandTorque_3.publish();

    return dwRes;
}

uint32_t EcUser_4::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition_1.subscribe();
    m_dseActualPosition_2.subscribe();
    m_dseActualPosition_3.subscribe();
    m_dseActualTorque_1.subscribe();
    m_dseActualTorque_2.subscribe();
    m_dseActualTorque_3.subscribe();

    return dwRes;
}

uint32_t EcUser_4::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseCommandTorque_1.set(m_commandTorque_1);
    m_dseCommandTorque_2.set(m_commandTorque_2);
    m_dseCommandTorque_3.set(m_commandTorque_3);

    return dwRes;
}

uint32_t EcUser_4::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition_1.get(m_actualPosition_1);
    m_dseActualPosition_2.get(m_actualPosition_2);
    m_dseActualPosition_3.get(m_actualPosition_3);
    m_dseActualTorque_1.get(m_actualTorque_1);
    m_dseActualTorque_2.get(m_actualTorque_2);
    m_dseActualTorque_3.get(m_actualTorque_3);

    return dwRes;
}

uint32_t EcUser_4::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    // std::cout << count++ << ", " << m_actualPosition_1 << ", " << m_actualPosition_2 << ", " << m_actualPosition_3 << ", "
    //           << m_actualTorque_1 << ", " << m_actualTorque_2 << ", " << m_actualTorque_3 << std::endl;

    // std::cout << "1 : " << m_actualPosition_1 << ", " << m_actualTorque_1 << std::endl;
    // std::cout << "2 : " << m_actualPosition_2 << ", " << m_actualTorque_2 << std::endl;

    m_commandTorque_1 = -970.0 * std::cos(m_actualPosition_1 + 11.0/7.0);
    m_commandTorque_2 = -670.0 * std::cos(m_actualPosition_2) * std::sin(-m_actualPosition_1);
    m_commandTorque_3 = 0;

    return dwRes;
}
