/*!
 \file debug.h

 \authors Shulenkov R.A.

*/

#ifndef DEBUG_H
#define DEBUG_H

#include <cstdint>
#include "CircularBuffer.h"
#include <string>
#include <array>

class Debug;

/*!
 \brief Class for debugging custom output stream

 \class ConsoleDebug debug.h "debug.h"
*/
class ConsoleDebug
{



public:
    /*!
     \brief Base constructor. A default constructor.

     \fn ConsoleDebug
    */
    ConsoleDebug() {}

    /*!
     \brief Base destructor.

     \fn ~ConsoleDebug
    */
    ~ConsoleDebug() {}

    friend ConsoleDebug &operator<<(ConsoleDebug &console, char data);
    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned char data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, short data);
    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned short data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, int data);
    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned int data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, long int data);
    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned long int data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, const char *data);
    friend ConsoleDebug &operator<<(ConsoleDebug &console, const std::string &data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, const unsigned char *data);

    friend ConsoleDebug &operator<<(ConsoleDebug &console, double data);

    ConsoleDebug &operator<<(ConsoleDebug & (*data)(ConsoleDebug &));
};

/*!
 \brief It is designed to automatically destroy Debug

 \class DebugDestroyer debug.h "debug.h"
*/
class DebugDestroyer
{
private:
    Debug *pInstance; /*! Pointer to instance */
public:

    /*!
     \brief Base destructor.

     \fn ~DebugDestroyer
    */
    ~DebugDestroyer();

    /*!
     \brief Initialization for Debug.

     \fn initialize
     \param p A pointer to instance
    */
    void initialize(Debug *p);
};

/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class Debug debug.h "debug.h"
*/
class Debug
{
private:
    static Debug *pInstance; /*! Pointer to instance */
    static DebugDestroyer destroyer; /*! Destroyer for Debug */
protected:

    /*!
     \brief Base constructor. A default constructor.

     \fn Debug
    */
    Debug();

    /*!
     \brief Copy constructor.

     \fn Debug
     \param
    */
    Debug(const Debug &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return Debug &operator
    */
    Debug &operator=(Debug &);

    /*!
     \brief Base destructor.

     \fn ~Debug
    */
    ~Debug() { }
    friend class DebugDestroyer;
public:
    ConsoleDebug dbg; /*! Object for ITM */


    static constexpr uint32_t sizeCircularBuffer = 512;
    CircularBuffer <char, sizeCircularBuffer> ringBuffer;

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return Debug The public static operation can be accessed easily by using the class name and operation name.
    */
    static Debug &getInstance();

    /*!
     * \brief Simple test for debug ITM
     */
    void test();
};


#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief Initialization of the mechanism for displaying messages through ITM

 \fn debugInit
*/
void debugInit(void);

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H
