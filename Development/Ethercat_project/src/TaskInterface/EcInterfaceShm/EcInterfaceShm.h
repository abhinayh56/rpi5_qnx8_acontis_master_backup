#ifndef EC_INTERFACE_SHM_H
#define EC_INTERFACE_SHM_H

#include "EcTaskInterfaceBase.h"

class EcInterfaceShm : public EcTaskInterfaceBase
{
public:
    EcInterfaceShm(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

    ~EcInterfaceShm();

    uint32_t registerXml();

    uint32_t setStore();

    uint32_t registerPublisher() override;

    uint32_t registerSubscriber() override;

    uint32_t publishData() override;

    uint32_t subscribeData() override;

    uint32_t mainProcess() override;

    uint32_t registerAllShm() override;

    uint32_t registerPublisherShm() override;

    uint32_t registerSubscriberShm() override;

    uint32_t publishDataShm() override;

    uint32_t subscribeDataShm() override;

private:
    Data_store_element<double> m_dseActualPosition;
    Data_store_element<double> m_dseCommandPosition;
    Data_store_element<bool>   m_dseTiLimitSwitchPressed;
    Data_store_element<bool>   m_dseTiClutchButtonPressed;

    Data_store_element<bool> m_dseShm_1_group_1;
	Data_store_element<bool> m_dseShm_2_group_1;
	Data_store_element<bool> m_dseShm_3_group_1;
	Data_store_element<bool> m_dseShm_4_group_1;
	Data_store_element<bool> m_dseShm_5_group_1;
	Data_store_element<bool> m_dseShm_6_group_1;
	Data_store_element<bool> m_dseShm_7_group_1;
	Data_store_element<bool> m_dseShm_8_group_1;

    Data_store_element<bool> m_dseShm_1_group_2;
	Data_store_element<bool> m_dseShm_2_group_2;
	Data_store_element<bool> m_dseShm_3_group_2;
	Data_store_element<bool> m_dseShm_4_group_2;
	Data_store_element<bool> m_dseShm_5_group_2;
	Data_store_element<bool> m_dseShm_6_group_2;
	Data_store_element<bool> m_dseShm_7_group_2;
	Data_store_element<bool> m_dseShm_8_group_2;

    double m_actualPosition;
    double m_commandPosition;
    bool   m_tiLimitSwitchPressed;
    bool   m_tiClutchButtonPressed;

    bool m_Shm_1_group_1 = false;
    bool m_Shm_2_group_1 = false;
    bool m_Shm_3_group_1 = false;
    bool m_Shm_4_group_1 = false;
    bool m_Shm_5_group_1 = false;
    bool m_Shm_6_group_1 = false;
    bool m_Shm_7_group_1 = false;
    bool m_Shm_8_group_1 = false;

    bool m_Shm_1_group_2 = false;
    bool m_Shm_2_group_2 = false;
    bool m_Shm_3_group_2 = false;
    bool m_Shm_4_group_2 = false;
    bool m_Shm_5_group_2 = false;
    bool m_Shm_6_group_2 = false;
    bool m_Shm_7_group_2 = false;
    bool m_Shm_8_group_2 = false;

    Shm_element<double> m_smeActualPosition;
    Shm_element<double> m_smeCommandPosition;
    Shm_element<bool>   m_smeTiLimitSwitchPressed;
    Shm_element<bool>   m_smeTiClutchButtonPressed;

    Shm_element<bool> m_smeShm_1_group_1;
    Shm_element<bool> m_smeShm_2_group_1;
    Shm_element<bool> m_smeShm_3_group_1;
    Shm_element<bool> m_smeShm_4_group_1;
    Shm_element<bool> m_smeShm_5_group_1;
    Shm_element<bool> m_smeShm_6_group_1;
    Shm_element<bool> m_smeShm_7_group_1;
    Shm_element<bool> m_smeShm_8_group_1;
    Shm_element<bool> m_smeShm_1_group_2;
    Shm_element<bool> m_smeShm_2_group_2;
    Shm_element<bool> m_smeShm_3_group_2;
    Shm_element<bool> m_smeShm_4_group_2;
    Shm_element<bool> m_smeShm_5_group_2;
    Shm_element<bool> m_smeShm_6_group_2;
    Shm_element<bool> m_smeShm_7_group_2;
    Shm_element<bool> m_smeShm_8_group_2;
};

#endif // EC_INTERFACE_SHM_H
