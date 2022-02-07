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
//std::cout << "numDevAll: "<<numDevAll <<std::endl;
//std::this_thread::sleep_for(std::chrono::seconds(1));

    //Выводим первый экран не менее 5 с
    lcd->setLcdYellow(false);
    lcd->setLcdRed(false);
    lcd->setBuzzer(false);
    auto serial = dev->getSerialNumber();
    for (int i=0; i<3; ++i)
    {
        lcd->display1(serial, 1000);
        lcd->display1_(serial, 1000);
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

        lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline, 1000);
        lcd->wait(500);

        std::string num1;
        std::string num2;
        std::string num3;
        std::string ip1;
        std::string ip2;
        std::string ip3;

        int indexNoPing = 0;
        int noPingSize = noPingDevices.size();
        if (noPingSize > 0)
        {

        }

        lcd->diagnosticFrame(num1, num2, num3, ip1, ip2, ip3, 1000);

for (auto &i : noPingDevices)
{
    std::cout << "noPingDevice: " << i <<std::endl;
}
//for (auto &i : noPingNumbersDevices)
//{
//    std::cout << "noPingNumberDevice: " << i <<std::endl;
//}


//std::cout << std::endl;
    }

}
