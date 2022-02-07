#include <string>
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
//    system(cmd.c_str());
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
    printToLcd (stringToDisplay, port);
}

void Display::display1(std::string serial, int ms)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "     GIT-COMM IPS   ";
    line2 = "ПДКВ ВЕРСИЯ ПО: 1.0 ";
    line3 = "  S/N:" + serial;
    line4 = "  Инициализация...  ";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Display::display1_(std::string serial, int ms)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "     GIT-COMM IPS   ";
    line2 = "ПДКВ ВЕРСИЯ ПО: 1.0 ";
    line3 = "  S/N:" + serial;
    line4 = "                    ";
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
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
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, port);
}

void Display::diagnosticFrame(std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3, int ms)
{
    std::string lcdbuz,line1,line2,line3,line4;
    lcdbuz = _lcdYellow + _lcdRed + _buzzer;
    line1 = "    НЕТ СВЯЗИ       ";
    line2 = num1 + "\"  \"" + ip1;
    line3 = num2 + "\"  \"" + ip2;
    line4 = num3 + "\"  \"" + ip3;
    printToLcd (lcdbuz + line1 + line2 + line3 + line4, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Display::setLcdYellow(bool newLcdYellow)
{
    _lcdYellow = newLcdYellow;
}

void Display::setLcdRed(bool newLcdRed)
{
    _lcdRed = newLcdRed;
}

void Display::setBuzzer(bool newBuzzer)
{
    _buzzer = newBuzzer;
}
