#pragma once
#include <string>
#include <vector>

std::pair<std::string,std::string> datetime();
std::vector<std::string> offlineDevices();
std::string numDevices();
std::string offlineDevicesNum();
std::string onlineDevicesNum();
void display0();
void display1(int ms);
void display2(int ms);
void display3(std::pair<std::string, std::string> dt,
              std::string numDev, std::string onDev,
              std::string offDev, int ms);
void display4(
        std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3, int ms);
void test(int scr);
