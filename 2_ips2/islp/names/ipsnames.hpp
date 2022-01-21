#ifndef IPSNAMES_H
#define IPSNAMES_H

#include <QString>
#include <string>
#include <iostream>

#define TYPE_PRINT_TABLE(type) std::cout <<"|"<< type << "|" << IPS::Stations::TypeName(type) << "|"<< #type <<"|\n";
/**
 * @namespace IPS
 */
namespace IPS {

  /**
   * @brief The TypeID enum contains of device types
   */
  enum TypeID
    {
    Unknown = (uint16_t) 0,
    TestDev = (uint16_t) 1,
    PDO16 = (uint16_t) 2,
    PDO16N = (uint16_t) 3,
    PDO32 = (uint16_t) 4,
    PDO32N = (uint16_t) 5,
    PDO48 = (uint16_t) 6,
    PDO16P = (uint16_t) 7,
    PDO32P = (uint16_t) 8,
    PDO64P = (uint16_t) 9,
    PDO80P = (uint16_t) 10,
    PDO96P = (uint16_t) 11,
    PDO112P = (uint16_t) 12,
    PDO16PT = (uint16_t) 13,
    PDO32PT = (uint16_t) 14,
    PDO64PT = (uint16_t) 15,
    PDO80PT = (uint16_t) 16,
    PDO96PT = (uint16_t) 17,
    PDO112PT = (uint16_t) 18,
    UPV2 = (uint16_t) 22,
    UPV4 = (uint16_t) 23,
    UPV6 = (uint16_t) 24,
    UPV2U = (uint16_t) 25,
    UPV4U = (uint16_t) 26,
    UPV6U = (uint16_t) 27,
    UM2120 = (uint16_t) 28,
    SL1 = (uint16_t) 29,
    Error = (uint16_t) 0xFFFF
    /** To be continued
     */
    };

  namespace Stations {

    /**
     * @brief TypeName returns QT string name for given type ID
     * @param TypeID
     * @return
     */
    QString QTypeName(uint16_t TypeID);

    /**
     * @brief TypeName returns std::string name for given type ID
     * @param TypeID
     * @return
     */
    std::string TypeName(uint16_t TypeID);
  }

}

#endif // IPSNAMES_H
