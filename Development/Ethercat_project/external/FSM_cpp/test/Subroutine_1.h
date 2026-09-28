// #ifndef SUBROUTINE_1_H
// #define SUBROUTINE_1_H

// #include <iostream>
// #include "SubroutineBase.h"

// class Subroutine_1 : public SubroutineBase
// {
// public:
//     Subroutine_1(uint32_t id, const std::string &name) : SubroutineBase(id, name)
//     {
//         std::cout << "Subroutine_1 constructor: " << "id: " << m_id << "name: " << m_name << std::endl;
//     }

//     ~Subroutine_1()
//     {
//         std::cout << "Subroutine_1 destructor: " << "id: " << m_id << " name: " << m_name << std::endl;
//     }

//     virtual uint32_t setContext(CtxtSubroutine* ctxtSubroutine) override
//     {
//         uint32_t dwRes = CallbackStatus::SUCCESS;

//         std::cout << "Subroutine_1 setContext: " << "id: " << m_id << " name: " << m_name << std::endl;

//         return dwRes;
//     }

//     virtual uint32_t registerPublisher() override
//     {
//         uint32_t dwRes = CallbackStatus::SUCCESS;

//         std::cout << "Subroutine_1 registerPublisher: " << "id: " << m_id << " name: " << m_name << std::endl;

//         return dwRes;
//     }

//     virtual uint32_t registerSubscriber() override
//     {
//         uint32_t dwRes = CallbackStatus::SUCCESS;

//         std::cout << "Subroutine_1 registerSubscriber: " << "id: " << m_id << " name: " << m_name << std::endl;

//         return dwRes;
//     }

//     virtual uint32_t config() override
//     {
//         uint32_t dwRes = CallbackStatus::SUCCESS;

//         std::cout << "Subroutine_1 config: " << "id: " << m_id << " name: " << m_name << std::endl;

//         return dwRes;
//     }

//     virtual uint32_t callback(CtxtCallback &ctxtCallback) override
//     {
//         uint32_t dwRes = CallbackStatus::SUCCESS;

//         std::cout << "Subroutine_1 callback: " << "id: " << m_id << " name: " << m_name << std::endl;

//         return dwRes;
//     }
// };

// #endif // SUBROUTINE_1_H
