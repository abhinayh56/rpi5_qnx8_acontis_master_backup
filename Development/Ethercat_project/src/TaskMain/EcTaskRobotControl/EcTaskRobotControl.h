#ifndef EC_TASK_ROBOT_CONTROL_H
#define EC_TASK_ROBOT_CONTROL_H

#include <vector>
#include "EcTaskRobotControlBase.h"
#include "EcTaskRobotControlFactory.h"

class EcTaskRobotControl
{
public:
    EcTaskRobotControl();

    ~EcTaskRobotControl();

    uint32_t addAll(std::vector<EcTaskRobotControlFactoryNs::TaskData>& taskData);

    uint32_t cleanup();

    uint32_t registerXml();
    
	uint32_t registerPublisher();

	uint32_t registerSubscriber();

    uint32_t publishData();

    uint32_t subscribeData();

    uint32_t mainProcess();

    EcTaskRobotControlBase* getElement(uint32_t index);

private:
    std::vector<EcTaskRobotControlBase*> m_ecTaskRobotControlBaseVector;
    int m_numTaskRobotControl = 0;

    uint32_t addTaskRobotControl(EcTaskRobotControlBase* pTaskRobotControl);
};

#endif // EC_TASK_ROBOT_CONTROL_H
