#include "ipsnames.hpp"
#include <map>

#define ERROR_TYPE_ID 0xFFFF

namespace IPS
{

  stationNames &stationNames::getInstance()
  {
    static stationNames instance;
    return instance;
  }

  stationNames::stationNames()
  {
  }

}

QString IPS::Stations::QTypeName(uint16_t TypeID)
{
  return QString::fromStdString (IPS::Stations::TypeName (TypeID));
}

std::string IPS::Stations::TypeName(uint16_t TypeID)
{
  if(IPS::stationNames::getInstance().names.count (TypeID) != 0) return IPS::stationNames::getInstance().names.at (TypeID);
  return IPS::stationNames::getInstance().names.at (ERROR_TYPE_ID);
}
