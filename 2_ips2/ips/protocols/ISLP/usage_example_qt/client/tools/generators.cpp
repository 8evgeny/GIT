#include "generators.hpp"

#include <QTime>
#include <QRandomGenerator>

/**
 * @brief generateStation генерирует параметры станции рандомно
 * @param station
 */
void generateStation(intercomStation *station)
{
  QRandomGenerator *rng=QRandomGenerator::global();

  station->board_type=static_cast<uint16_t>(rng->bounded(1,29));
  for(size_t x=0;x<sizeof(station->serial_number);x++)station->serial_number[x]=static_cast<char>(rng->bounded(0x30,0x39));
  for(size_t x=0;x<sizeof(station->macaddr);x++)station->macaddr[x]=static_cast<uint8_t>(rng->bounded(0x00,0xFF));
  for(size_t x=0;x<sizeof(station->ipaddr);x++)station->ipaddr[x]=static_cast<uint8_t>(rng->bounded(0x00,0xFF));
  for(size_t x=0;x<sizeof(station->netmask);x++)station->netmask[x]=static_cast<uint8_t>(rng->bounded(0x00,0xFF));
  for(size_t x=0;x<sizeof(station->gateway);x++)station->gateway[x]=static_cast<uint8_t>(rng->bounded(0x00,0xFF));
  station->setName("defaultname");
}
