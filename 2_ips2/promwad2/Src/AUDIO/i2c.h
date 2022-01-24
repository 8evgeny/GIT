#ifndef I2C_H
#define I2C_H

#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_i2c.h>

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class I2C i2c.h "i2c.h"
*/
class I2C
{
private:
    static I2C *pInstance;

    /*!
     \brief Base constructor. A default constructor.

     \fn I2C
    */
    I2C();

    /*!
     \brief Copy constructor.

     \fn I2C
     \param
    */
    I2C(const I2C &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return SAI &operator
    */
    I2C &operator=(I2C &);
public:
    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return I2C. The public static operation can be accessed easily by using the class name and operation name.
     */
    static I2C *getInstance();

    /*!
     \brief Read register data

     \fn readRegister
     \param addr Device address on the bus
     \param reg Register number
     \return uint8_t Subtracted value
    */
    uint8_t readRegister(uint8_t addr, uint8_t reg);

    /*!
     \brief Write register data

     \fn writeRegister
     \param addr Device address on the bus
     \param reg Register number
     \param val Value for writing
    */
    void writeRegister(uint8_t addr, uint8_t reg, uint8_t val);

    /*!
     \brief This method checks for errors

     \fn errorI2CAudio
     \param status Execution status
    */
    void errorI2CAudio(HAL_StatusTypeDef status);

    /*!
     * \brief hI2cHandler
     */
    I2C_HandleTypeDef *hI2cHandler;

    volatile FlagStatus i2c2ReadReady = RESET; /*! Status flag about reading data from I2C2 */
    volatile FlagStatus i2c2WriteReady = RESET; /*! Status flag about writing data to I2C2 */
    volatile FlagStatus i2c2Error = RESET; /*! Status flag about error in I2C2 */

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief I2CInit Initialization for I2C
 */
void i2cInitAudio(void);

#ifdef __cplusplus
}
#endif

#endif // I2C_H
