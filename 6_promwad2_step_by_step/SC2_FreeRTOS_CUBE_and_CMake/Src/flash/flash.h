/*!
 \file flash.h

 \authors Shulenkov R.A.
*/
#ifndef FLASH_H
#define FLASH_H

#include <cstdint>
#include "ff_gen_drv.h"

class Flash;

/*!
 \brief It is designed to automatically destroy Flash

\class FlashDestroyer flash.h "flash.h"
    */
class FlashDestroyer
{
private:
    Flash *pInstance;/*! Pointer to instance */

public:
    /*!
     \brief Base destructor.

    \fn ~FlashDestroyer
    */
    ~FlashDestroyer();

    /*!
     \brief Initialization for Flash.

    \fn initialize
    \param p A pointer to instance
    */
    void initialize(Flash *p);
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

\class Flash flash.h "flash.h"
    */
class Flash
{
private:
    static Flash *pInstance; /*! Pointer to instance */
    static FlashDestroyer destroyer; /*! Destroyer for UID */

protected:
    /*!
     \brief Base constructor. A default constructor.

     \fn Flash
    */
    Flash() { }
    /*!
     \brief Copy constructor.

     \fn UID
     \param
    */
    Flash(const Flash &);
    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return UID &operator
    */
    Flash &operator=(Flash &);
    /*!
     \brief Base destructor.

     \fn ~Flash
    */
    ~Flash() { }
    friend class FlashDestroyer;
public:
    static constexpr uint32_t IFLASH_PAGE_SIZE = 256;
    static constexpr uint32_t FATFS_SIZE = 0xC0000; //768 Kbytes

    static constexpr uint32_t ADDR_FLASH_SECTOR_0 = 0x08000000; /*! Base address of Sector 0, 32 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_1 = 0x08008000; /*! Base address of Sector 1, 32 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_2 = 0x08010000; /*! Base address of Sector 2, 32 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_3 = 0x08018000; /*! Base address of Sector 3, 32 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_4 = 0x08020000; /*! Base address of Sector 4, 128 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_5 = 0x08040000; /*! Base address of Sector 5, 256 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_6 = 0x08080000; /*! Base address of Sector 6, 256 Kbytes */
    static constexpr uint32_t ADDR_FLASH_SECTOR_7 = 0x080C0000; /*! Base address of Sector 7, 256 Kbytes */

    static constexpr uint32_t ADDR_FLASH_BANK_2 =  0x08100000; /*! Base address of bank 2 */

    /*!
     \brief It returns the sole instance of the class.
     \fn getInstance
     \return Flash. The public static operation can be accessed easily by using the class name and operation name.
    */
    static Flash &getInstance();

    /*!
     \brief Copy data from the flash memory to a @p buffer.
     \warning The @p buffer must be at least @p size bytes long.
     \param addr First address of the flash memory to be copied.
     \param buf Buffer to copy to.
     \param size Size of the data to be copied in bytes.
    */
    void read(uint32_t addr, char *buf, uint32_t size);

    /*!
     \brief Copy data from a @p buffer to the flash memory.
     \warning The flash memory area receiving the data must be erased.
     \warning The @p buffer must be at least @p size bytes long.
     \param addr First address in the flash memory where to copy the data to.
     \param buf Buffer containing the data to copy.
     \param size Size of the data to be copied in bytes.
    */
    void write(uint32_t addr, const char *buf, uint32_t size);

    /*!
     \brief Copy data from a @p buffer to the flash memory. Size 256 byte.
     \warning The flash memory area receiving the data must be erased.
     \warning The @p buffer must be at least @p size bytes long.
     \param addr First address in the flash memory where to copy the data to.
     \param data Buffer containing the data to copy.
    */
    void write(void *addr, void *data);

    /*!
     \brief Check if the data in @p buffer are identical to the one in flash memory.
     \param address First address in flash memory to be checked.
     \param buffer Buffer containing the data to compare.
     \param size Size of @p buffer in bytes.
     \return TRUE if the flash memory and the buffer contain identical data.
     \return FALSE if the flash memory and the buffer don't contain identical data.
    */
    bool compare(uint32_t address, const uint8_t *buffer, uint32_t size);

    /*!
     \brief  Locks the FLASH control register access
     \fn lock
    */
    void lock();

    /*!
     \brief  Locks the FLASH control register access
     \fn lock
    */
    void unlock();

    /*!
     * \brief Unlock the FLASH Option Control Registers access.
     * \fn flush
     */
    void flush();

    /*!
     * \brief Simple test for Flash
     * \fn test
     */
    void test();

    /*!
     \brief Erase the area of the new firmware

     \fn erase
    */
    void erase();

    /*!
     \brief Erase the area of the old (current) firmware

     \fn eraseForUpdate
    */
    void eraseForUpdate();

};

#ifdef __cplusplus
extern "C" {
#endif

extern Diskio_drvTypeDef FLASH_Driver;

#ifdef __cplusplus
}
#endif

#endif // FLASH_H
