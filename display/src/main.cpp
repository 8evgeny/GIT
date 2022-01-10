#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

#include "main.h"
#include "datetime.h"

int main(int argc, char *argv[])
{

    std::cout << datetime() <<"\n";

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
            stringToDisplay = "\""
                              "    GIT-COMM IPS    "
                              "        ПДКВ        "
                              "  S/N:903012122001  "
                              "     ЗАГРУЗКА...    "
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
