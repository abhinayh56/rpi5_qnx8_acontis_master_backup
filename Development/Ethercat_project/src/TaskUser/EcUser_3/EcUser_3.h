#ifndef USER_3_H
#define USER_3_H

#include "EcTaskUserBase.h"

class EcUser_3 : public EcTaskUserBase
{
public:
	EcUser_3(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

	~EcUser_3();

	uint32_t registerXml() override;

	uint32_t registerPublisher() override;

	uint32_t registerSubscriber() override;

	uint32_t publishData() override;

	uint32_t subscribeData() override;

	uint32_t mainProcess() override;
        
private:
	Data_store_element<double> m_dseActualPosition;
	Data_store_element<double> m_dseCommandPosition;
	Data_store_element<bool>   m_dseTiLimitSwitchPressed;
	Data_store_element<bool>   m_dseTiClutchButtonPressed;

	double m_actualPosition;
	double m_commandPosition;
	bool   m_tiLimitSwitchPressed;
	bool   m_tiClutchButtonPressed;
};

#endif // USER_3_H
