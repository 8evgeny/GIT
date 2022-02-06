#include "main.h"
#include <mutex>

std::vector<std::string> allDevices();
std::vector<std::string> offlineDevices();
void pingDevice(std::string i, std::string& result);
