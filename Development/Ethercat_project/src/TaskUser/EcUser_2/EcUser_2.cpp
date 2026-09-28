#include "EcUser_2.h"

EcUser_2::EcUser_2(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskUserBase(taskId, taskName, taskPath)
{
}

EcUser_2::~EcUser_2()
{
}

uint32_t EcUser_2::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseInputCh_1", m_dseInputCh_1);
    registerXmlElementDataStoreValue("dseInputCh_2", m_dseInputCh_2);
    registerXmlElementDataStoreValue("dseInputCh_3", m_dseInputCh_3);
    registerXmlElementDataStoreValue("dseInputCh_4", m_dseInputCh_4);
    registerXmlElementDataStoreValue("dseInputCh_5", m_dseInputCh_5);
    registerXmlElementDataStoreValue("dseInputCh_6", m_dseInputCh_6);
    registerXmlElementDataStoreValue("dseInputCh_7", m_dseInputCh_7);
    registerXmlElementDataStoreValue("dseInputCh_8", m_dseInputCh_8);
    registerXmlElementDataStoreValue("dseOutputCh_1", m_dseOutputCh_1);
    registerXmlElementDataStoreValue("dseOutputCh_2", m_dseOutputCh_2);
    registerXmlElementDataStoreValue("dseOutputCh_3", m_dseOutputCh_3);
    registerXmlElementDataStoreValue("dseOutputCh_4", m_dseOutputCh_4);
    registerXmlElementDataStoreValue("dseOutputCh_5", m_dseOutputCh_5);
    registerXmlElementDataStoreValue("dseOutputCh_6", m_dseOutputCh_6);
    registerXmlElementDataStoreValue("dseOutputCh_7", m_dseOutputCh_7);
    registerXmlElementDataStoreValue("dseOutputCh_8", m_dseOutputCh_8);
    
    return dwRes;
}

uint32_t EcUser_2::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseOutputCh_1.publish();
    m_dseOutputCh_2.publish();
    m_dseOutputCh_3.publish();
    m_dseOutputCh_4.publish();
    m_dseOutputCh_5.publish();
    m_dseOutputCh_6.publish();
    m_dseOutputCh_7.publish();
    m_dseOutputCh_8.publish();

    return dwRes;
}

uint32_t EcUser_2::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseInputCh_1.subscribe();
    m_dseInputCh_2.subscribe();
    m_dseInputCh_3.subscribe();
    m_dseInputCh_4.subscribe();
    m_dseInputCh_5.subscribe();
    m_dseInputCh_6.subscribe();
    m_dseInputCh_7.subscribe();
    m_dseInputCh_8.subscribe();

    return dwRes;
}

uint32_t EcUser_2::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseOutputCh_1.set(m_data_1);
    m_dseOutputCh_2.set(m_data_2);
    m_dseOutputCh_3.set(m_data_3);
    m_dseOutputCh_4.set(m_data_4);
    m_dseOutputCh_5.set(m_data_5);
    m_dseOutputCh_6.set(m_data_6);
    m_dseOutputCh_7.set(m_data_7);
    m_dseOutputCh_8.set(m_data_8);

    return dwRes;
}

uint32_t EcUser_2::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseInputCh_1.get(m_data_1);
    m_dseInputCh_2.get(m_data_2);
    m_dseInputCh_3.get(m_data_3);
    m_dseInputCh_4.get(m_data_4);
    m_dseInputCh_5.get(m_data_5);
    m_dseInputCh_6.get(m_data_6);
    m_dseInputCh_7.get(m_data_7);
    m_dseInputCh_8.get(m_data_8);

    return dwRes;
}

uint32_t EcUser_2::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

