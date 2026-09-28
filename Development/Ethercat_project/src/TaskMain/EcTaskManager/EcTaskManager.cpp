#include "EcTaskManager.h"

EcTaskManager::EcTaskManager()
{
    m_xmlHelper.loadFile("config.xml");

    std::string rootXPath      = m_xmlHelper.searchRoot("/config_data");
    std::cout << "rootXPath     : " << rootXPath << std::endl;
    std::string taskGroupXPath = m_xmlHelper.gotoNode(rootXPath, "taskGroup", 0);
    std::cout << "taskGroupXPath: " << taskGroupXPath << std::endl;
    
    std::string shmRootXPath       = m_xmlHelper.gotoNode(rootXPath, "shm", 0);
    std::cout << "shmRootXPath  : " << shmRootXPath << std::endl;
    int numShm = m_xmlHelper.getNumElement(shmRootXPath, "element");

    std::cout << "numShm        : " << numShm << std::endl;    
    m_taskInterfaceData.shmNameVector.resize(numShm);
    m_taskInterfaceData.shmSizeVector.resize(numShm);
    m_taskInterfaceData.shmLayoutXpathVector.resize(numShm);
    m_taskInterfaceData.shmStoreVector.resize(numShm);

    for(size_t i = 0; i < m_taskInterfaceData.shmStoreVector.size(); i++)
    {
        std::string shmIName;
        uint64_t    shmISize;
        std::string shmIXpath = m_xmlHelper.gotoNode(shmRootXPath, "element", i);
        std::cout << "shmIXpath     : " << shmIXpath << std::endl;
        std::string shmILayoutXpath = m_xmlHelper.gotoNode(shmIXpath, "layout", 0);

        m_xmlHelper.registerElementValue(shmIXpath, "name", shmIName);
        m_xmlHelper.registerElementValue(shmIXpath, "size", shmISize);
        std::cout << "---" << std::endl;
        std::cout << "shmIName        : " << shmIName         << std::endl;
        std::cout << "shmISize        : " << shmISize         << std::endl;
        std::cout << "shmILayoutXpath : " << shmILayoutXpath  << std::endl;
        std::cout << "---" << std::endl;

        m_taskInterfaceData.shmNameVector[i]        = shmIName;
        m_taskInterfaceData.shmSizeVector[i]        = shmISize;
        m_taskInterfaceData.shmLayoutXpathVector[i] = shmILayoutXpath;

        m_taskInterfaceData.shmStoreVector[i].create(shmIName, shmISize);
    }

    int numTaskGroups = m_xmlHelper.getNumElement(taskGroupXPath, "element");
    std::cout << "numTaskGroups : " << numTaskGroups << std::endl;

    m_taskGroupVector.resize(numTaskGroups);
    m_taskGroupDataVector.resize(numTaskGroups);

    std::cout << "---" << std::endl;

    for (int i = 0; i < numTaskGroups; i++)
    {
        {   // task ethercat slave
            std::string nodePath = "";
            nodePath = m_xmlHelper.gotoNode(taskGroupXPath, "element", i);
            nodePath = m_xmlHelper.gotoNode(nodePath, "taskEthercatSlave", 0);
            int numTaskEthercatSlave = m_xmlHelper.getNumElement(nodePath, "element");
            
            std::cout << "nodePath: " << nodePath << std::endl;
            std::cout << "numTaskEthercatSlave: " << numTaskEthercatSlave << std::endl;
            m_taskGroupDataVector[i].taskEthercatSlaveDataVector.resize(numTaskEthercatSlave);

            for (int j = 0; j < numTaskEthercatSlave; j++)
            {
                std::string taskType = "";
                uint32_t taskId = 0;
                std::string taskName = "";
                std::string taskPath = "";
                uint16_t slaveAddress = 0;
                std::string slaveName = "";

                std::string taskNodePath = m_xmlHelper.gotoNode(nodePath, "element", j);
                std::cout << "taskNodePath : " << taskNodePath << std::endl;

                m_xmlHelper.registerAttributeValue(taskNodePath, "taskType", taskType);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskId", taskId);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskName", taskName);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskPath", taskPath);
                m_xmlHelper.registerElementValue(taskNodePath, "slaveAddress", slaveAddress);
                m_xmlHelper.registerElementValue(taskNodePath, "slaveName", slaveName);

                std::cout << "taskType     : " << taskType << std::endl;
                std::cout << "taskId       : " << taskId << std::endl;
                std::cout << "taskName     : " << taskName << std::endl;
                std::cout << "taskPath     : " << taskPath << std::endl;
                std::cout << "slaveAddress : " << slaveAddress << std::endl;
                std::cout << "slaveName    : " << slaveName << std::endl;
                std::cout << "-" << std::endl;

                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].taskType = taskType;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].taskId = taskId;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].taskName = taskName;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].taskPath = taskPath;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].slaveAddress = slaveAddress;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].slaveName = slaveName;
                m_taskGroupDataVector[i].taskEthercatSlaveDataVector[j].xPath = taskNodePath;
            }
        }

        {   // task ethercat slave servo
            std::string nodePath = "";
            nodePath = m_xmlHelper.gotoNode(taskGroupXPath, "element", i);
            nodePath = m_xmlHelper.gotoNode(nodePath, "taskEthercatSlaveServo", 0);
            int numTaskEthercatSlaveServo = m_xmlHelper.getNumElement(nodePath, "element");
            std::cout << "nodePath: " << nodePath << std::endl;
            std::cout << "numTaskEthercatSlaveServo: " << numTaskEthercatSlaveServo << std::endl;
            m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector.resize(numTaskEthercatSlaveServo);

            for (int j = 0; j < numTaskEthercatSlaveServo; j++)
            {
                std::string taskType = "";
                uint32_t taskId = 0;
                std::string taskName = "";
                std::string taskPath = "";
                uint16_t slaveAddress = 0;
                std::string slaveName = "";
                int32_t offsetPosition = 0;
                int32_t offsetVelocity = 0;
                int16_t offsetTorque = 0;
                double factorPosition = 0.0;
                double factorVelocity = 0.0;
                double factorTorque = 0.0;

                std::string taskNodePath = m_xmlHelper.gotoNode(nodePath, "element", j);
                std::cout << "taskNodePath : " << taskNodePath << std::endl;

                m_xmlHelper.registerAttributeValue(taskNodePath, "taskType", taskType);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskId", taskId);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskName", taskName);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskPath", taskPath);
                m_xmlHelper.registerElementValue(taskNodePath, "slaveAddress", slaveAddress);
                m_xmlHelper.registerElementValue(taskNodePath, "slaveName", slaveName);
                m_xmlHelper.registerElementValue(taskNodePath, "offsetPosition", offsetPosition);
                m_xmlHelper.registerElementValue(taskNodePath, "offsetVelocity", offsetVelocity);
                m_xmlHelper.registerElementValue(taskNodePath, "offsetTorque", offsetTorque);
                m_xmlHelper.registerElementValue(taskNodePath, "factorPosition", factorPosition);
                m_xmlHelper.registerElementValue(taskNodePath, "factorVelocity", factorVelocity);
                m_xmlHelper.registerElementValue(taskNodePath, "factorTorque", factorTorque);

                std::cout << "taskType       : " << taskType << std::endl;
                std::cout << "taskId         : " << taskId << std::endl;
                std::cout << "taskName       : " << taskName << std::endl;
                std::cout << "taskPath       : " << taskPath << std::endl;
                std::cout << "slaveAddress   : " << slaveAddress << std::endl;
                std::cout << "slaveName      : " << slaveName << std::endl;
                std::cout << "offsetPosition : " << offsetPosition << std::endl;
                std::cout << "offsetVelocity : " << offsetVelocity << std::endl;
                std::cout << "offsetTorque   : " << offsetTorque << std::endl;
                std::cout << "factorPosition : " << factorPosition << std::endl;
                std::cout << "factorVelocity : " << factorVelocity << std::endl;
                std::cout << "factorTorque   : " << factorTorque << std::endl;
                std::cout << "-" << std::endl;

                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].taskType = taskType;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].taskId = taskId;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].taskName = taskName;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].taskPath = taskPath;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].slaveAddress = slaveAddress;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].slaveName = slaveName;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].xPath = taskNodePath;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].offsetPosition = offsetPosition;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].offsetVelocity = offsetVelocity;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].offsetTorque = offsetTorque;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].factorPosition = factorPosition;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].factorVelocity = factorVelocity;
                m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector[j].factorTorque = factorTorque;
            }
        }

        {   // task robot control
            std::string nodePath = "";
            nodePath = m_xmlHelper.gotoNode(taskGroupXPath, "element", i);
            nodePath = m_xmlHelper.gotoNode(nodePath, "taskRobotControl", 0);
            int numTaskRobotControl = m_xmlHelper.getNumElement(nodePath, "element");
            std::cout << "nodePath: " << nodePath << std::endl;
            std::cout << "numTaskRobotControl: " << numTaskRobotControl << std::endl;
            m_taskGroupDataVector[i].taskRobotControlDataVector.resize(numTaskRobotControl);

            for (int j = 0; j < numTaskRobotControl; j++)
            {
                std::string taskType = "";
                uint32_t taskId = 0;
                std::string taskName = "";
                std::string taskPath = "";

                std::string taskNodePath = m_xmlHelper.gotoNode(nodePath, "element", j);
                std::cout << "taskNodePath : " << taskNodePath << std::endl;

                m_xmlHelper.registerAttributeValue(taskNodePath, "taskType", taskType);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskId", taskId);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskName", taskName);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskPath", taskPath);

                std::cout << "taskType       : " << taskType << std::endl;
                std::cout << "taskId         : " << taskId << std::endl;
                std::cout << "taskName       : " << taskName << std::endl;
                std::cout << "taskPath       : " << taskPath << std::endl;
                std::cout << "-" << std::endl;

                m_taskGroupDataVector[i].taskRobotControlDataVector[j].taskType = taskType;
                m_taskGroupDataVector[i].taskRobotControlDataVector[j].taskId = taskId;
                m_taskGroupDataVector[i].taskRobotControlDataVector[j].taskName = taskName;
                m_taskGroupDataVector[i].taskRobotControlDataVector[j].taskPath = taskPath;
                m_taskGroupDataVector[i].taskRobotControlDataVector[j].xPath = taskNodePath;
            }
        }

        {   // task user
            std::string nodePath = "";
            nodePath = m_xmlHelper.gotoNode(taskGroupXPath, "element", i);
            nodePath = m_xmlHelper.gotoNode(nodePath, "taskUser", 0);
            int numTaskUser = m_xmlHelper.getNumElement(nodePath, "element");
            std::cout << "nodePath: " << nodePath << std::endl;
            std::cout << "numTaskUser: " << numTaskUser << std::endl;
            m_taskGroupDataVector[i].taskUserDataVector.resize(numTaskUser);

            for (int j = 0; j < numTaskUser; j++)
            {
                std::string taskType = "";
                uint32_t taskId = 0;
                std::string taskName = "";
                std::string taskPath = "";

                std::string taskNodePath = m_xmlHelper.gotoNode(nodePath, "element", j);
                std::cout << "taskNodePath : " << taskNodePath << std::endl;

                m_xmlHelper.registerAttributeValue(taskNodePath, "taskType", taskType);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskId", taskId);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskName", taskName);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskPath", taskPath);

                std::cout << "taskType       : " << taskType << std::endl;
                std::cout << "taskId         : " << taskId << std::endl;
                std::cout << "taskName       : " << taskName << std::endl;
                std::cout << "taskPath       : " << taskPath << std::endl;
                std::cout << "-" << std::endl;

                m_taskGroupDataVector[i].taskUserDataVector[j].taskType = taskType;
                m_taskGroupDataVector[i].taskUserDataVector[j].taskId = taskId;
                m_taskGroupDataVector[i].taskUserDataVector[j].taskName = taskName;
                m_taskGroupDataVector[i].taskUserDataVector[j].taskPath = taskPath;
                m_taskGroupDataVector[i].taskUserDataVector[j].xPath = taskNodePath;
            }
        }

        {   // task fsm
            std::string nodePath = "";
            nodePath = m_xmlHelper.gotoNode(taskGroupXPath, "element", i);
            nodePath = m_xmlHelper.gotoNode(nodePath, "taskState", 0);
            int numTaskState = m_xmlHelper.getNumElement(nodePath, "element");
            std::cout << "nodePath: " << nodePath << std::endl;
            std::cout << "numTaskState: " << numTaskState << std::endl;
            m_taskGroupDataVector[i].taskStateDataVector.resize(numTaskState);

            for (int j = 0; j < numTaskState; j++)
            {
                std::string taskType = "";
                uint32_t taskId = 0;
                std::string taskName = "";
                std::string taskPath = "";

                std::string taskNodePath = m_xmlHelper.gotoNode(nodePath, "element", j);
                std::cout << "taskNodePath : " << taskNodePath << std::endl;

                m_xmlHelper.registerAttributeValue(taskNodePath, "taskType", taskType);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskId", taskId);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskName", taskName);
                m_xmlHelper.registerAttributeValue(taskNodePath, "taskPath", taskPath);

                std::cout << "taskType       : " << taskType << std::endl;
                std::cout << "taskId         : " << taskId << std::endl;
                std::cout << "taskName       : " << taskName << std::endl;
                std::cout << "taskPath       : " << taskPath << std::endl;
                std::cout << "-" << std::endl;

                std::string entryNodePath = m_xmlHelper.gotoNode(taskNodePath, "entry", 0);
                std::string exitNodePath = m_xmlHelper.gotoNode(taskNodePath, "exit", 0);
                std::string transitionNodePath = m_xmlHelper.gotoNode(taskNodePath, "transition", 0);
                std::string callbackNodePath = m_xmlHelper.gotoNode(taskNodePath, "callback", 0);

                std::cout << "entryNodePath     : " << entryNodePath << std::endl;
                std::cout << "exitNodePath      : " << exitNodePath << std::endl;
                std::cout << "transitionNodePath: " << transitionNodePath << std::endl;
                std::cout << "callbackNodePath  : " << callbackNodePath << std::endl;
                
                uint32_t numEntry = m_xmlHelper.getNumElement(entryNodePath, "element");
                uint32_t numExit = m_xmlHelper.getNumElement(exitNodePath, "element");
                uint32_t numTransition = m_xmlHelper.getNumElement(transitionNodePath, "element");
                uint32_t numCallback = m_xmlHelper.getNumElement(callbackNodePath, "element");

                m_taskGroupDataVector[i].taskStateDataVector[j].taskType = taskType;
                m_taskGroupDataVector[i].taskStateDataVector[j].taskId = taskId;
                m_taskGroupDataVector[i].taskStateDataVector[j].taskName = taskName;
                m_taskGroupDataVector[i].taskStateDataVector[j].taskPath = taskPath;
                m_taskGroupDataVector[i].taskStateDataVector[j].numEntry = numEntry;
                m_taskGroupDataVector[i].taskStateDataVector[j].numExit = numExit;
                m_taskGroupDataVector[i].taskStateDataVector[j].numTransition = numTransition;
                m_taskGroupDataVector[i].taskStateDataVector[j].numCallback = numCallback;

                for(uint32_t k = 0; k < numEntry; k++)
                {
                    std::string currentSubroutinePath = m_xmlHelper.gotoNode(entryNodePath, "element", k);
                    std::string type;
                    uint32_t    id;
                    std::string name;
                    std::string path;

                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskType", type);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskId", id);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskName", name);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskPath", path);

                    m_taskGroupDataVector[i].taskStateDataVector[j].typeEntry.push_back(type);
                    m_taskGroupDataVector[i].taskStateDataVector[j].idEntry.push_back(id);
                    m_taskGroupDataVector[i].taskStateDataVector[j].nameEntry.push_back(name);
                    m_taskGroupDataVector[i].taskStateDataVector[j].xPathEntry.push_back(currentSubroutinePath);
                }

                for(uint32_t k = 0; k < numExit; k++)
                {
                    std::string currentSubroutinePath = m_xmlHelper.gotoNode(exitNodePath, "element", k);
                    std::string type;
                    uint32_t    id;
                    std::string name;
                    std::string path;

                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskType", type);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskId", id);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskName", name);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskPath", path);

                    m_taskGroupDataVector[i].taskStateDataVector[j].typeExit.push_back(type);
                    m_taskGroupDataVector[i].taskStateDataVector[j].idExit.push_back(id);
                    m_taskGroupDataVector[i].taskStateDataVector[j].nameExit.push_back(name);
                    m_taskGroupDataVector[i].taskStateDataVector[j].xPathExit.push_back(currentSubroutinePath);
                }

                for(uint32_t k = 0; k < numTransition; k++)
                {
                    std::string currentSubroutinePath = m_xmlHelper.gotoNode(transitionNodePath, "element", k);
                    std::string type;
                    uint32_t    id;
                    std::string name;
                    std::string path;

                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskType", type);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskId", id);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskName", name);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskPath", path);

                    m_taskGroupDataVector[i].taskStateDataVector[j].typeTransition.push_back(type);
                    m_taskGroupDataVector[i].taskStateDataVector[j].idTransition.push_back(id);
                    m_taskGroupDataVector[i].taskStateDataVector[j].nameTransition.push_back(name);
                    m_taskGroupDataVector[i].taskStateDataVector[j].xPathTransition.push_back(currentSubroutinePath);
                }

                for(uint32_t k = 0; k < numCallback; k++)
                {
                    std::string currentSubroutinePath = m_xmlHelper.gotoNode(callbackNodePath, "element", k);
                    std::string type;
                    uint32_t    id;
                    std::string name;
                    std::string path;

                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskType", type);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskId", id);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskName", name);
                    m_xmlHelper.registerAttributeValue(currentSubroutinePath, "taskPath", path);

                    m_taskGroupDataVector[i].taskStateDataVector[j].typeCallback.push_back(type);
                    m_taskGroupDataVector[i].taskStateDataVector[j].idCallback.push_back(id);
                    m_taskGroupDataVector[i].taskStateDataVector[j].nameCallback.push_back(name);
                    m_taskGroupDataVector[i].taskStateDataVector[j].xPathCallback.push_back(currentSubroutinePath);
                }
                
                // m_taskGroupDataVector[i].taskStateDataVector[j].xPath = taskNodePath; // check it
            }
        }

        std::cout << "======================FSM TEST START=========================" << std::endl;
        
        std::cout << "num task group : " << m_taskGroupDataVector.size() << std::endl;
        for(uint32_t i = 0; i < m_taskGroupDataVector.size(); i++)
        {
            std::cout << "----------" << std::endl;
            std::cout << "group index : " << i << std::endl;
            for(uint32_t j = 0; j < m_taskGroupDataVector[i].taskStateDataVector.size(); j++)
            {
                std::cout << "\n---" << std::endl;
                std::cout << "state index  : " << j << std::endl;
                
                std::cout << "-" << std::endl;
                
                std::cout << "taskType     :" << m_taskGroupDataVector[i].taskStateDataVector[j].taskType      << std::endl;
                std::cout << "taskId       :" << m_taskGroupDataVector[i].taskStateDataVector[j].taskId        << std::endl;
                std::cout << "taskName     :" << m_taskGroupDataVector[i].taskStateDataVector[j].taskName      << std::endl;
                std::cout << "taskPath     :" << m_taskGroupDataVector[i].taskStateDataVector[j].taskPath      << std::endl;
                std::cout << "numEntry     :" << m_taskGroupDataVector[i].taskStateDataVector[j].numEntry      << std::endl;
                std::cout << "numExit      :" << m_taskGroupDataVector[i].taskStateDataVector[j].numExit       << std::endl;
                std::cout << "numTransition:" << m_taskGroupDataVector[i].taskStateDataVector[j].numTransition << std::endl;
                std::cout << "numCallback  :" << m_taskGroupDataVector[i].taskStateDataVector[j].numCallback   << std::endl;
                
                std::cout << "-" << std::endl;
                std::cout << "numEntry     :" << m_taskGroupDataVector[i].taskStateDataVector[j].numEntry      << std::endl;
                for(uint32_t k = 0; k <  m_taskGroupDataVector[i].taskStateDataVector[j].numEntry; k++)
                {
                    std::cout << "entry index: " << k << std::endl;
                    std::cout << "entry type : " << m_taskGroupDataVector[i].taskStateDataVector[j].typeEntry[k]  << std::endl;
                    std::cout << "entry id   : " << m_taskGroupDataVector[i].taskStateDataVector[j].idEntry[k]    << std::endl;
                    std::cout << "entry name : " << m_taskGroupDataVector[i].taskStateDataVector[j].nameEntry[k]  << std::endl;
                    std::cout << "entry xPath: " << m_taskGroupDataVector[i].taskStateDataVector[j].xPathEntry[k] << std::endl;
                }

                std::cout << "-" << std::endl;
                std::cout << "numExit      :" << m_taskGroupDataVector[i].taskStateDataVector[j].numExit       << std::endl;
                for(uint32_t k = 0; k <  m_taskGroupDataVector[i].taskStateDataVector[j].numExit; k++)
                {
                    std::cout << "exit index: " << k << std::endl;
                    std::cout << "exit type : " << m_taskGroupDataVector[i].taskStateDataVector[j].typeExit[k]  << std::endl;
                    std::cout << "exit id   : " << m_taskGroupDataVector[i].taskStateDataVector[j].idExit[k]    << std::endl;
                    std::cout << "exit name : " << m_taskGroupDataVector[i].taskStateDataVector[j].nameExit[k]  << std::endl;
                    std::cout << "exit xPath: " << m_taskGroupDataVector[i].taskStateDataVector[j].xPathExit[k] << std::endl;
                }

                std::cout << "-" << std::endl;
                std::cout << "numTransition:" << m_taskGroupDataVector[i].taskStateDataVector[j].numTransition << std::endl;
                for(uint32_t k = 0; k <  m_taskGroupDataVector[i].taskStateDataVector[j].numTransition; k++)
                {
                    std::cout << "transition index: " << k << std::endl;
                    std::cout << "transition type : " << m_taskGroupDataVector[i].taskStateDataVector[j].typeTransition[k]  << std::endl;
                    std::cout << "transition id   : " << m_taskGroupDataVector[i].taskStateDataVector[j].idTransition[k]    << std::endl;
                    std::cout << "transition name : " << m_taskGroupDataVector[i].taskStateDataVector[j].nameTransition[k]  << std::endl;
                    std::cout << "transition xPath: " << m_taskGroupDataVector[i].taskStateDataVector[j].xPathTransition[k] << std::endl;
                }

                std::cout << "-" << std::endl;
                std::cout << "numCallback  :" << m_taskGroupDataVector[i].taskStateDataVector[j].numCallback   << std::endl;
                for(uint32_t k = 0; k <  m_taskGroupDataVector[i].taskStateDataVector[j].numCallback; k++)
                {
                    std::cout << "callback index: " << k << std::endl;
                    std::cout << "callback type : " << m_taskGroupDataVector[i].taskStateDataVector[j].typeCallback[k]  << std::endl;
                    std::cout << "callback id   : " << m_taskGroupDataVector[i].taskStateDataVector[j].idCallback[k]    << std::endl;
                    std::cout << "callback name : " << m_taskGroupDataVector[i].taskStateDataVector[j].nameCallback[k]  << std::endl;
                    std::cout << "callback xPath: " << m_taskGroupDataVector[i].taskStateDataVector[j].xPathCallback[k] << std::endl;
                }
                std::cout << "-" << std::endl;
            }
        }
        
        std::cout << "======================FSM TEST STOP=========================" << std::endl;

        std::cout << "---" << std::endl;
    }
}

