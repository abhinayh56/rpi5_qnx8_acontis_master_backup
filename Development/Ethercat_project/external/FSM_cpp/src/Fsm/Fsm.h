#ifndef FSM_H
#define FSM_H

#include "../StateBase/StateBase.h"
#include <vector>
#include <iostream>

struct CtxtSubroutine;

class Fsm
{
public:
    Fsm(uint32_t initialStateId);
    ~Fsm();

    uint32_t addState(StateBase* state);

    uint32_t setContext(CtxtSubroutine* ctxtSubroutine);
    uint32_t registerXml();
    uint32_t registerPublisher();
    uint32_t registerSubscriber();
    uint32_t config();
    uint32_t update();

    uint32_t getId() const;
    std::string getName() const;

protected:
    uint32_t m_id;
    std::string m_name;

private:
    uint32_t m_initialStateId;
    uint32_t m_currentStateId;
    uint32_t m_currentStateIndex;

    std::vector<StateBase*> m_stateVector;
};

#endif // FSM_H
