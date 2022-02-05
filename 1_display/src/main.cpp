//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

#include <iostream>
#include "main.h"
#include "sysCmd.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include <cstring>
#include <future>

int main(int argc, char *argv[])
{
    if (argc == 2)     //Тестовый режим (Аргумент - номер экрана)
    {
        std::cout<<"Test mode\n";
        test(std::stoi(argv[1]));
        return 0;
    }
     std::cout<<"Work mode\n";

//     auto t1 = std::chrono::steady_clock::now();
//     auto t2 = std::chrono::steady_clock::now();
//     auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
//     while (int_ms.count() < 5000)

         auto v = allDevices();
         std::vector<std::thread> taskPing(v.size());
//         for (uint i = 0; i < v.size(); ++i)
//         {
//             pingDevice(v[i]);


         std::string result0;
         std::string result1;
         std::thread PingDevice0{pingDevice,v[0], std::ref(result0)};
         PingDevice0.detach();
         std::thread PingDevice1{pingDevice,v[1], std::ref(result1)};
         PingDevice1.detach();
//         }




             while(1)
             {
                 std::cout << result0 <<std::endl;
                 std::cout << result1 <<std::endl;
//                 auto f0 = std::async(std::launch::async, pingDevice, v[0]);
//                 auto f1 = std::async(std::launch::async, pingDevice, v[3]);
//                 std::cout <<f0.get();
//                 std::cout <<f1.get();
                 std::this_thread::sleep_for(std::chrono::seconds(2));
             }


//         display2(500);

//         t2 = std::chrono::steady_clock::now();
//         int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);


}
