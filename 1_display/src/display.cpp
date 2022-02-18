#include <string>
#include <vector>
#include <thread>
#include "display.h"
#include <iostream>
#include <algorithm>

Display::Display()
{
    _lcdYellow = std::to_string(0);
    _lcdRed = std::to_string(0);
    _buzzer = std::to_string(0);
}
Display::~Display()
{
}

void Display::printToLcd(std::string str, std::string port)
{
    std::string cmd = "echo " + str + ">" + port;
    system(cmd.c_str());
}

void Display::display1(std::string version, std::string serial)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "\"    GIT-COMM IPS    \"";
    line2 = "\"ПДКВ  ВЕРСИЯ ПО: \"" + version;
    line3 = "\"  S/N:" + serial + "  \"";
    line4 = "\" Инициализация...   \"";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
}

void Display::display1_(std::string version, std::string serial)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "\"    GIT-COMM IPS    \"";
    line2 = "\"ПДКВ  ВЕРСИЯ ПО: \"" + version;
    line3 = "\"  S/N:" + serial + "  \"";
    line4 = "\"                    \"";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
}

void Display::dutyFrame(std::pair<std::string, std::string> dt,
              std::string numDev, std::string onDev,
              std::string offDev)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = dt.first + "\"  \"" + dt.second;
    line2 = "УСТРОЙСТВ ВСЕГО " + numDev;
    line3 = "\" \" НА СВЯЗИ\"       \"" + onDev  + "\" \"";
    line4 = " ОТКЛЮЧЕНО\"     \" " + offDev+ "\"     \"";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
}

//void Display::stateRedLed()
//{
//    if (!this->getLcdRed())
//    {// красный выключен - включаем его если с момента выключения прошла секунда
//        auto currTimeLedRed = system_clock::now();
//        auto interval = (currTimeLedRed - this->getRedLedOff());
//        auto int_s =  interval.count()/1000000000;
//        if (int_s >= 1 )
//        {
//            this->setRedLedOn(system_clock::now());
//            this->setLcdRed(true);
//        }
//    }
//    if (this->getLcdRed())
//    {// красный включен - проверяем если прошла 1с - выключаем его
//        auto currTimeLedRed = system_clock::now();
//        auto interval = (currTimeLedRed - this->getRedLedOn());
//        auto int_s =  interval.count()/1000000000;
//        if (int_s >= 1 )
//        {
//            this->setRedLedOff(system_clock::now());
//            this->setLcdRed(false);
//        }
//    }
//}


void Display::diagnosticFrame(std::vector<std::string>& noPingDevices,
                              std::vector<std::string>& noPingNumbersDevices)
{
    this->setLcdRed(true);
    //формируем диагностический экран
    std::string num1{""};
    std::string num2{""};
    std::string num3{""};
    std::string ip1{""};
    std::string ip2{""};
    std::string ip3{""};

    switch (noPingDevices.size())
    {
    case 0 :
        return;
    case 1 :
    {
        num1 = noPingNumbersDevices[0];
        ip1 = noPingDevices[0];
        std::string lcdbuz,line1,line2,line3,line4;
        lcdbuz = _lcdYellow + _lcdRed + _buzzer;
        line1 = "\"     НЕТ СВЯЗИ      \"";
        line2 = num1 + "\" \"" + ip1;
        line3 = "\"                    \"";
        line4 = "\"                    \"";
        printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        for (int i=0; i < 2; ++i)
        {
            wait(1000);
            setLcdRed(false);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
            wait(1000);
            setLcdRed(true);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        }
    }
    break;
    case 2 :
    {
        num1 = noPingNumbersDevices[0];
        num2 = noPingNumbersDevices[1];
        ip1 = noPingDevices[0];
        ip2 = noPingDevices[1];
        std::string lcdbuz,line1,line2,line3,line4;
        lcdbuz = _lcdYellow + _lcdRed + _buzzer;
        line1 = "\"     НЕТ СВЯЗИ      \"";
        line2 = num1 + "\" \"" + ip1;
        line3 = num2 + "\" \"" + ip2;
        line4 = "\"                    \"";
        printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        for (int i=0; i < 2; ++i)
        {
            wait(1000);
            setLcdRed(false);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
            wait(1000);
            setLcdRed(true);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        }
    }
    break;
    case 3 :
    {
        num1 = noPingNumbersDevices[0];
        num2 = noPingNumbersDevices[1];
        num3 = noPingNumbersDevices[2];
        ip1 = noPingDevices[0];
        ip2 = noPingDevices[1];
        ip3 = noPingDevices[2];
        std::string lcdbuz,line1,line2,line3,line4;
        lcdbuz = _lcdYellow + _lcdRed + _buzzer;
        line1 = "\"     НЕТ СВЯЗИ      \"";
        line2 = num1 + "\" \"" + ip1;
        line3 = num2 + "\" \"" + ip2;
        line4 = num3 + "\" \"" + ip3;
        printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        for (int i=0; i < 2; ++i)
        {
            wait(1000);
            setLcdRed(false);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
            wait(1000);
            setLcdRed(true);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
        }
    }
    break;
    default: //размер 4 и выше
    {
        int i = 0; //указатель на 1 строку
        //экран со сдвигом
        while (1)
        {
            setLcdRed(true);
            num1 = noPingNumbersDevices[i];
            num2 = noPingNumbersDevices[i + 1];
            num3 = noPingNumbersDevices[i + 2];
            ip1 = noPingDevices[i];
            ip2 = noPingDevices[i + 1];
            ip3 = noPingDevices[i + 2];
            std::string lcdbuz,line1,line2,line3,line4;
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            line1 = "\"     НЕТ СВЯЗИ      \"";
            line2 = num1 + "\" \"" + ip1;
            line3 = num2 + "\" \"" + ip2;
            line4 = num3 + "\" \"" + ip3;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);

            if (static_cast<int>(noPingDevices.size()) == (i + 3))
            {
                break;
            }
            wait(1000);
            setLcdRed(false);
            lcdbuz = _lcdYellow + _lcdRed + _buzzer;
            printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
            ++i;
        }

        //Здесь не сделано задержка


        for (int i=0; i < 2; ++i)
        {
            wait(1000);
            setLcdRed(false);
            wait(1000);
            setLcdRed(true);
        }
    }
    }


}

void Display::wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Display::setLcdYellow(bool newLcdYellow)
{
    if (newLcdYellow)
    {
        _lcdYellow = std::to_string(1);
    }
    else
    {
        _lcdYellow = std::to_string(0);
    }
}

void Display::setLcdRed(bool newLcdRed)
{
    if (newLcdRed)
    {
        _lcdRed = std::to_string(1);
    }
    else
    {
        _lcdRed = std::to_string(0);
    }
}

void Display::setBuzzer(bool newBuzzer)
{
    if (newBuzzer)
    {
        _buzzer = std::to_string(1);
    }
    else
    {
        _buzzer = std::to_string(0);
    }
}

bool Display::getLcdRed()
{
    if (_lcdRed != "1")
    {
        return false;
    }
    return true;
}

system_clock::time_point Display::getRedLedOn() const
{
    return _redLedOn;
}

void Display::setRedLedOn(system_clock::time_point newRedLedOn)
{
    _redLedOn = newRedLedOn;
}

system_clock::time_point Display::getRedLedOff() const
{
    return _redLedOff;
}

void Display::setRedLedOff(system_clock::time_point newRedLedOff)
{
    _redLedOff = newRedLedOff;
}

