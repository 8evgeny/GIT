#include <string>
#include <chrono>
#include <thread>
#include "display.h"

Display::Display()
{
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
                      "                     "
                      "                     "
                      "                     "
                      "                     "
                      "\"";
    printToLcd (stringToDisplay, port);
}

void Display::display1(int ms)
{
    std::string stringToDisplay = "\""
                      "     GIT-COMM IPS    "
                      "         PDKV        "
                      "   S/N:903012122001  "
                      "      Loading...     "
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

void Display::display2(int ms)
{
    std::string stringToDisplay = "\""
                      "     GIT-COMM IPS    "
                      "         PDKV        "
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
    std::string stringToDisplay = dt.first + "\"  \"" + dt.second + "\""
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
                      "    Not connect:    " +
                      num1 + "\"  \"" + ip1 +
                      num2 + "\"  \"" + ip2 +
                      num3 + "\"  \"" + ip3 +
                      "\"";
    printToLcd (stringToDisplay, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    clearDisplay();
}

