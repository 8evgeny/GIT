#pragma once
#include "main.h"
#include <mutex>


class Devices
{
public:
    Devices();
    ~Devices();

    std::vector<std::string> allDevices();
    void pingDevice(std::string i, std::string& result);
    std::string sysCdm(std::string comand);
    std::string readSerialNumber();
    const std::vector<std::string> &getIpAdressDevicesV() const;
    const std::string &getNumDevices() const;
    const std::string &getNumOnlineDevices() const;
    void setNumOnlineDevices(int num);
    const std::string &getNumOfflineDevices() const;
    void setNumOfflineDevices(int num);
    const std::vector<std::string> &getNumbersDevicesV() const;
    const std::string &getSerialNumber() const;

private:
    std::vector<std::string> _ipAdressDevices;
    std::vector<std::string> _numbersDevices;
    std::string _numAllDevices{""};
    std::string _numOnlineDevices{""};
    std::string _numOfflineDevices{""};
    std::string _serialNumber{""};
};
