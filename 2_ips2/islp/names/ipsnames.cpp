#include "ipsnames.hpp"
#include <map>

namespace IPS {
  static const std::map<uint16_t, std::string> st_names =
    {
      { 0, "Неизвестен" },
      { 1, "Тест" },
      { 2, "ПДО-16" },
      { 3, "ПДО-16Н" },
      { 4, "ПДО-32" },
      { 5, "ПДО-32Н" },
      { 6, "ПДО-48" },
      { 7, "ПДО-16П" },
      { 8, "ПДО-32П" },
      { 9, "ПДО-64П" },
      { 10, "ПДО-80П" },
      { 11, "ПДО-96П" },
      { 12, "ПДО-112П" },
      { 13, "ПДО-16ПТ" },
      { 14, "ПДО-32ПТ" },
      { 15, "ПДО-64ПТ" },
      { 16, "ПДО-80ПТ" },
      { 17, "ПДО-96ПТ" },
      { 18, "ПДО-112ПТ" },
      { 22, "УПВ-2" },
      { 23, "УПВ-4" },
      { 24, "УПВ-6" },
      { 25, "УПВ-2У" },
      { 26, "УПВ-4У" },
      { 27, "УПВ-6У" },
      { 28, "УМ-2120" },
      { 29, "СЛ1" },
      { 0xFFFF, "Ошибка" } };
}

QString IPS::Stations::QTypeName(uint16_t TypeID)
{
  return QString::fromStdString (IPS::Stations::TypeName (TypeID)); //qt_stations_names[TypeID];
}

std::string IPS::Stations::TypeName(uint16_t TypeID)
{
  if(IPS::st_names.count (TypeID) != 0) return IPS::st_names.at (TypeID);
  return IPS::st_names.at (0xFFFF);
}
