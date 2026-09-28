#ifndef EC_TASK_STATE_FACTORY_H
#define EC_TASK_STATE_FACTORY_H

#include "EcStateStandby.h"
#include "EcStateDisabling.h"
#include "EcStateDisabled.h"
#include "EcStateEnabling.h"
#include "EcStateEnabled.h"
#include "EcStateReady.h"
#include "EcStateJoystickControl.h"
#include "EcStateStop.h"
#include "EcStateFault.h"
#include "EcStateClearingFault.h"
#include "EcStateEStop.h"
#include "EcStateClearingEStop.h"
#include "EcStateQuickStop.h"
#include "EcStateClearingQuickStop.h"
#include "EcStateX.h"

namespace EcTaskStateFactoryNs
{
    struct TaskData
    {
        std::string              taskType;
        uint32_t                 taskId;
        std::string              taskName;
        std::string              taskPath;
        uint32_t                 numEntry;
        uint32_t                 numExit;
        uint32_t                 numTransition;
        uint32_t                 numCallback;

        std::vector<std::string> typeEntry;
        std::vector<uint32_t>    idEntry;
        std::vector<std::string> nameEntry;
        std::vector<std::string> xPathEntry;

        std::vector<std::string> typeExit;
        std::vector<uint32_t>    idExit;
        std::vector<std::string> nameExit;
        std::vector<std::string> xPathExit;

        std::vector<std::string> typeTransition;
        std::vector<uint32_t>    idTransition;
        std::vector<std::string> nameTransition;
        std::vector<std::string> xPathTransition;

        std::vector<std::string> typeCallback;
        std::vector<uint32_t>    idCallback;
        std::vector<std::string> nameCallback;
        std::vector<std::string> xPathCallback;
    };
};

class EcTaskStateFactory
{
public:
    EcTaskStateFactory();

    ~EcTaskStateFactory();

    static EcTaskStateBase* create(EcTaskStateFactoryNs::TaskData &taskData);

    static SubroutineBase* createSubroutine(std::string subTaskType, EcTaskStateFactoryNs::TaskData &taskData, std::uint32_t index, std::string callbackType);
};

#endif // EC_TASK_STATE_FACTORY_H
