#include "Fsm.h"

Fsm::Fsm(uint32_t initialStateId) :
	m_id(0),
	m_name("None"),
    m_initialStateId(initialStateId),
    m_currentStateId(initialStateId),
    m_currentStateIndex(0)
{
    m_stateVector.reserve(20);
}

Fsm::~Fsm()
{
}

uint32_t Fsm::addState(StateBase* state)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_stateVector.push_back(state);    
    std::cout << "---\n";
    std::cout << "added state | id: " << m_stateVector[m_stateVector.size()-1]->getId() << ", name: " << m_stateVector[m_stateVector.size()-1]->getName() << std::endl;
    std::cout << "num states: " << m_stateVector.size() << std::endl;

    return dwRes;
}

uint32_t Fsm::setContext(CtxtSubroutine* ctxtSubroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        dwRes |= m_stateVector[i]->setContext(ctxtSubroutine);
    }

    return dwRes;
}

uint32_t Fsm::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        dwRes |= m_stateVector[i]->registerXml();
    }
    
    return dwRes;
}

uint32_t Fsm::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        dwRes |= m_stateVector[i]->registerPublisher();
    }
    
    return dwRes;
}

uint32_t Fsm::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        dwRes |= m_stateVector[i]->registerSubscriber();
    }

    return dwRes;
}

uint32_t Fsm::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "---\nconfiguring states" << std::endl;

    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        std::cout << "---\n";
        std::cout << "configuring state | id: " << m_stateVector[i]->getId() << ", name: " << m_stateVector[i]->getName() << std::endl;
        dwRes |= m_stateVector[i]->config();

        if(m_currentStateId == m_stateVector[i]->getId())
        {
            m_currentStateIndex = i;
        }
    }

    return dwRes;
}

uint32_t Fsm::update()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    dwRes |= m_stateVector[m_currentStateIndex]->update(m_currentStateId);

    for(uint32_t i=0; i<m_stateVector.size(); i++)
    {
        if((m_stateVector[i]->getId()) == m_currentStateId)
        {
            m_currentStateIndex = i;
            break;
        }
    }

    return dwRes;
}

uint32_t Fsm::getId() const
{
    return m_id;
}

std::string Fsm::getName() const
{
    return m_name;
}
