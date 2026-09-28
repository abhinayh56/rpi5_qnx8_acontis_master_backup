#include "EcTaskStateFactory.h"

EcTaskStateFactory::EcTaskStateFactory()
{
}

EcTaskStateFactory::~EcTaskStateFactory()
{
}

EcTaskStateBase *EcTaskStateFactory::create(EcTaskStateFactoryNs::TaskData &taskData)
{
    EcTaskStateBase *newTask = nullptr;

    std::cout << "state name: " << taskData.taskType << std::endl;

    if (taskData.taskType == "STANDBY")
    {
        newTask = new EcStateStandby(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "DISABLING")
    {
        newTask = new EcStateDisabling(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "DISABLED")
    {
        newTask = new EcStateDisabled(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "ENABLING")
    {
        newTask = new EcStateEnabling(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "ENABLED")
    {
        newTask = new EcStateEnabled(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "READY")
    {
        newTask = new EcStateReady(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "JOYSTICK_CONTROL")
    {
        newTask = new EcStateJoystickControl(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "STOP")
    {
        newTask = new EcStateStop(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "FAULT")
    {
        newTask = new EcStateFault(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "CLEARING_FAULT")
    {
        newTask = new EcStateClearingFault(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "E_STOP")
    {
        newTask = new EcStateEStop(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "CLEARING_E_STOP")
    {
        newTask = new EcStateClearingEStop(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "QUICK_STOP")
    {
        newTask = new EcStateQuickStop(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "CLEARING_QUICK_STOP")
    {
        newTask = new EcStateClearingQuickStop(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else if (taskData.taskType == "STATE_X")
    {
        newTask = new EcStateX(taskData.taskId, taskData.taskName, taskData.taskPath);
    }
    else
    {
        newTask = nullptr;
    }

    if (newTask != nullptr)
    {
        newTask->setNumOnEntrySubroutine(taskData.numEntry);
        newTask->setNumOnExitSubroutine(taskData.numExit);
        newTask->setNumTransitionSubroutine(taskData.numTransition);
        newTask->setNumCallbackSubroutine(taskData.numCallback);

        for(uint32_t i = 0; i < taskData.numEntry; i++)
        {
            SubroutineBase *newSubTask = createSubroutine(taskData.typeEntry[i], taskData, i, "entry");
            newTask->addOnEntry(newSubTask);
        }

        for(uint32_t i = 0; i < taskData.numExit; i++)
        {
            SubroutineBase *newSubTask = createSubroutine(taskData.typeExit[i], taskData, i, "exit");
            newTask->addOnExit(newSubTask);
        }

        for(uint32_t i = 0; i < taskData.numTransition; i++)
        {
            SubroutineBase *newSubTask = createSubroutine(taskData.typeTransition[i], taskData, i, "transition");
            newTask->addTransition(newSubTask);
        }

        for(uint32_t i = 0; i < taskData.numCallback; i++)
        {
            SubroutineBase *newSubTask = createSubroutine(taskData.typeCallback[i], taskData, i, "callback");
            newTask->addCallback(newSubTask);
        }
    }

    return newTask;
}

SubroutineBase* EcTaskStateFactory::createSubroutine(std::string subTaskType, EcTaskStateFactoryNs::TaskData &taskData, std::uint32_t index, std::string callbackType)
{

    std::cout << "subroutine name: " << subTaskType << ", index : " << index << ", callbackType : " << callbackType << std::endl;
    
    SubroutineBase *newSubTask = nullptr;

    std::string subroutineType = subTaskType;
    uint32_t    id;
    std::string name;
    std::string xPath;

    if(callbackType == "entry")
    {
        id    = taskData.idEntry[index];
        name  = taskData.nameEntry[index];
        xPath = taskData.xPathEntry[index];
    }
    else if(callbackType == "exit")
    {
        id    = taskData.idExit[index];
        name  = taskData.nameExit[index];
        xPath = taskData.xPathExit[index];
    }
    else if(callbackType == "callback")
    {
        id    = taskData.idCallback[index];
        name  = taskData.nameCallback[index];
        xPath = taskData.xPathCallback[index];
    }
    else if(callbackType == "transition")
    {
        id    = taskData.idTransition[index];
        name  = taskData.nameTransition[index];
        xPath = taskData.xPathTransition[index];
    }

         if(subroutineType == "EcStateStandbyNs::OnEntrySubroutine")              {newSubTask = new EcStateStandbyNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateStandbyNs::OnExitSubroutine")               {newSubTask = new EcStateStandbyNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateStandbyNs::CallbackSubroutine")             {newSubTask = new EcStateStandbyNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateStandbyNs::TransitionSubroutine")           {newSubTask = new EcStateStandbyNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateDisablingNs::OnEntrySubroutine")            {newSubTask = new EcStateDisablingNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateDisablingNs::OnExitSubroutine")             {newSubTask = new EcStateDisablingNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateDisablingNs::CallbackSubroutine")           {newSubTask = new EcStateDisablingNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateDisablingNs::TransitionSubroutine")         {newSubTask = new EcStateDisablingNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateDisabledNs::OnEntrySubroutine")             {newSubTask = new EcStateDisabledNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateDisabledNs::OnExitSubroutine")              {newSubTask = new EcStateDisabledNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateDisabledNs::CallbackSubroutine")            {newSubTask = new EcStateDisabledNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateDisabledNs::TransitionSubroutine")          {newSubTask = new EcStateDisabledNs::TransitionSubroutine(id, name);}
    
    else if(subroutineType == "EcStateEnablingNs::OnEntrySubroutine")             {newSubTask = new EcStateEnablingNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateEnablingNs::OnExitSubroutine")              {newSubTask = new EcStateEnablingNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateEnablingNs::CallbackSubroutine")            {newSubTask = new EcStateEnablingNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateEnablingNs::TransitionSubroutine")          {newSubTask = new EcStateEnablingNs::TransitionSubroutine(id, name);}
    
    else if(subroutineType == "EcStateEnabledNs::OnEntrySubroutine")              {newSubTask = new EcStateEnabledNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateEnabledNs::OnExitSubroutine")               {newSubTask = new EcStateEnabledNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateEnabledNs::CallbackSubroutine")             {newSubTask = new EcStateEnabledNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateEnabledNs::TransitionSubroutine")           {newSubTask = new EcStateEnabledNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateReadyNs::OnEntrySubroutine")                {newSubTask = new EcStateReadyNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateReadyNs::OnExitSubroutine")                 {newSubTask = new EcStateReadyNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateReadyNs::CallbackSubroutine")               {newSubTask = new EcStateReadyNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateReadyNs::TransitionSubroutine")             {newSubTask = new EcStateReadyNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateJoystickControlNs::OnEntrySubroutine")      {newSubTask = new EcStateJoystickControlNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateJoystickControlNs::OnExitSubroutine")       {newSubTask = new EcStateJoystickControlNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateJoystickControlNs::CallbackSubroutine")     {newSubTask = new EcStateJoystickControlNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateJoystickControlNs::TransitionSubroutine")   {newSubTask = new EcStateJoystickControlNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateStopNs::OnEntrySubroutine")                 {newSubTask = new EcStateStopNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateStopNs::OnExitSubroutine")                  {newSubTask = new EcStateStopNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateStopNs::CallbackSubroutine")                {newSubTask = new EcStateStopNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateStopNs::TransitionSubroutine")              {newSubTask = new EcStateStopNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateFaultNs::OnEntrySubroutine")                {newSubTask = new EcStateFaultNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateFaultNs::OnExitSubroutine")                 {newSubTask = new EcStateFaultNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateFaultNs::CallbackSubroutine")               {newSubTask = new EcStateFaultNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateFaultNs::TransitionSubroutine")             {newSubTask = new EcStateFaultNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateClearingFaultNs::OnEntrySubroutine")        {newSubTask = new EcStateClearingFaultNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateClearingFaultNs::OnExitSubroutine")         {newSubTask = new EcStateClearingFaultNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingFaultNs::CallbackSubroutine")       {newSubTask = new EcStateClearingFaultNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingFaultNs::TransitionSubroutine")     {newSubTask = new EcStateClearingFaultNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateEStopNs::OnEntrySubroutine")                {newSubTask = new EcStateEStopNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateEStopNs::OnExitSubroutine")                 {newSubTask = new EcStateEStopNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateEStopNs::CallbackSubroutine")               {newSubTask = new EcStateEStopNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateEStopNs::TransitionSubroutine")             {newSubTask = new EcStateEStopNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateClearingEStopNs::OnEntrySubroutine")        {newSubTask = new EcStateClearingEStopNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateClearingEStopNs::OnExitSubroutine")         {newSubTask = new EcStateClearingEStopNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingEStopNs::CallbackSubroutine")       {newSubTask = new EcStateClearingEStopNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingEStopNs::TransitionSubroutine")     {newSubTask = new EcStateClearingEStopNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateQuickStopNs::OnEntrySubroutine")            {newSubTask = new EcStateQuickStopNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateQuickStopNs::OnExitSubroutine")             {newSubTask = new EcStateQuickStopNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateQuickStopNs::CallbackSubroutine")           {newSubTask = new EcStateQuickStopNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateQuickStopNs::TransitionSubroutine")         {newSubTask = new EcStateQuickStopNs::TransitionSubroutine(id, name);}
    
    else if(subroutineType == "EcStateClearingQuickStopNs::OnEntrySubroutine")    {newSubTask = new EcStateClearingQuickStopNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateClearingQuickStopNs::OnExitSubroutine")     {newSubTask = new EcStateClearingQuickStopNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingQuickStopNs::CallbackSubroutine")   {newSubTask = new EcStateClearingQuickStopNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateClearingQuickStopNs::TransitionSubroutine") {newSubTask = new EcStateClearingQuickStopNs::TransitionSubroutine(id, name);}

    else if(subroutineType == "EcStateXNs::OnEntrySubroutine")                    {newSubTask = new EcStateXNs::OnEntrySubroutine(id, name);}
    else if(subroutineType == "EcStateXNs::OnExitSubroutine")                     {newSubTask = new EcStateXNs::OnExitSubroutine(id, name);}
    else if(subroutineType == "EcStateXNs::CallbackSubroutine")                   {newSubTask = new EcStateXNs::CallbackSubroutine(id, name);}
    else if(subroutineType == "EcStateXNs::TransitionSubroutine")                 {newSubTask = new EcStateXNs::TransitionSubroutine(id, name);}
    else                                                                          {newSubTask = nullptr;}

    if(newSubTask != nullptr)
    {
        newSubTask->setXPath(xPath);
    }

    return newSubTask;
}
