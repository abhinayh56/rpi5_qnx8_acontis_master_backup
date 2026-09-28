#ifndef STATE_BASE_H
#define STATE_BASE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "../SubroutineBase/SubroutineBase.h"

struct CtxtCallback
{
    uint32_t idNextState;
};

class StateBase
{
public:
    StateBase(uint32_t id, const std::string &name);
    virtual ~StateBase();
    
    virtual uint32_t addOnEntry(SubroutineBase* subroutine);
    virtual uint32_t addTransition(SubroutineBase* subroutine);
    virtual uint32_t addCallback(SubroutineBase* subroutine);
    virtual uint32_t addOnExit(SubroutineBase* subroutine);

    virtual uint32_t setContext(CtxtSubroutine* ctxtSubroutine);
    uint32_t registerXml();
    virtual uint32_t registerPublisher();
    virtual uint32_t registerSubscriber();
    virtual uint32_t config();
    virtual uint32_t update(uint32_t &idNextState);

    virtual uint32_t getId() const;
    virtual std::string getName() const;

    virtual void setNumOnEntrySubroutine(uint32_t n);
    virtual void setNumOnExitSubroutine(uint32_t n);
    virtual void setNumTransitionSubroutine(uint32_t n);
    virtual void setNumCallbackSubroutine(uint32_t n);

protected:
    uint32_t m_id;
    std::string m_name;

private:
    std::vector<SubroutineBase*> m_onEntryVector;
    std::vector<SubroutineBase*> m_onExitVector;
    std::vector<SubroutineBase*> m_transitionVector;
    std::vector<SubroutineBase*> m_callbackVector;
    
    CtxtCallback m_ctxtCallback;
    bool m_flagEntry;
    bool m_flagTransition;
};

#endif // STATE_BASE_H
