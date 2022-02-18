#include <iostream>
#include "main.h"
#include <chrono>
#include <thread>
#include "devices.h"
#include "display.h"
#include <memory>

int main()
{
    using std::chrono::system_clock;
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
    auto serial = dev->getSerialNumber();
    auto major = dev->getVersionProgramm();
    auto version = major + ".0";
    /* После окончания загрузки операционной системы перед началом опроса подключенных
       устройств - два сигнала (звучание в течение 0,5 с с интервалом 0,5 с).         */

    lcd->setLcdYellow(false);
    lcd->setLcdRed(false);

    for (int i=0; i < 2; ++i)
    {
        lcd->setBuzzer(true);
        lcd->display1(version, serial);
        lcd->wait(500);
        lcd->setBuzzer(false);
        lcd->display1(version, serial);
        lcd->wait(500);
    }

    for (int i=0; i<4;++i)
    {
        lcd->display1(version, serial);
        lcd->wait(1000);
        lcd->display1_(version, serial);
        lcd->wait(1000);
    }

    std::vector<std::string> noPingDevices;
    std::vector<std::string> noPingNumbersDevices;
    bool firstInvoke = true;
    //временная отметка 1 раз в минуту зуммер
    system_clock::time_point currTime1 = system_clock::now();
    //временная отметка включен красный светодиод
    system_clock::time_point redLedOn = system_clock::now();
    //временная отметка выключен красный светодиод
    system_clock::time_point redLedOff = system_clock::now();
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

        //number device offline
        int off = dev->getIpAdressDevicesV().size() - on;
        dev->setNumOfflineDevices(off);
        std::string numDevOffline = dev->getNumOfflineDevices();

        /* потеряна со всеми устройствами, должен непрерывно светиться
            индикатор красного цвета (индикатор желтого цвета светиться не должен) и должна
            непрерывно воспроизводиться последовательность сигналов (звучание в течение 0,5 с с
            интервалом 0,5 с) */
        if (on == 0)
        {
            lcd->setLcdRed(true);
            bool exit = false;

            while (!exit)
            {
                for (auto &k:resultsPing)
                {
                    if (k == "")
                        exit = true;
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
            if (!lcd->getLcdRed())
            {// красный выключен - включаем его если с момента выключения прошла секунда
                auto currTimeLedRed = system_clock::now();
                auto interval = (currTimeLedRed - redLedOff);
                auto int_s =  interval.count()/1000000000;
                if (int_s >= 1 )
                {
                    redLedOn = system_clock::now();
                    lcd->setLcdRed(true);
                }
            }
            if (lcd->getLcdRed())
            {// красный включен - проверяем если прошла 1с - выключаем его
                auto currTimeLedRed = system_clock::now();
                auto interval = (currTimeLedRed - redLedOn);
                auto int_s =  interval.count()/1000000000;
                if (int_s >= 1 )
                {
                    redLedOff = system_clock::now();
                    lcd->setLcdRed(false);
                }
            }

            if (firstInvoke)
            {
                firstInvoke = false;
                //четыре зумма по 0,5с
                for (int i = 0; i < 4; ++i)
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
                auto currTime2 = system_clock::now();
                auto interval = (currTime2 - currTime1);
                auto int_s =  interval.count()/1000000000;
                if (int_s >= 60 )
                {
                    currTime1 = currTime2;
                    //один сигнал раз в нинуту
                    lcd->setBuzzer(true);
                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(500);
                    lcd->setBuzzer(false);
                }
                for (int i = 0; i < 4 ; ++i)
                {//для смены времени


                    if (!lcd->getLcdRed())
                    {// красный выключен - включаем его если с момента выключения прошла секунда
                        auto currTimeLedRed = system_clock::now();
                        auto interval = (currTimeLedRed - redLedOff);
                        auto int_s =  interval.count()/1000000000;
                        if (int_s >= 1 )
                        {
                            redLedOn = system_clock::now();
                            lcd->setLcdRed(true);
                        }
                    }
                    if (lcd->getLcdRed())
                    {// красный включен - проверяем если прошла 1с - выключаем его
                        auto currTimeLedRed = system_clock::now();
                        auto interval = (currTimeLedRed - redLedOn);
                        auto int_s =  interval.count()/1000000000;
                        if (int_s >= 1 )
                        {
                            redLedOff = system_clock::now();
                            lcd->setLcdRed(false);
                        }
                    }




                    lcd->dutyFrame(datetime(), numDevAll, numDevOnline, numDevOffline);
                    lcd->wait(1000);
                }
                //Идем к экрану диагностики
            }

            lcd->diagnosticFrame(noPingDevices, noPingNumbersDevices);

        }// есть оффлайн
        else
        {// все онлайн
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

            lcd->diagnosticFrame(noPingDevices, noPingNumbersDevices);
        }// все онлайн
    }//end while(1)
}
