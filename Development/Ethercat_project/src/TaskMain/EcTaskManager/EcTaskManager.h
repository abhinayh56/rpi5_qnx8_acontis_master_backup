#ifndef EC_TASK_MANAGER_H
#define EC_TASK_MANAGER_H

#include "EcTaskEthercatSlave.h"
#include "EcTaskEthercatSlaveServo.h"
#include "EcTaskRobotControl.h"
#include "EcTaskUser.h"
#include "EcTaskInterface.h"
#include "EcTaskFsm.h"
#include <vector>
#include "xmlHelper.h"
#include "IPC.h"

namespace EcTaskManagerNs
{
    struct TaskGroup
    {
        EcTaskEthercatSlave ecTaskEthercatSlave;
        EcTaskEthercatSlaveServo ecTaskEthercatSlaveServo;
        EcTaskRobotControl ecTaskRobotControl;
        EcTaskUser ecTaskUser;
        EcTaskFsm ecTaskFsm;

        CtxtSubroutine m_ctxt;
    };

    struct TaskGroupData
    {
        std::vector<EcTaskEthercatSlaveFactoryNs::TaskData>      taskEthercatSlaveDataVector;
        std::vector<EcTaskEthercatSlaveServoFactoryNs::TaskData> taskEthercatSlaveServoDataVector;
        std::vector<EcTaskRobotControlFactoryNs::TaskData>       taskRobotControlDataVector;
        std::vector<EcTaskUserFactoryNs::TaskData>               taskUserDataVector;
        std::vector<EcTaskStateFactoryNs::TaskData>              taskStateDataVector;
    };

    struct TaskInterfaceData
    {
        std::vector<std::string> shmNameVector;
        std::vector<uint64_t>    shmSizeVector;
        std::vector<std::string> shmLayoutXpathVector;
        std::vector<Shm_store>   shmStoreVector;
    };
}

class EcTaskManager
{
public:
    EcTaskManager();

    ~EcTaskManager();

    uint32_t initTask();

    uint32_t preapareTask();

    uint32_t setupTask();

    uint32_t cyclicTask();

    uint32_t diagnosisTask();

    uint32_t notifyTask();

private:
    std::vector<EcTaskManagerNs::TaskGroup> m_taskGroupVector;
    std::vector<EcTaskManagerNs::TaskGroupData> m_taskGroupDataVector;

    EcTaskInterface m_taskInterface;
    EcTaskManagerNs::TaskInterfaceData m_taskInterfaceData;

private:
   XmlHelper &m_xmlHelper = XmlHelper::getInstance();
};

#endif // EC_TASK_MANAGER_H
