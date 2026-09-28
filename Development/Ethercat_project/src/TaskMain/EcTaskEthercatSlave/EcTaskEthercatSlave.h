#ifndef EC_TASK_ETHERCAT_SLAVE_H
#define EC_TASK_ETHERCAT_SLAVE_H

#include <vector>
#include "EcTaskEthercatSlaveBase.h"
#include "EcTaskEthercatSlaveFactory.h"

class EcTaskEthercatSlave
{
public:
    EcTaskEthercatSlave();

    ~EcTaskEthercatSlave();

    uint32_t addAll(std::vector<EcTaskEthercatSlaveFactoryNs::TaskData>& taskData);

    uint32_t cleanup();

    uint32_t checkSlave();

    uint32_t registerPdo();
    
	uint32_t registerXml();
    
    uint32_t registerPublisher();

	uint32_t registerSubscriber();

    uint32_t transferTxPdo();

    uint32_t transferRxPdo();

    uint32_t processTxPdo();

    uint32_t processRxPdo();

    uint32_t publishData();

    uint32_t subscribeData();

    uint32_t mainProcess();

    void dispTxPdo();

    void dispRxPdo();

    EcTaskEthercatSlaveBase* getElement(uint32_t index);

private:
    std::vector<EcTaskEthercatSlaveBase*> m_ecTaskEthercatSlaveBaseVector;

    uint32_t addTaskSlave(EcTaskEthercatSlaveBase* pSlave);
};

#endif // EC_TASK_ETHERCAT_SLAVE_H
