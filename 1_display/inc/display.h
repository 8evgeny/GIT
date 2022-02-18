#pragma once
#include <string>
#include <chrono>
#include <memory>
#include <vector>

using std::chrono::system_clock;

class Display
{
public:
    Display();
    ~Display();

    void printToLcd(std::string str, std::string port);
    void display1(std::string version, std::string serial);
    void display1_(std::string version, std::string serial);
    void dutyFrame(std::pair<std::string, std::string> dt,
                  std::string numDev, std::string onDev,
                  std::string offDev);
    void diagnosticFrame(std::vector<std::string>& noPingDevices, std::vector<std::string>& noPingNumbersDevices);
    void wait(int ms);
    void setLcdYellow(bool);
    void setLcdRed(bool);
    void setBuzzer(bool);
    bool getLcdRed();
    system_clock::time_point getRedLedOn() const;
    void setRedLedOn(system_clock::time_point newRedLedOn);
    system_clock::time_point getRedLedOff() const;
    void setRedLedOff(system_clock::time_point newRedLedOff);
    void stateRedLed();

private:
    std::string _port = "/dev/ttyACM0";
    std::string _lcdYellow;
    std::string _lcdRed;
    std::string _buzzer;
    //временная отметка включен красный светодиод
    system_clock::time_point _redLedOn;
    //временная отметка выключен красный светодиод
    system_clock::time_point _redLedOff;
};
