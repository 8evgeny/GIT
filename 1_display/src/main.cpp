//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

#include <iostream>
#include "main.h"
#include "sysCmd.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include <cstring>

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
     while (1)
     {
         auto v = allDevices();
         for (auto &i:v)
         {
             std::cout << "device "<< i <<" ping\n";
             if (strstr(sysCdm("ping -c 3 -f -i 0,2 -n " + i + " | grep \" 0% packet loss\"").c_str(),
                        "0% packet loss"))
             {
                 std::cout << "device "<< i <<" available\n";
             }
             else
             {
                 std::cout << "device "<< i <<" unavailable\n";
             }
             std::this_thread::sleep_for(std::chrono::seconds(1));

         }


//         display2(500);

//         t2 = std::chrono::steady_clock::now();
//         int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
     }

}
