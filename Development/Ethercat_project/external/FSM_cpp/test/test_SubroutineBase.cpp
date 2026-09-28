// #include <iostream>
// #include "SubroutineBase.h"

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
//     CtxtCallback ctxtCallback_2;
//     CtxtCallback ctxtCallback_3;
//     CtxtCallback ctxtCallback_4;
//     CtxtCallback ctxtCallback_5;

//     SubroutineBase subroutine_1(1, "Name_1");
//     SubroutineBase subroutine_2(2, "Name_2");
//     SubroutineBase subroutine_3(3, "Name_3");
//     SubroutineBase subroutine_4(4, "Name_4");
//     SubroutineBase subroutine_5(5, "Name_5");

//     subroutine_1.setContext(&ctxtSubroutine);
//     subroutine_2.setContext(&ctxtSubroutine);
//     subroutine_3.setContext(&ctxtSubroutine);
//     subroutine_4.setContext(&ctxtSubroutine);
//     subroutine_5.setContext(&ctxtSubroutine);

//     subroutine_1.registerPublisher();
//     subroutine_2.registerPublisher();
//     subroutine_3.registerPublisher();
//     subroutine_4.registerPublisher();
//     subroutine_5.registerPublisher();

//     subroutine_1.registerSubscriber();
//     subroutine_2.registerSubscriber();
//     subroutine_3.registerSubscriber();
//     subroutine_4.registerSubscriber();
//     subroutine_5.registerSubscriber();

//     subroutine_1.config();
//     subroutine_2.config();
//     subroutine_3.config();
//     subroutine_4.config();
//     subroutine_5.config();

//     subroutine_1.callback(ctxtCallback_1);
//     subroutine_2.callback(ctxtCallback_1);
//     subroutine_3.callback(ctxtCallback_1);
//     subroutine_4.callback(ctxtCallback_1);
//     subroutine_5.callback(ctxtCallback_1);

//     std::cout << "subroutine_1.getId(): " << subroutine_1.getId() << std::endl;
//     std::cout << "subroutine_2.getId(): " << subroutine_2.getId() << std::endl;
//     std::cout << "subroutine_3.getId(): " << subroutine_3.getId() << std::endl;
//     std::cout << "subroutine_4.getId(): " << subroutine_4.getId() << std::endl;
//     std::cout << "subroutine_5.getId(): " << subroutine_5.getId() << std::endl;

//     std::cout << "subroutine_1.getName(): " << subroutine_1.getName() << std::endl;
//     std::cout << "subroutine_2.getName(): " << subroutine_2.getName() << std::endl;
//     std::cout << "subroutine_3.getName(): " << subroutine_3.getName() << std::endl;
//     std::cout << "subroutine_4.getName(): " << subroutine_4.getName() << std::endl;
//     std::cout << "subroutine_5.getName(): " << subroutine_5.getName() << std::endl;

//     return 0;
// }
