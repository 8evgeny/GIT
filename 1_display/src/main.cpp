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

    //starting ping in threads
    std::vector<std::string> resultsPing{dev->getIpAdressDevicesV().size()};
    for (auto &i : resultsPing)
    {
        i="";
    }

    std::vector<std::thread> pingThread;
    for (uint i = 0; i < dev->getIpAdressDevicesV().size(); ++i)
    {
        pingThread.push_back(std::thread(&Devices::pingDevice, dev,
                             dev->getIpAdressDevicesV()[i], std::ref(resultsPing[i])));
    }

    for (auto &i : pingThread)
    {
        i.detach();
    }

    //число устройств общее
    std::string numDevAll = dev->getNumDevices();
//std::cout << "numDevAll: "<<numDevAll <<std::endl;

    auto serial = dev->getSerialNumber();
    /* После окончания загрузки операционной системы перед началом опроса подключенных
       устройств - два сигнала (звучание в течение 0,5 с с интервалом 0,5 с).         */

    //Выводим первый экран не менее 5 с
    lcd->setLcdYellow(false);
    lcd->setLcdRed(false);
    lcd->setBuzzer(true);
    lcd->display1(serial);
    lcd->wait(500);
    lcd->setBuzzer(false);
    lcd->display1(serial);
    lcd->wait(500);
    lcd->setBuzzer(true);
    lcd->display1(serial);
    lcd->wait(500);
    lcd->setBuzzer(false);
    lcd->display1(serial);
    lcd->wait(500);
    lcd->display1_(serial);
    lcd->wait(1000);
    lcd->display1(serial);
    lcd->wait(1000);
    lcd->display1_(serial);
    lcd->wait(1000);

    std::vector<std::string> noPingDevices;
    std::vector<std::string> noPingNumbersDevices;
    bool firstInvoke = true;
    //временная отметка 1 раз в минуту зуммер
    auto startMinuteForBuzzer = std::chrono::steady_clock::now();

    while(1)
    {
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
                noPingDevices.push_back(dev->getIpAdressDevicesV().at(i));
                noPingNumbersDevices.push_back(dev->getNumbersDevicesV().at(i));
            }
            ++i;
        }

        //number device online
        dev->setNumOnlineDevices(on);
        std::string numDevOnline = dev->getNumOnlineDevices();
std::cout << "numDevOnline: " <<numDevOnline <<std::endl;

        //number device offline
        int off = dev->getIpAdressDevicesV().size() - on;
        dev->setNumOfflineDevices(off);
        std::string numDevOffline = dev->getNumOfflineDevices();
std::cout << "numDevOffline:"<< numDevOffline <<std::endl;

//for (auto &i:resultsPing)
//{
//    if (i != "") std::cout << i <<std::endl;
//}
//std::cout <<std::endl;

        /* потеряна со всеми устройствами, должен непрерывно светиться
            индикатор красного цвета (индикатор желтого цвета светиться не должен) и должна
            непрерывно воспроизводиться последовательность сигналов (звучание в течение 0,5 с с
            интервалом 0,5 с) */
        if (on == 0)
        {
            lcd->setLcdYellow(false);
            lcd->setLcdRed(true);

            while (1)
            {
                for (auto &k:resultsPing)
                {
                    if (k == "")
                    {
                        break;
                    }
                }
                lcd->setBuzzer(true);
                lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                lcd->wait(500);
                lcd->setBuzzer(false);
                lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                lcd->wait(500);
            }
        }//end all offline

        if (off !=0)
        {// есть оффлайн
            lcd->setLcdYellow(true);
            lcd->setLcdRed(false);

            if (firstInvoke)
            {
                firstInvoke = false;
                //четыре зумма по 0,5с
                for (int i = 0; i<4; ++i)
                {
                    lcd->setBuzzer(true);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                    lcd->setBuzzer(false);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                }
                for (int i = 0; i < 4 ; ++i)
                {//для смены времени
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(1000);
                }

                //При первом вызове дежурный кадр показываем 5 секунд
                //Идем к экрану диагностики
            }//end first invoke
            else
            {//не первый вызов, есть оффлайн
                auto currTime = std::chrono::steady_clock::now();
                auto int_s = std::chrono::duration_cast<std::chrono::seconds>(currTime - startMinuteForBuzzer);
                if (int_s.count() > 60 )
                {
                    startMinuteForBuzzer = std::chrono::steady_clock::now(); //новая отметка
                    //один сигнал раз в нинуту
                    lcd->setBuzzer(true);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                    lcd->setBuzzer(false);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                }
                for (int i = 0; i < 4 ; ++i)
                {//для смены времени
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(1000);
                }
                //Идем к экрану диагностики
            }

            lcd->diagnostic(noPingDevices, noPingNumbersDevices);
        }// есть оффлайн
        else
        {// все онлайн
            lcd->setLcdYellow(false);
            lcd->setLcdRed(false);

            if (firstInvoke)
            {
                firstInvoke = false;
                //три зумма по 0,5с
                for (int i = 0; i < 3; ++i)
                {
                    lcd->setBuzzer(true);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                    lcd->setBuzzer(false);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                }
            }
            for (int i = 0; i < 4 ; ++i)
            {
                lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                lcd->wait(1000);
            }

            lcd->diagnostic(noPingDevices, noPingNumbersDevices);

        }// все онлайн

//for (auto &i : noPingDevices)
//{
//    std::cout << "noPingDevice: " << i <<std::endl;
//}
//for (auto &i : noPingNumbersDevices)
//{
//    std::cout << "noPingNumberDevice: " << i <<std::endl;
//}
//std::cout << std::endl;

    }//end while(1)
}
