#include <iostream>
#include <fstream>
#include <vector>

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

std::string numDevices()
//возвращает общее число устройств
{
    std::ifstream file("../devices_all");
    std::string s;
    file >> s;
    return s;
}
