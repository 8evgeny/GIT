#include <iostream>
#include "main.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include "display.h"
#include <memory>

int main(int argc, char *argv[])
{
    std::shared_ptr<Devices> dev(new Devices);
    std::unique_ptr<Display> lcd(new Display);
    if (argc == 2)     //Тестовый режим (Аргумент - номер экрана)
    {
        std::cout<<"Test mode\n";
        test(std::stoi(argv[1]));
        return 0;
    }

//     auto t1 = std::chrono::steady_clock::now();
//     auto t2 = std::chrono::steady_clock::now();
//     auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
//     while (int_ms.count() < 5000)

    //Запускаем пинг в потоках
     std::vector<std::string> resultsPing{dev->getIpAdressDevices().size()};
     for (auto &i : resultsPing)
     {
         i="";
     }

     std::vector<std::thread> pingThread;
     for (uint i = 0; i < dev->getIpAdressDevices().size(); ++i)
     {
         pingThread.push_back(std::thread(&Devices::pingDevice, dev,
                              dev->getIpAdressDevices()[i], std::ref(resultsPing[i])));
     }

     for (auto &i : pingThread)
     {
         i.detach();
     }
    //число устройств общее
     std::string num = dev->getNumDevices();
     std::cout << num <<std::endl;
     while(1)
     {

         std::this_thread::sleep_for(std::chrono::seconds(1));
         for (auto &i:resultsPing)
         {
             std::cout << i <<std::endl;
         }
         std::cout <<std::endl;
     }


//         display2(500);

//         t2 = std::chrono::steady_clock::now();
//         int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);


}
