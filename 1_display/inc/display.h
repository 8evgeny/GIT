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
                  std::string offDev, bool CheckStateBuzzer);
    void diagnosticFrame(std::vector<std::string>& noPingDevices, std::vector<std::string>& noPingNumbersDevices);
    void noConnectFrame(std::string num1, std::string num2, std::string num3,
                                 std::string ip1, std::string ip2, std::string ip3);

    void wait(int ms);
    void setLcdYellow(bool);
    void setLcdRed(bool);
    void setBuzzer(bool);

    bool getLcdRedState();
    system_clock::time_point getRedLedTimeOn() const;
    void setRedLedTimeOn(system_clock::time_point newRedLedOn);
    system_clock::time_point getRedLedTimeOff() const;
    void setRedLedTimeOff(system_clock::time_point newRedLedOff);
    void changeStateRedLed();

    bool getBuzzerState();
    system_clock::time_point getBuzzerTimeOn() const;
    void setBuzzerTimeOn(system_clock::time_point newBuzzerTimeOn);
    system_clock::time_point getBuzzerTimeOff() const;
    void setBuzzerTimeOff(system_clock::time_point newBuzzerTimeOff);
    void changeStateBuzzer();



private:
    std::string _port = "/dev/ttyACM0";
    std::string _lcdYellow;
    std::string _lcdRed;
    std::string _buzzer;
    //временная отметка включен красный светодиод
    system_clock::time_point _redLedTimeOn;
    //временная отметка выключен красный светодиод
    system_clock::time_point _redLedTimeOff;
    //временная отметка включен зуммер
    system_clock::time_point _buzzerTimeOn;
    //временная отметка выключен зуммер
    system_clock::time_point _buzzerTimeOff;
};
