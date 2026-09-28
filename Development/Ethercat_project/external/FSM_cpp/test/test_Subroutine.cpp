// #include <iostream>
// #include "SubroutineBase.h"
// #include "Subroutine_1.h"

// struct CtxtSubroutine
// {
// };

// struct CtxtCallback
// {
// };

// int main()
// {
//     CtxtSubroutine ctxtSubroutine;
//     CtxtCallback ctxtCallback_1;

//     SubroutineBase* subroutine_base;

//     Subroutine_1 subroutine_1_1(11, "name_1_1");
//     Subroutine_1 subroutine_1_2(12, "name_1_2");
//     Subroutine_1 subroutine_1_3(13, "name_1_3");
//     Subroutine_1 subroutine_1_4(14, "name_1_4");
//     Subroutine_1 subroutine_1_5(15, "name_1_5");

//     subroutine_1_1.setContext(&ctxtSubroutine);
//     subroutine_1_2.setContext(&ctxtSubroutine);
//     subroutine_1_3.setContext(&ctxtSubroutine);
//     subroutine_1_4.setContext(&ctxtSubroutine);
//     subroutine_1_5.setContext(&ctxtSubroutine);

//     subroutine_1_1.registerPublisher();
//     subroutine_1_2.registerPublisher();
//     subroutine_1_3.registerPublisher();
//     subroutine_1_4.registerPublisher();
//     subroutine_1_5.registerPublisher();

//     subroutine_1_1.registerSubscriber();
//     subroutine_1_2.registerSubscriber();
//     subroutine_1_3.registerSubscriber();
//     subroutine_1_4.registerSubscriber();
//     subroutine_1_5.registerSubscriber();

//     subroutine_1_1.config();
//     subroutine_1_2.config();
//     subroutine_1_3.config();
//     subroutine_1_4.config();
//     subroutine_1_5.config();

//     subroutine_1_1.callback(ctxtCallback_1);
//     subroutine_1_2.callback(ctxtCallback_1);
//     subroutine_1_3.callback(ctxtCallback_1);
//     subroutine_1_4.callback(ctxtCallback_1);
//     subroutine_1_5.callback(ctxtCallback_1);

//     std::cout << "subroutine_1_1.getId(): " <<  subroutine_1_1.getId() << std::endl;
//     std::cout << "subroutine_1_2.getId(): " <<  subroutine_1_2.getId() << std::endl;
//     std::cout << "subroutine_1_3.getId(): " <<  subroutine_1_3.getId() << std::endl;
//     std::cout << "subroutine_1_4.getId(): " <<  subroutine_1_4.getId() << std::endl;
//     std::cout << "subroutine_1_5.getId(): " <<  subroutine_1_5.getId() << std::endl;

//     std::cout << "subroutine_1_1.getName(): " <<  subroutine_1_1.getName() << std::endl;
//     std::cout << "subroutine_1_2.getName(): " <<  subroutine_1_2.getName() << std::endl;
//     std::cout << "subroutine_1_3.getName(): " <<  subroutine_1_3.getName() << std::endl;
//     std::cout << "subroutine_1_4.getName(): " <<  subroutine_1_4.getName() << std::endl;
//     std::cout << "subroutine_1_5.getName(): " <<  subroutine_1_5.getName() << std::endl;


//     return 0;
// }
