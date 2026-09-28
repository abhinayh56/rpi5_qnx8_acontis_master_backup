#include "EcUser_5.h"

EcUser_5::EcUser_5(uint16_t taskId, const std::string &taskName, const std::string &taskPath) :
    EcTaskUserBase(taskId, taskName, taskPath)
{
}

EcUser_5::~EcUser_5()
{
}

uint32_t EcUser_5::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    registerXmlElementDataStoreValue("dseShm_1", m_dseShm_1);
    registerXmlElementDataStoreValue("dseShm_2", m_dseShm_2);
    registerXmlElementDataStoreValue("dseShm_3", m_dseShm_3);
    registerXmlElementDataStoreValue("dseShm_4", m_dseShm_4);
    registerXmlElementDataStoreValue("dseShm_5", m_dseShm_5);
    registerXmlElementDataStoreValue("dseShm_6", m_dseShm_6);
    registerXmlElementDataStoreValue("dseShm_7", m_dseShm_7);
    registerXmlElementDataStoreValue("dseShm_8", m_dseShm_8);
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

uint32_t EcUser_5::registerPublisher()
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

uint32_t EcUser_5::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseShm_1.subscribe();
    m_dseShm_2.subscribe();
    m_dseShm_3.subscribe();
    m_dseShm_4.subscribe();
    m_dseShm_5.subscribe();
    m_dseShm_6.subscribe();
    m_dseShm_7.subscribe();
    m_dseShm_8.subscribe();

    return dwRes;
}

uint32_t EcUser_5::publishData()
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

uint32_t EcUser_5::subscribeData()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_dseShm_1.get(m_data_1);
    m_dseShm_2.get(m_data_2);
    m_dseShm_3.get(m_data_3);
    m_dseShm_4.get(m_data_4);
    m_dseShm_5.get(m_data_5);
    m_dseShm_6.get(m_data_6);
    m_dseShm_7.get(m_data_7);
    m_dseShm_8.get(m_data_8);

    return dwRes;
}

uint32_t EcUser_5::mainProcess()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}
