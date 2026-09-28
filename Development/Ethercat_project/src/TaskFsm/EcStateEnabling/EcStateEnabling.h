#ifndef EC_STATE_ENABLING_H
#define EC_STATE_ENABLING_H

#include "EcTaskStateBase.h"
#include "SubroutineBase.h"

class EcStateEnabling : public EcTaskStateBase
{
public:
    EcStateEnabling(uint32_t taskId ,const std::string& taskName, const std::string &taskPath);
    ~EcStateEnabling();
    virtual uint32_t addSubroutine() override;
};

namespace EcStateEnablingNs
{
    class OnEntrySubroutine : public SubroutineBase
    {
    public:
        OnEntrySubroutine(uint32_t id, const std::string &name);

        ~OnEntrySubroutine();

        uint32_t registerXml() override;

        uint32_t registerPublisher() override;
        
        uint32_t registerSubscriber() override;

        uint32_t config() override;

        uint32_t callback(CtxtCallback &ctxtCallback) override;
    };

    class OnExitSubroutine : public SubroutineBase
    {
    public:
        OnExitSubroutine(uint32_t id, const std::string &name);

        ~OnExitSubroutine();

        uint32_t registerXml() override;

        uint32_t registerPublisher() override;
        
        uint32_t registerSubscriber() override;

        uint32_t config() override;

        uint32_t callback(CtxtCallback &ctxtCallback) override;
    };

    class CallbackSubroutine : public SubroutineBase
    {
    public:
        CallbackSubroutine(uint32_t id, const std::string &name);

        ~CallbackSubroutine();

        uint32_t registerXml() override;

        uint32_t registerPublisher() override;
        
        uint32_t registerSubscriber() override;

        uint32_t config() override;

        uint32_t callback(CtxtCallback &ctxtCallback) override;
    };

    class TransitionSubroutine : public SubroutineBase
    {
    public:
        TransitionSubroutine(uint32_t id, const std::string &name);

        ~TransitionSubroutine();

        uint32_t registerXml() override;

        uint32_t registerPublisher() override;
        
        uint32_t registerSubscriber() override;

        uint32_t config() override;

        uint32_t callback(CtxtCallback &ctxtCallback) override;

    private:
        Data_store_element<bool> m_dseEStopTrig;
        Data_store_element<bool> m_dseEStopClear;
        Data_store_element<bool> m_dseDisableTrig;
        Data_store_element<bool> m_dseClearFaultTrig;
        Data_store_element<bool> m_dseClearQuickStopTrig;
        Data_store_element<bool> m_dseEnableTrig;
        Data_store_element<bool> m_dseReadyTrig;
        Data_store_element<bool> m_dseCustomStateTrig;
        Data_store_element<bool> m_dseStopTrig;

        bool m_eStopTrig = false;
        bool m_eStopClear = false;
        bool m_disableTrig = false;
        bool m_clearFaultTrig = false;
        bool m_clearQuickStopTrig = false;
        bool m_enableTrig = false;
        bool m_readyTrig = false;
        bool m_customStateTrig = false;
        bool m_stopTrig = false;
    };
}

#endif // EC_STATE_ENABLING_H
