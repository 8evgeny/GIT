#include <iostream>
#include "main.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include "display.h"
#include <memory>

int main()
{
    std::shared_ptr<Devices> dev(new Devices);
    std::shared_ptr<Display> lcd(new Display);

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
//std::cout << "numDevAll: "<<numDevAll <<std::endl;

    //Выводим первый экран не менее 5 с
    lcd->setLcdYellow(false);
    lcd->setLcdRed(false);
    lcd->setBuzzer(false);
    auto serial = dev->getSerialNumber();
    for (int i=0; i<3; ++i)
    {
        lcd->display1(serial);
        lcd->wait(1000);
        if (i != 3)
        {
            lcd->display1_(serial);
            lcd->wait(1000);
        }
    }

    std::vector<std::string> noPingDevices;
    std::vector<std::string> noPingNumbersDevices;

    while(1)
    {
        //число устройств онлайн
        int on = 0;
        int i = 0;
        noPingDevices.clear();
        noPingNumbersDevices.clear();
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

//for (auto &i:resultsPing)
//{
//    if (i != "") std::cout << i <<std::endl;
//}
//std::cout <<std::endl;

        lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
        lcd->wait(500);

        if (off !=0)
        {
            lcd->diagnostic(noPingDevices, noPingNumbersDevices);
        }

//for (auto &i : noPingDevices)
//{
//    std::cout << "noPingDevice: " << i <<std::endl;
//}
//for (auto &i : noPingNumbersDevices)
//{
//    std::cout << "noPingNumberDevice: " << i <<std::endl;
//}


//std::cout << std::endl;
    }

}
