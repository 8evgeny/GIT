/*!
 \file trng.h

 \authors Shulenkov R.A.
*/
#ifndef TRNG_H
#define TRNG_H

#include <cstdint>
#include "stm32f7xx_hal.h"

class TRNG;

/*!
 \brief It is designed to automatically destroy TRNG

 \class TRNGDestroyer trng.h "trng.h"
*/
class TRNGDestroyer
{
private:
    TRNG *pInstance; /*! Pointer to instance */
public:
    /*!
     \brief Base destructor.

     \fn ~TRNGDestroyer
     */
    ~TRNGDestroyer();

    /*!
     \brief Initialization for TRNG

     \fn initialize
     \param p A pointer to instance
    */
    void initialize(TRNG *p);
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

\class TRNG trng.h "trng.h"
*/
class TRNG
{
private:
    static TRNG *pInstance;
    static TRNGDestroyer destroyer;
protected:

    /*!
     \brief Base constructor. A default constructor.

     \fn TRNG
    */
    TRNG();

    /*!
     \brief Copy constructor.

     \fn TRNG
     \param
    */
    TRNG(const TRNG &);


    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return TRNG &operator
    */
    TRNG &operator=(TRNG &);

    /*!
     \brief Base destructor.

     \fn ~TRNG
     */
    ~TRNG() { }
    friend class TRNGDestroyer;
public:
    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return TRNG. The public static operation can be accessed easily by using the class name and operation name.
    */
    static TRNG &getInstance();

    /*!
     * \brief This function is needed to get a random value.
     * \return A random value.
     */
    uint32_t random();
    RNG_HandleTypeDef *handle; /*! Pointer to RNG_HandleTypeDef */

    /*!
     * \brief Simple test for TRMG
     * \fn test
     */
    void test();
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief rngInit Initialization for TRNG
 * \fn rngInit
 */
void rngInit();

#ifdef __cplusplus
}
#endif

#endif // TRNG_H
