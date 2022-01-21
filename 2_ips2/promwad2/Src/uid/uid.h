/*!
 \file uid.h
 \brief This file describes how to work with UID

        The unique device identifier is ideally suited:
            - for use as serial numbers (for example USB string serial numbers or other end
            applications)
            - for use as security keys in order to increase the security of code in Flash memory while
            using and combining this unique ID with software cryptographic primitives and
            protocols before programming the internal Flash memory
            - to activate secure boot processes, etc.

        The 96-bit unique device identifier provides a reference number which is unique for any
        device and in any context. These bits can never be altered by the user.
        The 96-bit unique device identifier can also be read in single bytes/half-words/words in
        different ways and then be concatenated using a custom algorithm.

 \authors Shulenkov R.A.

*/

#ifndef UID_H
#define UID_H

#include <cstdint>
#include "stm32f7xx_hal.h"

class UID;

/*!
 \brief It is designed to automatically destroy UID

 \class UIDDestroyer uid.h "uid.h"
*/
class UIDDestroyer
{
private:
    UID *pInstance; /*! Pointer to instance */

public:

    /*!
     \brief Base destructor.

     \fn ~UIDDestroyer
    */
    ~UIDDestroyer();

    /*!
     \brief Initialization for UID.

     \fn initialize
     \param p A pointer to instance
    */
    void initialize(UID *p);
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class UID uid.h "uid.h"
*/
class UID
{
private:
    static UID *pInstance; /*! Pointer to instance */
    static UIDDestroyer destroyer; /*! Destroyer for UID */

protected:
    /*!
     \brief Base constructor. A default constructor.

     \fn UID
    */
    UID() {}

    /*!
     \brief Copy constructor.

     \fn UID
     \param
    */
    UID(const UID &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return UID &operator
    */
    UID &operator=(UID &);

    /*!
     \brief Base destructor.

     \fn ~UID
    */
    ~UID() {}
    friend class UIDDestroyer;

public:
    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return UID. The public static operation can be accessed easily by using the class name and operation name.
    */
    static UID &getInstance();

    /*!
     \brief This function gets the first part of the UID.

     \fn getFirstPartOfUid
     \return It is the first part of the UID.
    */
    uint32_t getFirstPartOfUid()
    {
        return HAL_GetUIDw0();
    }

    /*!
     \brief This function gets the second part of the UID.

     \fn getSecondPartOfUid
     \return It is the second part of the UID.
    */
    uint32_t getSecondPartOfUid()
    {
        return HAL_GetUIDw1();
    }

    /*!
     \brief This function gets the third part of the UID.

     \fn getThirdPartOfUid
     \return It is the third part of the UID.
    */
    uint32_t getThirdPartOfUid()
    {
        return HAL_GetUIDw2();
    }

    /*!
     * \brief Simple test for UID
     * \fn test
     */
    void test();
};

#endif  // UID_H
