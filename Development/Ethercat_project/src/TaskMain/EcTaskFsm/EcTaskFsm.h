#ifndef EC_TASK_FSM_H
#define EC_TASK_FSM_H

#include "Fsm.h"
#include "EcState.h"
#include <vector>
#include "EcTaskStateBase.h"
#include "EcTaskStateFactory.h"

class EcTaskFsm
{
public:
    EcTaskFsm();

    ~EcTaskFsm();

    uint32_t addAll(std::vector<EcTaskStateFactoryNs::TaskData>& taskData);

    uint32_t cleanup();

    uint32_t registerXml();
    
    uint32_t registerPublisher();

    uint32_t registerSubscriber();

    uint32_t setContext(CtxtSubroutine *ctxt);

    uint32_t config();

    uint32_t update();

private:
    std::vector<EcTaskStateBase *> m_stateVector;

    uint32_t addTaskState(EcTaskStateBase* pState);

    Fsm m_fsm;
};

#endif // EC_TASK_FSM_H
