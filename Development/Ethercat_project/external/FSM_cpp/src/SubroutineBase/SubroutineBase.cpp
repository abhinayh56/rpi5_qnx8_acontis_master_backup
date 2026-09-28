#include "SubroutineBase.h"

SubroutineBase::SubroutineBase(uint32_t id, const std::string& name) : 
    XmlDataRegister(),
    m_id(id),
    m_name(name),
    p_ctxtSubroutine(nullptr)
{
    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " constructor" << std::endl;
}

SubroutineBase::~SubroutineBase()
{
    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " destructor" << std::endl;
}

uint32_t SubroutineBase::registerXml()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " Register xml " << std::endl;

    return dwRes;
}

uint32_t SubroutineBase::setContext(CtxtSubroutine* ctxtSubroutine)
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    p_ctxtSubroutine = ctxtSubroutine;

    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " Context set " << std::endl;

    return dwRes;
}

uint32_t SubroutineBase::registerPublisher()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " registerPublisher " << std::endl;

    return dwRes;
}

uint32_t SubroutineBase::registerSubscriber()
{
    uint32_t dwRes = CallbackStatus::SUCCESS;

    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " registerSubscriber. " << std::endl;

    return dwRes;
}

uint32_t SubroutineBase::config()
{
    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " config. " << std::endl;
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineBase::callback(CtxtCallback& ctxtCallback)
{
    std::cout << "SubroutineBase: " << "m_id: " << m_id << ", m_name: " << m_name << " callback. " << std::endl;
    return CallbackStatus::SUCCESS;
}

uint32_t SubroutineBase::getId() const
{
    return m_id;
}

std::string SubroutineBase::getName() const
{
    return m_name;
}
