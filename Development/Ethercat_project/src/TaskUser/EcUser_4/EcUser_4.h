#ifndef USER_4_H
#define USER_4_H

#include "EcTaskUserBase.h"
#include "cmath"
#include "math.h"

class EcUser_4 : public EcTaskUserBase
{
public:
	EcUser_4(uint16_t taskId, const std::string &taskName, const std::string &taskPath);

	~EcUser_4();

	uint32_t registerXml() override;

	uint32_t registerPublisher() override;

	uint32_t registerSubscriber() override;

	uint32_t publishData() override;

	uint32_t subscribeData() override;

	uint32_t mainProcess() override;
        
private:
	Data_store_element<double> m_dseActualPosition_1;
	Data_store_element<double> m_dseActualPosition_2;
	Data_store_element<double> m_dseActualPosition_3;
	Data_store_element<double> m_dseActualTorque_1;
	Data_store_element<double> m_dseActualTorque_2;
	Data_store_element<double> m_dseActualTorque_3;
	Data_store_element<double> m_dseCommandTorque_1;
	Data_store_element<double> m_dseCommandTorque_2;
	Data_store_element<double> m_dseCommandTorque_3;

	double m_actualPosition_1;
	double m_actualPosition_2;
	double m_actualPosition_3;
	double m_actualTorque_1;
	double m_actualTorque_2;
	double m_actualTorque_3;

	double m_commandTorque_1;
	double m_commandTorque_2;
	double m_commandTorque_3;

	uint64_t count = 0;
};

#endif // USER_4_H
