#include <ctime>
#include <chrono>
#include <iostream>
#include "datetime.h"

std::pair<std::string,std::string> datetime()
{
    using std::chrono::system_clock;
    std::chrono::duration<int,std::ratio<60*60*24> > one_day (1);

    system_clock::time_point today = system_clock::now();
    system_clock::time_point tomorrow = today + one_day;

    std::time_t tt;

    tt = system_clock::to_time_t ( today );
//    std::cout << "today is: " << ctime(&tt);

    tt = system_clock::to_time_t ( tomorrow );
//    std::cout << "tomorrow will be: " << ctime(&tt);

    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

//  https://www.cplusplus.com/reference/ctime/strftime/
    strftime (buffer,80,"Now it's %I:%M%p.",timeinfo);
//    puts (buffer);

    std::pair<std::string,std::string> dt;
    strftime (buffer,80,"%d.%m.%Y",timeinfo);
    dt.first =buffer;
    strftime (buffer,80,"%T",timeinfo);
    dt.second =buffer;
    return dt;
}
