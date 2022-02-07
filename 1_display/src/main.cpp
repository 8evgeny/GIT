#include <iostream>
#include "main.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include "display.h"
#include <memory>

int main(int argc, char *argv[])
{

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

    std::shared_ptr<Devices> dev(new Devices);
    std::unique_ptr<Display> lcd(new Display);

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
    std::string numDevAll = dev->getNumDevices();
    std::cout << "numDevAll: "<<numDevAll <<std::endl;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //число устройств онлайн
        int on = 0;
        int i = 0;
        std::vector<std::string> noPingDevices;
        std::vector<std::string> noPingNumbersDevices;
        for (auto &k:resultsPing)
        {
            if (k == "")
            {
                ++on;
            }
            else
            {
                //Пинг не прошел i - индекс device
                noPingDevices.push_back(dev->getIpAdressDevices().at(i));
                noPingNumbersDevices.push_back(dev->getNumbersDevices().at(i));
            }
            ++i;
        }
        dev->setNumOnlineDevices(on);
        std::string numDevOnline = dev->getNumOnlineDevices();
        std::cout << "numDevOnline: " <<numDevOnline <<std::endl;

        //число устройств оффлайн
        int off = dev->getIpAdressDevices().size() - on;
        dev->setNumOfflineDevices(off);
        std::string numDevOffline = dev->getNumOfflineDevices();
        std::cout << "numDevOffline:"<< numDevOffline <<std::endl;

//        for (auto &i:resultsPing)
//        {
//            if (i != "") std::cout << i <<std::endl;
//        }
//        std::cout <<std::endl;

        lcd->display2(1000);
        lcd->wait(500);

        lcd->display3(datetime(), numDevAll, numDevOnline, numDevOffline, 1000);
        lcd->wait(500);

        for (auto &i : noPingDevices)
        {
            std::cout << "noPingDevice: "<<i <<std::endl;
        }
        for (auto &i : noPingNumbersDevices)
        {
            std::cout << "noPingNumberDevice: "<< i <<std::endl;
        }

      std::cout << std::endl;
    }


//         display2(500);

//         t2 = std::chrono::steady_clock::now();
//         int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);


}