EcTaskManager::~EcTaskManager()
{
}

uint32_t EcTaskManager::initTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    dwRes |= m_taskInterface.addAll(m_taskInterfaceData.shmStoreVector, m_taskInterfaceData.shmLayoutXpathVector);

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.addAll(m_taskGroupDataVector[i].taskEthercatSlaveDataVector);
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.addAll(m_taskGroupDataVector[i].taskEthercatSlaveServoDataVector);
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.addAll(m_taskGroupDataVector[i].taskRobotControlDataVector);
        dwRes |= m_taskGroupVector[i].ecTaskUser.addAll(m_taskGroupDataVector[i].taskUserDataVector);
        dwRes |= m_taskGroupVector[i].ecTaskFsm.addAll(m_taskGroupDataVector[i].taskStateDataVector);
    }

    return dwRes;
}

uint32_t EcTaskManager::preapareTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    dwRes |= m_taskInterface.registerXml();

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.registerXml();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.registerXml();
        dwRes |= m_taskGroupVector[i].ecTaskUser.registerXml();
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.registerXml();
        dwRes |= m_taskGroupVector[i].ecTaskFsm.registerXml();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.checkSlave();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.checkSlave();

        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.registerPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.registerPdo();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.registerPublisher();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.registerPublisher();
        dwRes |= m_taskGroupVector[i].ecTaskUser.registerPublisher();
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.registerPublisher();
        dwRes |= m_taskGroupVector[i].ecTaskFsm.registerPublisher();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.registerSubscriber();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.registerSubscriber();
        dwRes |= m_taskGroupVector[i].ecTaskUser.registerSubscriber();
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.registerSubscriber();
        dwRes |= m_taskGroupVector[i].ecTaskFsm.registerSubscriber();
    }

    dwRes |= m_taskInterface.registerSubscriber();
    dwRes |= m_taskInterface.registerPublisher();
    dwRes |= m_taskInterface.registerAllShm();

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        m_taskGroupVector[i].m_ctxt.p_ecTaskEthercatSlave = &m_taskGroupVector[i].ecTaskEthercatSlave;
        m_taskGroupVector[i].m_ctxt.p_ecTaskEthercatSlaveServo = &m_taskGroupVector[i].ecTaskEthercatSlaveServo;
        m_taskGroupVector[i].m_ctxt.p_ecTaskRobotControl = &m_taskGroupVector[i].ecTaskRobotControl;
        m_taskGroupVector[i].m_ctxt.p_ecTaskUser = &m_taskGroupVector[i].ecTaskUser;

        dwRes |= m_taskGroupVector[i].ecTaskFsm.setContext(&m_taskGroupVector[i].m_ctxt);
        dwRes |= m_taskGroupVector[i].ecTaskFsm.config();
    }

    return dwRes;
}

