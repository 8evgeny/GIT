/**
 * @file command.cpp
 *
 */

#include "command.h"

void sendToCan()
{
    CAN* can = CAN::getInstance();
    std::vector<Command*> v;
//    v.push_back( new CreateCanMessage(can));
    v.push_back( new SendCanMessage(can));

    for (size_t i=0; i<v.size(); ++i)
        v[i]->execute();

    v.clear();
}
