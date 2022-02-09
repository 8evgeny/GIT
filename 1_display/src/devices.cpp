#include <fstream>
#include <vector>
#include <thread>
#include <cstring>
#include "devices.h"
#include <iostream>

Devices::Devices()
{
    _ipAdressDevices = allDevices();

    for (uint k = 1; k <= static_cast<uint>(_ipAdressDevices.size()); ++k)
    {
        char buffer [5];
        sprintf(buffer, "%04d", k);
        _numbersDevices.push_back(buffer);
    }

    char buffer [4];
    sprintf(buffer, "%03d", static_cast<int>(_ipAdressDevices.size()));
    _numAllDevices = buffer;
    _serialNumber = readSerialNumber();
    _versionProgramm = readVersionProgramm();
}

Devices::~Devices()
{
}

std::string Devices::readSerialNumber()
{
    std::ifstream file("../git-info.txt");
    std::string s;
    while(file)
    {
        file >> s;
        if (strstr(s.c_str(), "serial"))
            break;
    }
    //После слова serial: должен быть пробел
    file >> s; //Серийный номер
    return  s;
}

std::string Devices::readVersionProgramm()
{
    std::ifstream file("../git-info.txt");
    std::string s;
    while(file)
    {
        file >> s;
        if (strstr(s.c_str(), "version"))
            break;
    }
    //После слова version: должен быть пробел
    file >> s; //номер версии
    return  s;
}

std::vector<std::string> Devices::allDevices()
{
    //возвращает вектор строк - все ip адреса
    std::ifstream file("../devices_all");
    std::vector<std::string> v;
    std::string s;
    while(file >> s)
    {
        v.push_back(s);
    }
    return v;
}

void Devices::pingDevice(std::string i, std::string& result)
{
    while(1)
    {
        if (strstr(sysCdm("ping -c 1 -f -i 0,2 -n " + i + " "
                          "| grep \" 0% packet loss\"").c_str(), "0% packet loss"))
        {
            result = "";
        }
        else
        {
            std::string date = datetime().first + "  " + datetime().second;
            std::string sys = "echo " + date + " device " + i + " unavailable  >> ../logfile";
            system(sys.c_str());
            result = "device " + i + " unavailable";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
}

std::string Devices::sysCdm(std::string comand)
{
    std::string result{""};
    char buf[BUFSIZ];
    FILE *ptr;
    if ((ptr = popen(comand.c_str(), "r")) != NULL)
    {
        while (fgets(buf, BUFSIZ, ptr) != NULL)
        {
            result.append(buf);
        }
    }

    pclose(ptr);
    return result;
}

const std::vector<std::string> &Devices::getIpAdressDevicesV() const
{
    return _ipAdressDevices;
}

const std::string &Devices::getNumDevices() const
{
    return _numAllDevices;
}

const std::string &Devices::getNumOnlineDevices() const
{
    return _numOnlineDevices;
}

void Devices::setNumOnlineDevices(int num)
{
    char buffer [80];
    sprintf(buffer, "%03d", num);
    _numOnlineDevices = buffer;
}

const std::string &Devices::getNumOfflineDevices() const
{
    return _numOfflineDevices;
}

void Devices::setNumOfflineDevices(int num)
{
    char buffer [80];
    sprintf(buffer, "%03d", num);
    _numOfflineDevices = buffer;
}

const std::vector<std::string> &Devices::getNumbersDevicesV() const
{
    return _numbersDevices;
}

const std::string &Devices::getSerialNumber() const
{
    return _serialNumber;
}

const std::string &Devices::getVersionProgramm() const
{
    return _versionProgramm;
}




