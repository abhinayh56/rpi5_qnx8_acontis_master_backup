#include "EcInterfaceShm.h"

EcInterfaceShm::EcInterfaceShm(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskInterfaceBase(taskId, taskName, taskPath),
    m_actualPosition(0),
    m_commandPosition(0),
    m_tiLimitSwitchPressed(false),
    m_tiClutchButtonPressed(false)
{
}

EcInterfaceShm::~EcInterfaceShm()
{
}

uint32_t EcInterfaceShm::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition.set_info("ACTUAL_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, true);
    m_dseCommandPosition.set_info("COMMAND_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, true);
    m_dseTiLimitSwitchPressed.set_info("TI_LIMIT_SWITCH_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, true);
    m_dseTiClutchButtonPressed.set_info("TI_CLUTCH_BUTTON_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, true);

    m_dseShm_1_group_1.set_info("SHM_CH_1", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_2_group_1.set_info("SHM_CH_2", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_3_group_1.set_info("SHM_CH_3", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_4_group_1.set_info("SHM_CH_4", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_5_group_1.set_info("SHM_CH_5", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_6_group_1.set_info("SHM_CH_6", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_7_group_1.set_info("SHM_CH_7", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_8_group_1.set_info("SHM_CH_8", "/group_1/task/interface/shm", 0, 1);
    m_dseShm_1_group_2.set_info("SHM_CH_1", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_2_group_2.set_info("SHM_CH_2", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_3_group_2.set_info("SHM_CH_3", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_4_group_2.set_info("SHM_CH_4", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_5_group_2.set_info("SHM_CH_5", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_6_group_2.set_info("SHM_CH_6", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_7_group_2.set_info("SHM_CH_7", "/group_2/task/interface/shm", 0, 1);
    m_dseShm_8_group_2.set_info("SHM_CH_8", "/group_2/task/interface/shm", 0, 1);
    
    m_smeActualPosition.set_info("ACTUAL_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0);
    m_smeCommandPosition.set_info("COMMAND_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0);
    m_smeTiLimitSwitchPressed.set_info("TI_LIMIT_SWITCH_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0);
    m_smeTiClutchButtonPressed.set_info("TI_CLUTCH_BUTTON_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0);

    m_smeShm_1_group_1.set_info("SHM_CH_1", "/group_1/task/interface/shm", 0);
    m_smeShm_2_group_1.set_info("SHM_CH_2", "/group_1/task/interface/shm", 0);
    m_smeShm_3_group_1.set_info("SHM_CH_3", "/group_1/task/interface/shm", 0);
    m_smeShm_4_group_1.set_info("SHM_CH_4", "/group_1/task/interface/shm", 0);
    m_smeShm_5_group_1.set_info("SHM_CH_5", "/group_1/task/interface/shm", 0);
    m_smeShm_6_group_1.set_info("SHM_CH_6", "/group_1/task/interface/shm", 0);
    m_smeShm_7_group_1.set_info("SHM_CH_7", "/group_1/task/interface/shm", 0);
    m_smeShm_8_group_1.set_info("SHM_CH_8", "/group_1/task/interface/shm", 0);
    m_smeShm_1_group_2.set_info("SHM_CH_1", "/group_2/task/interface/shm", 0);
    m_smeShm_2_group_2.set_info("SHM_CH_2", "/group_2/task/interface/shm", 0);
    m_smeShm_3_group_2.set_info("SHM_CH_3", "/group_2/task/interface/shm", 0);
    m_smeShm_4_group_2.set_info("SHM_CH_4", "/group_2/task/interface/shm", 0);
    m_smeShm_5_group_2.set_info("SHM_CH_5", "/group_2/task/interface/shm", 0);
    m_smeShm_6_group_2.set_info("SHM_CH_6", "/group_2/task/interface/shm", 0);
    m_smeShm_7_group_2.set_info("SHM_CH_7", "/group_2/task/interface/shm", 0);
    m_smeShm_8_group_2.set_info("SHM_CH_8", "/group_2/task/interface/shm", 0);

    m_xPath = m_shmLayoutXpath;
    // registerXmlElementDataStoreValue("dseActualPosition", m_dseActualPosition);
    // registerXmlElementDataStoreValue("dseCommandPosition", m_dseCommandPosition);
    // registerXmlElementDataStoreValue("dseTiLimitSwitchPressed", m_dseTiLimitSwitchPressed);
    // registerXmlElementDataStoreValue("dseTiClutchButtonPressed", m_dseTiClutchButtonPressed);

    setStore();

    return dwRes;
}

uint32_t EcInterfaceShm::setStore()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_smeActualPosition.setStore(*m_shm_store);
    m_smeCommandPosition.setStore(*m_shm_store);
    m_smeTiLimitSwitchPressed.setStore(*m_shm_store);
    m_smeTiClutchButtonPressed.setStore(*m_shm_store);
    m_smeShm_1_group_1.setStore(*m_shm_store);
    m_smeShm_2_group_1.setStore(*m_shm_store);
    m_smeShm_3_group_1.setStore(*m_shm_store);
    m_smeShm_4_group_1.setStore(*m_shm_store);
    m_smeShm_5_group_1.setStore(*m_shm_store);
    m_smeShm_6_group_1.setStore(*m_shm_store);
    m_smeShm_7_group_1.setStore(*m_shm_store);
    m_smeShm_8_group_1.setStore(*m_shm_store);
    m_smeShm_1_group_2.setStore(*m_shm_store);
    m_smeShm_2_group_2.setStore(*m_shm_store);
    m_smeShm_3_group_2.setStore(*m_shm_store);
    m_smeShm_4_group_2.setStore(*m_shm_store);
    m_smeShm_5_group_2.setStore(*m_shm_store);
    m_smeShm_6_group_2.setStore(*m_shm_store);
    m_smeShm_7_group_2.setStore(*m_shm_store);
    m_smeShm_8_group_2.setStore(*m_shm_store);

    return dwRes;
}

uint32_t EcInterfaceShm::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseCommandPosition.publish();

    m_dseShm_1_group_1.publish();
    m_dseShm_2_group_1.publish();
    m_dseShm_3_group_1.publish();
    m_dseShm_4_group_1.publish();
    m_dseShm_5_group_1.publish();
    m_dseShm_6_group_1.publish();
    m_dseShm_7_group_1.publish();
    m_dseShm_8_group_1.publish();
    m_dseShm_1_group_2.publish();
    m_dseShm_2_group_2.publish();
    m_dseShm_3_group_2.publish();
    m_dseShm_4_group_2.publish();
    m_dseShm_5_group_2.publish();
    m_dseShm_6_group_2.publish();
    m_dseShm_7_group_2.publish();
    m_dseShm_8_group_2.publish();

    return dwRes;
}

uint32_t EcInterfaceShm::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition.subscribe();
    m_dseTiLimitSwitchPressed.subscribe();
    m_dseTiClutchButtonPressed.subscribe();

    m_dseShm_1_group_1.subscribe();
    m_dseShm_2_group_1.subscribe();
    m_dseShm_3_group_1.subscribe();
    m_dseShm_4_group_1.subscribe();
    m_dseShm_5_group_1.subscribe();
    m_dseShm_6_group_1.subscribe();
    m_dseShm_7_group_1.subscribe();
    m_dseShm_8_group_1.subscribe();
    m_dseShm_1_group_2.subscribe();
    m_dseShm_2_group_2.subscribe();
    m_dseShm_3_group_2.subscribe();
    m_dseShm_4_group_2.subscribe();
    m_dseShm_5_group_2.subscribe();
    m_dseShm_6_group_2.subscribe();
    m_dseShm_7_group_2.subscribe();
    m_dseShm_8_group_2.subscribe();

    return dwRes;
}

uint32_t EcInterfaceShm::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseCommandPosition.set(m_commandPosition);
    m_dseShm_1_group_1.set(m_Shm_1_group_1);
    m_dseShm_2_group_1.set(m_Shm_2_group_1);
    m_dseShm_3_group_1.set(m_Shm_3_group_1);
    m_dseShm_4_group_1.set(m_Shm_4_group_1);
    m_dseShm_5_group_1.set(m_Shm_5_group_1);
    m_dseShm_6_group_1.set(m_Shm_6_group_1);
    m_dseShm_7_group_1.set(m_Shm_7_group_1);
    m_dseShm_8_group_1.set(m_Shm_8_group_1);
    m_dseShm_1_group_2.set(m_Shm_1_group_2);
    m_dseShm_2_group_2.set(m_Shm_2_group_2);
    m_dseShm_3_group_2.set(m_Shm_3_group_2);
    m_dseShm_4_group_2.set(m_Shm_4_group_2);
    m_dseShm_5_group_2.set(m_Shm_5_group_2);
    m_dseShm_6_group_2.set(m_Shm_6_group_2);
    m_dseShm_7_group_2.set(m_Shm_7_group_2);
    m_dseShm_8_group_2.set(m_Shm_8_group_2);

    return dwRes;
}

uint32_t EcInterfaceShm::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseActualPosition.get(m_actualPosition);
    m_dseTiLimitSwitchPressed.get(m_tiLimitSwitchPressed);
    m_dseTiClutchButtonPressed.get(m_tiClutchButtonPressed);

    return dwRes;
}

uint32_t EcInterfaceShm::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    // std::cout << "Ti clutch, limitSwitch, actual_angle         : " << m_tiClutchButtonPressed << ", " << m_tiLimitSwitchPressed << ", " << m_actualPosition << std::endl;

    return dwRes;
}

uint32_t EcInterfaceShm::registerAllShm()
{

    m_smeActualPosition.register_element();
    m_smeCommandPosition.register_element();
    m_smeTiLimitSwitchPressed.register_element();
    m_smeTiClutchButtonPressed.register_element();

    m_smeShm_1_group_1.register_element();
    m_smeShm_2_group_1.register_element();
    m_smeShm_3_group_1.register_element();
    m_smeShm_4_group_1.register_element();
    m_smeShm_5_group_1.register_element();
    m_smeShm_6_group_1.register_element();
    m_smeShm_7_group_1.register_element();
    m_smeShm_8_group_1.register_element();
    m_smeShm_1_group_2.register_element();
    m_smeShm_2_group_2.register_element();
    m_smeShm_3_group_2.register_element();
    m_smeShm_4_group_2.register_element();
    m_smeShm_5_group_2.register_element();
    m_smeShm_6_group_2.register_element();
    m_smeShm_7_group_2.register_element();
    m_smeShm_8_group_2.register_element();

    return CallbackStatus::SUCCESS;
}

uint32_t EcInterfaceShm::registerPublisherShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcInterfaceShm::registerSubscriberShm()
{
    return CallbackStatus::SUCCESS;
}

uint32_t EcInterfaceShm::publishDataShm()
{
    m_smeActualPosition.set(m_actualPosition);
    m_smeTiLimitSwitchPressed.set(m_tiLimitSwitchPressed);
    m_smeTiClutchButtonPressed.set(m_tiClutchButtonPressed);

    return CallbackStatus::SUCCESS;
}

uint32_t EcInterfaceShm::subscribeDataShm()
{
    m_smeCommandPosition.get(m_commandPosition);
    m_smeShm_1_group_1.get(m_Shm_1_group_1);
    m_smeShm_2_group_1.get(m_Shm_2_group_1);
    m_smeShm_3_group_1.get(m_Shm_3_group_1);
    m_smeShm_4_group_1.get(m_Shm_4_group_1);
    m_smeShm_5_group_1.get(m_Shm_5_group_1);
    m_smeShm_6_group_1.get(m_Shm_6_group_1);
    m_smeShm_7_group_1.get(m_Shm_7_group_1);
    m_smeShm_8_group_1.get(m_Shm_8_group_1);
    m_smeShm_1_group_2.get(m_Shm_1_group_2);
    m_smeShm_2_group_2.get(m_Shm_2_group_2);
    m_smeShm_3_group_2.get(m_Shm_3_group_2);
    m_smeShm_4_group_2.get(m_Shm_4_group_2);
    m_smeShm_5_group_2.get(m_Shm_5_group_2);
    m_smeShm_6_group_2.get(m_Shm_6_group_2);
    m_smeShm_7_group_2.get(m_Shm_7_group_2);
    m_smeShm_8_group_2.get(m_Shm_8_group_2);

    // std::cout
    // << m_Shm_1_group_1 << ", "
    // << m_Shm_2_group_1 << ", "
    // << m_Shm_3_group_1 << ", "
    // << m_Shm_4_group_1 << ", "
    // << m_Shm_5_group_1 << ", "
    // << m_Shm_6_group_1 << ", "
    // << m_Shm_7_group_1 << ", "
    // << m_Shm_8_group_1 << std::endl;
    // " | SHM 2: "
    // << m_Shm_1_group_2 << ", "
    // << m_Shm_2_group_2 << ", "
    // << m_Shm_3_group_2 << ", "
    // << m_Shm_4_group_2 << ", "
    // << m_Shm_5_group_2 << ", "
    // << m_Shm_6_group_2 << ", "
    // << m_Shm_7_group_2 << ", "
    // << m_Shm_8_group_2 << std::endl;

    return CallbackStatus::SUCCESS;
}