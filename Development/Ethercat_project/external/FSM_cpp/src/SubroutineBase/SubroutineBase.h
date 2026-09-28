#ifndef SUBROUTINE_BASE_H
#define SUBROUTINE_BASE_H

#include <string>
#include <stdint.h>
#include <iostream>
#include "CallbackStatus.h"
#include "xmlDataRegister.h"

struct CtxtSubroutine;
struct CtxtCallback;

class SubroutineBase : public XmlDataRegister
{
public:
    SubroutineBase(uint32_t id, const std::string &name);
    virtual ~SubroutineBase();
    virtual uint32_t registerXml() override;
    virtual uint32_t setContext(CtxtSubroutine* ctxtSubroutine);
    virtual uint32_t registerPublisher();
    virtual uint32_t registerSubscriber();
    virtual uint32_t config();
    virtual uint32_t callback(CtxtCallback &ctxtCallback);

    uint32_t getId() const;
    std::string getName() const;

protected:
    uint32_t m_id;
    std::string m_name;
    CtxtSubroutine* p_ctxtSubroutine;
};

#endif // SUBROUTINE_BASE_H
