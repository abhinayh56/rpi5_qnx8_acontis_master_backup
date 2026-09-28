#include "StateBase.h"

StateBase::StateBase(uint32_t id, const std::string &name) : 
	m_id(id),
    m_name(name),
    m_ctxtCallback({id}),
	m_flagEntry(true),
    m_flagTransition(false)
{
}

StateBase::~StateBase()
{
}

uint32_t StateBase::addOnEntry(SubroutineBase* subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onEntryVector.push_back(subroutine);

    return dwRes;
}

uint32_t StateBase::addTransition(SubroutineBase* subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_transitionVector.push_back(subroutine);

    return dwRes;
}

uint32_t StateBase::addCallback(SubroutineBase* subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_callbackVector.push_back(subroutine);

    return dwRes;
}

uint32_t StateBase::addOnExit(SubroutineBase* subroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    m_onExitVector.push_back(subroutine);

    return dwRes;
}

uint32_t StateBase::setContext(CtxtSubroutine* ctxtSubroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;
    
    for(uint32_t i=0; i<m_onEntryVector.size(); i++)
    {
        dwRes |= m_onEntryVector[i]->setContext(ctxtSubroutine);
    }
    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->setContext(ctxtSubroutine);
    }
    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->setContext(ctxtSubroutine);
    }
    for(uint32_t i=0; i<m_onExitVector.size(); i++)
    {
        dwRes |= m_onExitVector[i]->setContext(ctxtSubroutine);
    }

    return dwRes;
}

uint32_t StateBase::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_onEntryVector.size(); i++)
    {
        dwRes |= m_onEntryVector[i]->registerXml();
    }
    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->registerXml();
    }
    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->registerXml();
    }
    for(uint32_t i=0; i<m_onExitVector.size(); i++)
    {
        dwRes |= m_onExitVector[i]->registerXml();
    }
    
    return dwRes;
}

uint32_t StateBase::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_onEntryVector.size(); i++)
    {
        dwRes |= m_onEntryVector[i]->registerPublisher();
    }
    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->registerPublisher();
    }
    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->registerPublisher();
    }
    for(uint32_t i=0; i<m_onExitVector.size(); i++)
    {
        dwRes |= m_onExitVector[i]->registerPublisher();
    }

    return dwRes;
}

uint32_t StateBase::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_onEntryVector.size(); i++)
    {
        dwRes |= m_onEntryVector[i]->registerSubscriber();
    }
    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->registerSubscriber();
    }
    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->registerSubscriber();
    }
    for(uint32_t i=0; i<m_onExitVector.size(); i++)
    {
        dwRes |= m_onExitVector[i]->registerSubscriber();
    }

    return dwRes;
}

uint32_t StateBase::config()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    for(uint32_t i=0; i<m_onEntryVector.size(); i++)
    {
        dwRes |= m_onEntryVector[i]->config();
    }
    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->config();
    }
    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->config();
    }
    for(uint32_t i=0; i<m_onExitVector.size(); i++)
    {
        dwRes |= m_onExitVector[i]->config();
    }

    return dwRes;
}

uint32_t StateBase::update(uint32_t &nextStateId)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    if(m_flagEntry == true)
    {
        for(uint32_t i=0; i<m_onEntryVector.size(); i++)
        {
            dwRes |= m_onEntryVector[i]->callback(m_ctxtCallback);
            std::cout << "========> " << m_onEntryVector[i]->getName() << " | " << m_onEntryVector[i]->getXPath() << std::endl;
        }

        m_flagEntry = false;
    }

    for(uint32_t i=0; i<m_callbackVector.size(); i++)
    {
        dwRes |= m_callbackVector[i]->callback(m_ctxtCallback);
    }

    for(uint32_t i=0; i<m_transitionVector.size(); i++)
    {
        dwRes |= m_transitionVector[i]->callback(m_ctxtCallback);

        if(m_ctxtCallback.idNextState != m_id)
        {
            std::cout << "========> " << m_transitionVector[i]->getName() << " | " << m_onEntryVector[i]->getXPath() << std::endl;
            m_flagTransition = true;
            break;
        }
    }

    if(m_flagTransition == true)
    {
        for(uint32_t i=0; i < m_onExitVector.size(); i++)
        {
            dwRes |= m_onExitVector[i]->callback(m_ctxtCallback);
            std::cout << "========> " << m_onExitVector[i]->getName() << " | " << m_onEntryVector[i]->getXPath() << std::endl;
            std::cout << "---"<< std::endl;
        }
        nextStateId = m_ctxtCallback.idNextState;
        m_ctxtCallback.idNextState = m_id;
        m_flagTransition = false;
        m_flagEntry = true;
    }

    return dwRes;
}

uint32_t StateBase::getId() const
{
    return m_id;
}

std::string StateBase::getName() const
{
    return m_name;
}

void StateBase::setNumOnEntrySubroutine(uint32_t n)
{
    m_onEntryVector.reserve(n);
}

void StateBase::setNumOnExitSubroutine(uint32_t n)
{
    m_onExitVector.reserve(n);
}

void StateBase::setNumTransitionSubroutine(uint32_t n)
{
    m_transitionVector.reserve(n);
}

void StateBase::setNumCallbackSubroutine(uint32_t n)
{
    m_callbackVector.reserve(n);
}
