/*!
 \file fsforeeprom.h

 \authors Shulenkov R.A.
*/
#ifndef FSFOREEPROM_H
#define FSFOREEPROM_H

#include <cstdint>
#include "lfs.h"
class FsForEeprom;

/*!
 \brief It is designed to automatically destroy FsForEeprom

 \class FsForEeprom fsforeeprom.h "fsforeeprom.h"
*/
class FsForEepromDestroyer
{
private:
    FsForEeprom *pInstance; /*! Pointer to instance */
public:
    /*!
     \brief Base destructor.

     \fn ~FsForEepromDestroyer
    */
    ~FsForEepromDestroyer();

    /*!
     \brief Initialization for FsForEeprom.

     \fn initialize
     \param p A pointer to instance
    */
    void initialize(FsForEeprom *p);
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

\class FsForEeprom fsforeeprom.h "fsforeeprom.h"
*/
class FsForEeprom
{
private:
    static FsForEeprom *pInstance; /*! Pointer to instance */
    static FsForEepromDestroyer destroyer; /*! Destroyer for FsForEeprom */
protected:
    /*!
     \brief Base constructor. A default constructor.

     \fn FsForEeprom
    */
    FsForEeprom();

    /*!
     \brief Copy constructor.

     \fn FsForEeprom
     \param
    */
    FsForEeprom(const FsForEeprom &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return FsForEeprom &operator
    */
    FsForEeprom &operator=(FsForEeprom &);

    /*!
    \brief Base destructor.

    \fn ~FsForEeprom
    */
    ~FsForEeprom() { }
    friend class FsForEepromDestroyer;
public:
    static FsForEeprom &getInstance();

    ///\todo check after taking our customer boards

    /*!
    \brief  Initializes peripherals used by the I2C EEPROM driver.
    \retval EEPROM_OK (0) if operation is correctly performed, else return value
            different from EEPROM_OK (0)
    */
    uint32_t init(void);

    /*!
    \brief  Reads a block of data from the EEPROM.
    \param  pBuffer: pointer to the buffer that receives the data read from
             the EEPROM.
    \param  ReadAddr: EEPROM's internal address to start reading from.
    \param  NumByteToRead: pointer to the variable holding number of bytes to
            be read from the EEPROM.

           \note The variable pointed by NumByteToRead is reset to 0 when all the
                 data are read from the EEPROM. Application should monitor this
                 variable in order know when the transfer is complete.

    \retval EEPROM_OK (0) if operation is correctly performed, else return value
            different from EEPROM_OK (0) or the timeout user callback.
    */
    uint32_t read(uint8_t *pBuffer, uint16_t readAddr, uint16_t *numByteToRead);

    /*!
    \brief  Writes buffer of data to the I2C EEPROM.
    \param  pBuffer: pointer to the buffer  containing the data to be written
            to the EEPROM.
    \param  WriteAddr: EEPROM's internal address to write to.
    \param  NumByteToWrite: number of bytes to write to the EEPROM.
    \retval EEPROM_OK (0) if operation is correctly performed, else return value
            different from EEPROM_OK (0) or the timeout user callback.
    */
    uint32_t write(uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);

    /*!
     * \brief Simple test for FsForEeprom
     * \fn test
     */
    void test();

    lfs_t *lfsPtr;
    lfs_file_t *filePtr;
};

#ifdef __cplusplus
extern "C" {
#endif

void littleFsInit(void);

#ifdef __cplusplus
}
#endif

#endif // FSFOREEPROM_H
