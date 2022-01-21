/*!
 \file bootloader.h

*/

#pragma once

#include "MD5/MD5.h"
#include <stdint.h>

namespace bootloader {

/*!
 \brief  The function check the common header

 \fn checkHeader
 \return uint8_t
*/
bool checkHeader();
/*!
 \brief The function check the start header

 \fn checkStart
 \return uint8_t
*/
bool checkStart();

/*!
 \brief The function check MD5

 \fn checkMD5
 \return uint8_t
*/
bool checkMD5();

/*!
 \brief The function check MD5

\fn checkMD5
\return uint8_t
*/
bool checkMD5Bare();
/*!
 \brief The function updates a new firmware

 \fn updateFirmware
 \return uint8_t
*/
bool updateFirmware();
/*!
 \brief The function copies a firmware

 \fn copyFirmware
*/
void copyFirmware();
/*!
 \brief This function is need to jump to bootloader

 \fn jumpToBootloader
*/
void jumpToFirmware();
};
