/*!
 \file i2ceeprom.h

*/

#ifndef I2CEEPROM_H
#define I2CEEPROM_H

#include <cstdint>
#include "stm32f7xx_hal.h"


/*!
 \brief Implementation: ensures that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class Eeprom_i2c i2c.h "i2c.h"
*/

class I2c1
{
private:
    static I2c1 * pInstance; /*!< TODO: describe */

    /*!
     \brief Base constructor. A default constructor.

     \fn I2c1
    */
    I2c1();

    /*!
     \brief Copy constructor.

     \fn I2c1
     \param
    */
    I2c1(const I2c1&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return I2c1 &operator
    */
    I2c1& operator=(I2c1&);

public:

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return I2c1. The public static operation can be accessed easily by using the class name and operation name.
    */
    static I2c1 * getInstance();

    /*!
     \brief The function reads data from the memory through I2C

     \fn readData
     \param DevAddress Target device address
     \param MemAddress Internal memory address
     \param pData Pointer to data buffer
     \param Size Amount of data to be sent
     \return HAL_StatusTypeDef HAL status
    */
    HAL_StatusTypeDef readData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

    /*!
     \brief The function writes data to the memory through I2C

     \fn writeData
     \param DevAddress Target device address
     \param MemAddress Internal memory address
     \param pData Pointer to data buffer
     \param Size Amount of data to be sent
     \return HAL_StatusTypeDef HAL status
    */
    HAL_StatusTypeDef writeData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

    I2C_HandleTypeDef *i2c1Handle; /*!< The variable is pointer to the I2C Handle Type */


    constexpr static uint32_t timeOutMax = 300; /*!< Maximum Timeout values for flags waiting loops. These timeouts are not based
                on accurate values, they just guarantee that the application will not remain
                stuck if the I2C communication is corrupted. */

    volatile FlagStatus i2cReadReady = RESET; /**< Status flag about reading data from EEPROM through I2C */
    volatile FlagStatus i2cWriteReady = RESET; /*!< Status flag about writing data from EEPROM through I2C */

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief The enumeration that stores values of the work mode I2C
 *
 * \enum
 */
enum {
    FAST_MODE_PLUS = 0x00200922,
    STANDART_MODE = 0x20404768,
    FAST_MODE = 0x6000030D
};

/*!
 \brief The function initializes the I2C interface.

 \fn I2C1Init
*/
void I2C1Init(void);
/*!
 \brief The function initializes periphery to read and write data to the EEPROM.

 \fn EEPROM_IO_Init
*/
void EEPROM_IO_Init(void);
/*!
 \brief The function writes data to the EEPROM

 \fn readData
 \param DevAddress EEPROM device address
 \param MemAddress Internal memory address
 \param pBuffer Pointer to data buffer
 \param BufferSize Amount of data to be sent
 \return HAL_StatusTypeDef HAL status
*/
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);
/*!
 \brief The function reads data from the EEPROM

 \fn EEPROM_IO_ReadData
 \param DevAddress EEPROM device address
 \param MemAddress Internal memory address
 \param pBuffer Pointer to data buffer
 \param BufferSize Amount of data to be sent
 \return HAL_StatusTypeDef HAL status
*/
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);
/*!
 \brief The function checks if memory device is ready for communication.

 \fn EEPROM_IO_IsDeviceReady
 \param DevAddress EEPROM device address
 \param Trials Number of trials
 \return HAL_StatusTypeDef HAL status
*/
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

#ifdef __cplusplus
}
#endif

#endif // I2CEEPROM_H
