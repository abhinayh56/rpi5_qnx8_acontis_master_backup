#ifndef EC_TASK_INTERFACE_H
#define EC_TASK_INTERFACE_H

#include <vector>
#include "EcTaskInterfaceBase.h"
#include "EcInterfaceShm.h"

class EcTaskInterface
{
public:
	EcTaskInterface();

	~EcTaskInterface();

	uint32_t addAll(std::vector<Shm_store> &shmStoreVector, std::vector<std::string> &shmLayoutXpathVector);

	uint32_t cleanup();

	uint32_t registerXml();
	
	uint32_t registerPublisher();

	uint32_t registerSubscriber();

	uint32_t publishData();

	uint32_t subscribeData();

	uint32_t mainProcess();

	uint32_t registerAllShm();

	uint32_t registerPublisherShm();

	uint32_t registerSubscriberShm();

	uint32_t publishDataShm();

	uint32_t subscribeDataShm();

	EcTaskInterfaceBase *getElement(uint32_t index);

private:
	std::vector<EcTaskInterfaceBase *> m_ecTaskInterfaceBaseVector;
};

#endif // EC_TASK_INTERFACE_H
