#include "islp_server.h"

void IslpServer::start() noexcept
{
    m_proc.start();
    m_proc.yellStations();
}
