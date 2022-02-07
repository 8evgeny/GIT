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

void Display::display1(int ms)
{
    std::string stringToDisplay = "\""
                      + _lcdYellow + _lcdRed + _buzzer +
                      "     GIT-COMM IPS    "
                      " ПДКВ ВЕРСИЯ ПО: 1.0 "
                      "   S/N:903012122001  "
                      "   Инициализация...  "
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::display2(int ms)
{
    std::string stringToDisplay = "\""
                      + _lcdYellow + _lcdRed + _buzzer +
                      "     GIT-COMM IPS    "
                      " ПДКВ ВЕРСИЯ ПО: 1.0 "
                      "   Version: ____     "
                      "   Initialisation... "
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::display3(std::pair<std::string, std::string> dt,
              std::string numDev, std::string onDev,
              std::string offDev, int ms)
{
    std::string stringToDisplay = "\""
                      + _lcdYellow + _lcdRed + _buzzer +
                      dt.first + "\"  \"" + dt.second +
                      "All device  -   " + numDev +
                      "Online      -   " + onDev +
                      "Offline     -   " + offDev +
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::display4(
        std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3, int ms)
{
    std::string stringToDisplay = "\""
                      + _lcdYellow + _lcdRed + _buzzer +
                      "    Not connect:    " +
                      num1 + "\"  \"" + ip1 +
                      num2 + "\"  \"" + ip2 +
                      num3 + "\"  \"" + ip3 +
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::wait(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Display::setLcdYellow(const std::string &newLcdYellow)
{
    _lcdYellow = newLcdYellow;
}

void Display::setLcdRed(const std::string &newLcdRed)
{
    _lcdRed = newLcdRed;
}

void Display::setBuzzer(const std::string &newBuzzer)
{
    _buzzer = newBuzzer;
}
