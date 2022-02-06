#include <iostream>
#include "main.h"
#include <chrono>
#include <thread>
#include "devices.h"

int main(int argc, char *argv[])
{
    Devices devices;
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

     auto ipAdressDevices = devices.allDevices();
     std::vector<std::string> resultsPing{ipAdressDevices.size()};
         for (auto &i : resultsPing)
         {
             i="";
         }
         std::vector<std::thread> pingThread;

         for (uint i = 0; i < ipAdressDevices.size(); ++i)
         {
             pingThread.push_back(std::thread(&Devices::pingDevice, &devices,
                                              ipAdressDevices[i], std::ref(resultsPing[i])));
         }

         for (auto &i : pingThread)
         {
             i.detach();
         }

          while(1)
          {
              for (auto &i:resultsPing)
              {
                  std::cout << i <<std::endl;
              }

          std::this_thread::sleep_for(std::chrono::seconds(2));
          std::cout <<std::endl;
          }


//         display2(500);

//         t2 = std::chrono::steady_clock::now();
//         int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);


}
