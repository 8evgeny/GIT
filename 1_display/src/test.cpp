#include <iostream>
#include "main.h"
#include "display.h"

void test(int scr)
{
    Display d;
    switch (scr)
    {
    case 0:
        d.display0();
        break;
    case 1:
        d.display1(10000);
        break;
    case 2:
        d.display2(10000);
        break;
    case 3:
    {
        auto dt = datetime();
        std::cout << dt.first <<"  "<<dt.second<< "\n";
//        std::string numDev = numDevices();
//        std::string onDev = onlineDevicesNum();
//        std::string offDev = offlineDevicesNum();
//        std::cout<<"All Devices:\t\t"<<numDev<<"\n";
//        std::cout<<"Online Devices:\t"<<onDev<<"\n";
//        std::cout<<"Offline Devices:\t"<<offDev<<"\n";

//        d.display3(dt, numDev, onDev, offDev, 10000);
    }
        break;
    case 4:
    {
//        std::vector<std::string> offDevises = offlineDevices();
//        for (auto &i : offDevises)  std::cout<<i<<"\n";
//        std::string num1 = "1231";
//        std::string num2 = "1333";
//        std::string num3 = "1445";
//        std::string ip1 = "172.101.101.101";
//        std::string ip2 = "172.101.101.101";
//        std::string ip3 = "172.101.101.101";

//        d.display4(num1,num2,num3,ip1,ip2,ip3, 10000);
    }
        break;

    default:
        d.display0();
    }
}
