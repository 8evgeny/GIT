/*!
 \file debug.cpp

 \authors Shulenkov R.A.

 \code
    debugInit();

    ...

    uint32_t i  = 0;

    while (1) {
        Debug::getInstance().dbg << 123 <<  " " << i << " " << "1234567890\n";
        Debug::getInstance().dbg << 456 <<  " " << i << " " << "1234567890\n";
        Debug::getInstance().dbg << 789 <<  " " << i << " " << "1234567890\n";
    }
 \endcode

*/

#include "debug.h"
#include "debug_printf.h"
#include <cstdint>


#include <stm32h7xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif

static void writeByte(unsigned char byte)
{
    ITM_SendChar(static_cast<uint32_t>(byte));
}

/*!
  \brief Debug Initialization Function
  \param None
  \retval None
  */
void debugInit(void)
{
    debugDevOut(writeByte);
}

#ifdef __cplusplus
}
#endif

Debug::Debug()
{
}

Debug *Debug::pInstance = nullptr;
DebugDestroyer Debug::destroyer;

DebugDestroyer::~DebugDestroyer()
{
    delete pInstance;
}

void DebugDestroyer::initialize(Debug *p)
{
    pInstance = p;
}

Debug &Debug::getInstance()
{
    if (!pInstance)     {
        pInstance = new Debug();
        destroyer.initialize(pInstance);
    }
    return *pInstance;
}

/*!
 \brief Custom output stream for debugging [const char *]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, const char *data)
{
    debugPrintf("%s", data);
    return console;
}


ConsoleDebug &operator<<(ConsoleDebug &console, const std::string &data)
{
    debugPrintf("%s", data.c_str());
    return console;
}


/*!
 \brief Custom output stream for debugging [const unsigned char *]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, const unsigned char *data)
{
    debugPrintf("%s", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [unsigned char]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, unsigned char data)
{
    debugPrintf("%u", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [char]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, char data)
{
    debugPrintf("%c", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [short]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, short data)
{
    debugPrintf("%h", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [unsigned short]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, unsigned short data)
{
    debugPrintf("%h", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [int]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, int data)
{
    debugPrintf("%d", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [unsigned int]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, unsigned int data)
{
    debugPrintf("%u", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [long int]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, long int data)
{
    debugPrintf("%l", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [unsigned long int]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, unsigned long int data)
{
    debugPrintf("%ul", data);
    return console;
}

/*!
 \brief Custom output stream for debugging [double]
*/
ConsoleDebug &operator<<(ConsoleDebug &console, double data)
{
    debugPrintf("%f", data);
    return console;
}

/*!
 \brief Custom output stream for debugging
*/
ConsoleDebug &ConsoleDebug::operator<<(ConsoleDebug & (*data)(ConsoleDebug &))
{
    return (*data)(*this);
}


void Debug::test()
{
    //simple test for ITM
    auto simpleTest = [](ConsoleDebug & dbg, uint32_t iter) -> void {
        for (uint32_t i = 0; i < iter ; i++)
        {
            dbg << 123 <<  " " << i << " " << "1234567890\n";
            dbg << 456 <<  " " << i << " " << "1234567890\n";
            dbg << 789 <<  " " << i << " " << "1234567890\n";
        }
    };

    simpleTest(this->dbg, 100);
}