uint32_t EcTaskManager::setupTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskManager::cyclicTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    dwRes |= m_taskInterface.subscribeDataShm();
    dwRes |= m_taskInterface.publishData();

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.transferTxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.transferTxPdo();

        //	m_taskGroupVector[i].ec.TaskEthercatSlave.dispTxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.processTxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.processTxPdo();

        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.publishData();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.publishData();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskUser.subscribeData();
        dwRes |= m_taskGroupVector[i].ecTaskUser.mainProcess();
        dwRes |= m_taskGroupVector[i].ecTaskUser.publishData();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskFsm.update();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.subscribeData();
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.mainProcess();
        dwRes |= m_taskGroupVector[i].ecTaskRobotControl.publishData();
    }

    for (size_t i = 0; i < m_taskGroupVector.size(); i++)
    {
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.subscribeData();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.subscribeData();

        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.mainProcess();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.mainProcess();

        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.processRxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.processRxPdo();

        //	m_taskGroupVector[i].ecTaskEthercatSlave.dispRxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlave.transferRxPdo();
        dwRes |= m_taskGroupVector[i].ecTaskEthercatSlaveServo.transferRxPdo();
    }

    dwRes |= m_taskInterface.subscribeData();
    dwRes |= m_taskInterface.publishDataShm();

    return dwRes;
}

uint32_t EcTaskManager::diagnosisTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}

uint32_t EcTaskManager::notifyTask()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    return dwRes;
}
