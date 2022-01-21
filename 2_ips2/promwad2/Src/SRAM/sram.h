/*!
 \file sram.h

*/

#ifndef SRAM_H
#define SRAM_H

#include "stm32f7xx_hal.h"
#include "../Debug/debug.h"


/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class SRAM sram.h "sram.h"
*/
class SRAM
{
private:
    static SRAM * pInstance; /*!< TODO: describe */

    /*!
     \brief Base constructor. A default constructor.

     \fn CAN
    */
    SRAM();

    /*!
     \brief Copy constructor.

     \fn CAN
     \param
    */
    SRAM(const SRAM&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return CAN &operator
    */
    SRAM& operator=(SRAM&);

public:

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return CAN. The public static operation can be accessed easily by using the class name and operation name.
    */
    static SRAM * getInstance();

    /*!
     * \brief The function reads data from the SRAM
     * \param dataBuf pointer to data buffer
     * \param bufSize size of the data buffer
     * \param addr start address of SRAM
     * \return Status of reading
     */
    HAL_StatusTypeDef readData(uint32_t *dataBuf, size_t bufSize, uint32_t *addr);

    /*!
     * \brief The function writes data to the SRAM
     * \param dataBuf pointer to data buffer
     * \param bufSize size of the data buffer
     * \param addr start address of SRAM
     * \return Status of writing
     */
    HAL_StatusTypeDef writeData(uint32_t *data, size_t dataSize, uint32_t *addr);

    SRAM_HandleTypeDef *sram1Handle; /*!< The variable is pointer to the SRAM Handle Type */
    volatile bool sramDataReady = RESET; /*!< Data availability status flag  */

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief SRAMInit Initialization
 */
void SRAMInit(void);


#ifdef __cplusplus
}
#endif

#endif //SRAM_H
