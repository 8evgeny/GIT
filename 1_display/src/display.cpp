#include <string>
#include <vector>
#include <thread>
#include "display.h"

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

void Display::clearDisplay()
{
    std::string stringToDisplay = "\""
                     + _lcdYellow + _lcdRed + _buzzer +
                      "                    "
                      "                    "
                      "                    "
                      "                    "
                      "\"";
    printToLcd (stringToDisplay, _port);
}

void Display::display1(std::string serial)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "     GIT-COMM IPS   ";
    line2 = "ПДКВ ВЕРСИЯ ПО: 1.0 ";
    line3 = "  S/N:  " + serial;
    line4 = "  Инициализация...  ";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
}

void Display::display1_(std::string serial)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "     GIT-COMM IPS   ";
    line2 = "ПДКВ ВЕРСИЯ ПО: 1.0 ";
    line3 = "  S/N:  " + serial;
    line4 = "                    ";
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
    line3 = " НА СВЯЗИ   " + onDev;
    line4 = " ОТКЛЮЧЕНО  " + offDev;
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
}

void Display::diagnosticFrame(std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "    НЕТ СВЯЗИ       ";
    line2 = num1 + "\"  \"" + ip1;
    line3 = num2 + "\"  \"" + ip2;
    line4 = num3 + "\"  \"" + ip3;
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, _port);
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

void Display::diagnostic(std::vector<std::string> noPingDevices, std::vector<std::string> noPingNumbersDevices)
{
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
            diagnosticFrame(num1, num2, num3, ip1, ip2, ip3);
            //Добавить 3 секунды со сменой t
        }
            break;
        case 2 :
        {
            num1 = noPingNumbersDevices[0];
            num2 = noPingNumbersDevices[1];
            ip1 = noPingDevices[0];
            ip2 = noPingDevices[1];
            diagnosticFrame(num1, num2, num3, ip1, ip2, ip3);
            //Добавить 3 секунды со сменой t
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
            diagnosticFrame(num1, num2, num3, ip1, ip2, ip3);
            //Добавить 3 секунды со сменой t
        }
        break;
        default: //размер 4 и выше
        {
            int i = 0; //указатель на 1 строку
            //экран со сдвигом
            while (1)
            {
                num1 = noPingNumbersDevices[i];
                num2 = noPingNumbersDevices[i + 1];
                num3 = noPingNumbersDevices[i + 2];
                ip1 = noPingDevices[i];
                ip2 = noPingDevices[i + 1];
                ip3 = noPingDevices[i + 2];
                diagnosticFrame(num1, num2, num3, ip1, ip2, ip3);
                if (static_cast<int>(noPingDevices.size()) == (i + 3))
                {
                    break;
                }
                wait(1000);
                ++i;
            }
            //Добавить 3 секунды со сменой t
        }
    }


}
