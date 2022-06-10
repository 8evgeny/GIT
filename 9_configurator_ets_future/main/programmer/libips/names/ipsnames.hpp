#ifndef IPSNAMES_H
#define IPSNAMES_H

#include <string>
#include <iostream>

#define TYPE_PRINT_TABLE(type) std::cout <<"|"<< type << "|" << IPS::Stations::TypeName(type) << "|"<< #type <<"|\n";
/**
 * @namespace IPS
 */
namespace IPS
{

  /**
   * @brief The TypeID enum contains of device types
   */
enum TypeID
{
    Unknown = static_cast<uint16_t>(0),
    TestDev = static_cast<uint16_t>(1),
    PDO16 = static_cast<uint16_t>(2),
    PDO16N = static_cast<uint16_t>(3),
    PDO32 = static_cast<uint16_t>(4),
    PDO32N = static_cast<uint16_t>(5),
    PDO48 = static_cast<uint16_t>(6),
    PDO16P = static_cast<uint16_t>(7),
    PDO32P = static_cast<uint16_t>(8),
    PDO64P = static_cast<uint16_t>(9),
    PDO80P = static_cast<uint16_t>(10),
    PDO96P = static_cast<uint16_t>(11),
    PDO112P = static_cast<uint16_t>(12),
    PDO16PT = static_cast<uint16_t>(13),
    PDO32PT = static_cast<uint16_t>(14),
    PDO64PT = static_cast<uint16_t>(15),
    PDO80PT = static_cast<uint16_t>(16),
    PDO96PT = static_cast<uint16_t>(17),
    PDO112PT = static_cast<uint16_t>(18),
    UPV2 = static_cast<uint16_t>(22),
    UPV4 = static_cast<uint16_t>(23),
    UPV6 = static_cast<uint16_t>(24),
    UPV2U = static_cast<uint16_t>(25),
    UPV4U = static_cast<uint16_t>(26),
    UPV6U = static_cast<uint16_t>(27),
    UM2120 = static_cast<uint16_t>(28),
    SL1 = static_cast<uint16_t>(29),
    Error = static_cast<uint16_t>(0xFFFF)
    /** To be continued
    */
 };

  namespace Stations
  {

    /**
     * @brief QTypeName returns QT string name for given type ID
     * @param TypeID
     * @return
     */
  std::string QTypeName(uint16_t TypeID);
  /**
   * @brief TypeName returns string name for given type ID
   * @param TypeID
   * @return
   */
  std::string TypeName(uint16_t TypeID);
  }

}

#endif // IPSNAMES_H
