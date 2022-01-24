#include <QString>
#include "station_programmer.h"

StationProgrammer::StationProgrammer(StationItem* StationItem)
    : m_stationItem(StationItem)
{
}
QString const& StationProgrammer::mac() const noexcept
{
    return m_mac;
}
bool StationProgrammer::status() const noexcept
{
    return m_status;
}
QString const& StationProgrammer::serialNumber() const noexcept
{
    return m_serialNumber;
}
StationItem* StationProgrammer::stationItem()
{    
    return m_stationItem;
}
void StationProgrammer::setMac(QString const& mac) noexcept
{    
    m_mac = mac;
}
void StationProgrammer::setStatus(bool status) noexcept
{      
    m_status = status;
}
void StationProgrammer::setSerialNumber(QString const& serialNumber) noexcept
{
    m_serialNumber = serialNumber;
}
