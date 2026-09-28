#include "EcUser_1.h"

EcUser_1::EcUser_1(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskUserBase(taskId, taskName, taskPath)
{
}

EcUser_1::~EcUser_1()
{
}

uint32_t EcUser_1::registerXml()
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
    registerXmlElementDataStoreValue("dseEStopTrig", m_dseEStopTrig);
    registerXmlElementDataStoreValue("dseEStopClear", m_dseEStopClear);
    registerXmlElementDataStoreValue("dseDisableTrig", m_dseDisableTrig);
    registerXmlElementDataStoreValue("dseClearFaultTrig", m_dseClearFaultTrig);
    registerXmlElementDataStoreValue("dseClearQuickStopTrig", m_dseClearQuickStopTrig);
    registerXmlElementDataStoreValue("dseEnableTrig", m_dseEnableTrig);
    registerXmlElementDataStoreValue("dseReadyTrig", m_dseReadyTrig);
    registerXmlElementDataStoreValue("dseCustomStateTrig", m_dseCustomStateTrig);
    registerXmlElementDataStoreValue("dseStopTrig", m_dseStopTrig);
    
    return dwRes;
}

uint32_t EcUser_1::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseEStopTrig.publish();
    m_dseEStopClear.publish();
    m_dseDisableTrig.publish();
    m_dseClearFaultTrig.publish();
    m_dseClearQuickStopTrig.publish();
    m_dseEnableTrig.publish();
    m_dseReadyTrig.publish();
    m_dseCustomStateTrig.publish();
    m_dseStopTrig.publish();

    return dwRes;
}

uint32_t EcUser_1::registerSubscriber()
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

uint32_t EcUser_1::publishData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseEStopTrig.set(m_data_1);
    m_dseEStopClear.set(m_data_2);
    m_dseDisableTrig.set(m_data_3);
    m_dseClearFaultTrig.set(m_data_4);
    m_dseClearQuickStopTrig.set(m_data_4);
    m_dseEnableTrig.set(m_data_5);
    m_dseReadyTrig.set(m_data_6);
    m_dseCustomStateTrig.set(m_data_7);
    m_dseStopTrig.set(m_data_8);

    return dwRes;
}

uint32_t EcUser_1::subscribeData()
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

uint32_t EcUser_1::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}
