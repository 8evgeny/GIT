//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

#include <iostream>
#include "main.h"
#include "datetime.h"
#include "devices.h"


int main(int argc, char *argv[])
{
    std::vector<std::string> offDevises = offlineDevices();
    for (auto &i : offDevises)
        std::cout<<i<<"\n";

    std::string numDev = numDevices();
    std::string onDev = onlineDevicesNum();
    std::string offDev = offlineDevicesNum();

    std::cout<<"All Devices:\t\t"<<numDev<<"\n";
    std::cout<<"Online Devices:\t"<<onDev<<"\n";
    std::cout<<"Offline Devices:\t"<<offDev<<"\n";

    auto dt = datetime();
    std::cout << dt.first <<"  "<<dt.second<< "\n";

    int num = std::stoi(argv[1]);
    std::string port = "/dev/ttyACM0";
    std::string stringToDisplay;

    switch (num)
    {
            case 0:
        {
            stringToDisplay = "\""
                              "                    "
                              "                    "
                              "                    "
                              "                    "
                              "\"";
        }
            break;

            case 1:
        {
            stringToDisplay = "\""
                              "    GIT-COMM IPS    "
                              "        PDKV        "
                              "  S/N:903012122001  "
                              "     Loading...     "
                              "\"";
        }
            break;

            case 2:
        {
            stringToDisplay = dt.first + "\"  \"" + dt.second + "\""
                              "All device  -   " + numDev +
                              "Online      -   " + onDev +
                              "Offline     -   " + offDev +
                              "\"";
        }
            break;

            case 3:
        {
            stringToDisplay = "\""
                              "                    "
                              "                    "
                              "                    "
                              "                    "
                              "\"";
        }
            break;

            default:
    {
            stringToDisplay = "\""
                              "                    "
                              "                    "
                              "                    "
                              "                    "
                              "\"";
    }
 }

 std::string cmd = "echo " + stringToDisplay + ">" + port;
 system(cmd.c_str());

 }
