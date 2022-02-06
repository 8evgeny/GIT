#include <fstream>
#include <vector>
#include <thread>
#include <cstring>
#include "sysCmd.h"
#include <mutex>

std::string numDevices()
//возвращает общее число устройств
{
    std::ifstream file("../devices_num");
    std::string s;
    file >> s;
    int d = stoi(s);
    char buffer [80];
    sprintf(buffer, "%04d", d);
    return buffer;
}

std::vector<std::string> allDevices()
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


std::vector<std::string> offlineDevices()
{
    //возвращает вектор строк - оффлайн устройства
    std::ifstream file("../devices_offline");
    std::vector<std::string> v;
    std::string s;
    while(file >> s)
    {
        v.push_back(s);
    }
    return v;
}

std::string offlineDevicesNum()
//возвращает число оффлайн
{
    auto v = offlineDevices();
    char buffer [5];
    sprintf(buffer, "%04d", (int)v.size());
    std::string onDev = buffer;
    return buffer;
}

std::string onlineDevicesNum()
//возвращает число онлайн
{
    int on = std::stoi(numDevices()) - std::stoi(offlineDevicesNum());
    char buffer [80];
    sprintf(buffer, "%04d", on);
    std::string onDev = buffer;
    return buffer;
}

void pingDevice(std::string i, std::string& result)
{
    while(1)
    {
//        m.lock();
//        std::cout << "device "<< i <<" ping\n";
        if (strstr(sysCdm("ping -c 3 -f -i 0,2 -n " + i + " | grep \" 0% packet loss\"").c_str(),
                   "0% packet loss"))
        {
            result = "device " + i + " available";
        }
        else
        {
            result = "device " + i + " unavailable";
        }
//        m.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

