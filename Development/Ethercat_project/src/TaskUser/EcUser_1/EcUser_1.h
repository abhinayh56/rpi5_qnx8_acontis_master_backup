#ifndef USER_1_H
#define USER_1_H

#include "EcTaskUserBase.h"

class EcUser_1 : public EcTaskUserBase
{
public:
	EcUser_1(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

	~EcUser_1();

	uint32_t registerXml() override;

	uint32_t registerPublisher() override;

	uint32_t registerSubscriber() override;

	uint32_t publishData() override;

	uint32_t subscribeData() override;

	uint32_t mainProcess() override;
        
private:
	Data_store_element<bool> m_dseInputCh_1;
	Data_store_element<bool> m_dseInputCh_2;
	Data_store_element<bool> m_dseInputCh_3;
	Data_store_element<bool> m_dseInputCh_4;
	Data_store_element<bool> m_dseInputCh_5;
	Data_store_element<bool> m_dseInputCh_6;
	Data_store_element<bool> m_dseInputCh_7;
	Data_store_element<bool> m_dseInputCh_8;

	Data_store_element<bool> m_dseEStopTrig;
	Data_store_element<bool> m_dseEStopClear;
	Data_store_element<bool> m_dseDisableTrig;
	Data_store_element<bool> m_dseClearFaultTrig;
	Data_store_element<bool> m_dseClearQuickStopTrig;
	Data_store_element<bool> m_dseEnableTrig;
	Data_store_element<bool> m_dseReadyTrig;
	Data_store_element<bool> m_dseCustomStateTrig;
	Data_store_element<bool> m_dseStopTrig;

	bool m_data_1 = false;
	bool m_data_2 = false;
	bool m_data_3 = false;
	bool m_data_4 = false;
	bool m_data_5 = false;
	bool m_data_6 = false;
	bool m_data_7 = false;
	bool m_data_8 = false;
};

#endif // USER_1_H
