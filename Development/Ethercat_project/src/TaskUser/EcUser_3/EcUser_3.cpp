#include "EcUser_3.h"

EcUser_3::EcUser_3(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskUserBase(taskId, taskName, taskPath),
    m_actualPosition(0),
    m_commandPosition(0),
    m_tiLimitSwitchPressed(false),
    m_tiClutchButtonPressed(false)
{
}

EcUser_3::~EcUser_3()
{
}

uint32_t EcUser_3::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    registerXmlElementDataStoreValue("dseActualPosition", m_dseActualPosition);
    registerXmlElementDataStoreValue("dseCommandPosition", m_dseCommandPosition);
    registerXmlElementDataStoreValue("dseTiLimitSwitchPressed", m_dseTiLimitSwitchPressed);
    registerXmlElementDataStoreValue("dseTiClutchButtonPressed", m_dseTiClutchButtonPressed);
    
    return dwRes;
}

uint32_t EcUser_3::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseCommandPosition.publish();

    return dwRes;
}

uint32_t EcUser_3::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition.subscribe();
    m_dseTiLimitSwitchPressed.subscribe();
    m_dseTiClutchButtonPressed.subscribe();

    return dwRes;
}

uint32_t EcUser_3::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    // m_dseCommandPosition.set(m_commandPosition);

    return dwRes;
}

uint32_t EcUser_3::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition.get(m_actualPosition);
    m_dseTiLimitSwitchPressed.get(m_tiLimitSwitchPressed);
    m_dseTiClutchButtonPressed.get(m_tiClutchButtonPressed);

    return dwRes;
}

uint32_t EcUser_3::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    // std::cout << "Ti clutch, limitSwitch, actual_angle         : " << m_tiClutchButtonPressed << ", " << m_tiLimitSwitchPressed << ", " << m_actualPosition << std::endl;

    return dwRes;
}
