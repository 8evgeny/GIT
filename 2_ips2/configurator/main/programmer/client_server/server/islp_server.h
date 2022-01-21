#pragma once
#include "../server/src_server/msgprocessor.hpp"
#include "../../libips/islp/stationresponse.hpp"

class IslpServer
{

public:
    void start() noexcept;
    std::list<intercomStation>& stations() noexcept {
        return m_proc.stations();
    }

private:
    msgProcessor m_proc;    
    std::list<intercomStation> m_stations;    
};
