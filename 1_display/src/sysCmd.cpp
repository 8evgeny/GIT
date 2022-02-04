#include "sysCmd.h"

std::string sysCdm(std::string comand)
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
