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


    const std::vector<std::string> &getIpAdressDevices() const;

private:
    std::vector<std::string> _ipAdressDevices;

};
