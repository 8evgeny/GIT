/*!
 \file crc16_ccitt.h

*/

#ifndef CRC16_CCITT_H
#define CRC16_CCITT_H

#include <cstdint>
#include <array>
#include "CircularBuffer.h"
#include <vector>
#include "packageClass.h"

/*!
 \brief The function computes the CRC16 CCITT checksum

 \fn crc16_ccitt
 \param pData message pointer
 \param length message length
 \return uint16_t checksum
*/

class CRC16_CCITT
{
private:
#ifdef STM32F777x
    static constexpr uint8_t lengthRx = 8;
    static constexpr uint8_t lengthTx = 18;
#else
    static constexpr uint8_t lengthRx = 18;
    static constexpr uint8_t lengthTx = 8;
#endif

public:
    uint16_t get(uPackageTx package);
    uint16_t get(uPackageRx package);
};


#endif // CRC16_CCITT_H
