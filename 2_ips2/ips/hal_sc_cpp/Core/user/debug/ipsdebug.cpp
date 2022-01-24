/*!
 \file debug.cpp

*/

/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <ipsdebug.h>
#include "debug_printf.h"
#include <cstdint>
#include "FreeRTOS.h"

#ifdef STM32F777xx
#include <stm32f7xx_hal.h>
#else
#include <stm32h7xx_hal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

static void writeByte(unsigned char byte)
{
    ITM_SendChar((uint32_t)byte);
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
//
//Debug::Debug()
//{
//}
//
//Debug *Debug::pInstance = nullptr;
////DebugDestroyer Debug::destroyer;
///*
//DebugDestroyer::~DebugDestroyer()
//{
//    delete pInstance;
//}
//
//void DebugDestroyer::initialize(Debug *p)
//{
//    pInstance = p;
//}
//*/
//Debug &Debug::getInstance()
//{
//    if (!pInstance)     {
//        //pInstance = new Debug();
//    	pInstance=(Debug*)pvPortMalloc(sizeof(Debug));
//    	//*pInstance = &Debug();
//        //destroyer.initialize(pInstance);
//    }
//    return *pInstance;
//}
//
///*!
// \brief Custom output stream for debugging [const char *]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, const char *data)
//{
//    debugPrintf("%s", data);
//    return console;
//}
//
//
//ConsoleDebug &operator<<(ConsoleDebug &console, const std::string &data)
//{
//    debugPrintf("%s", data.c_str());
//    return console;
//}
//
//
///*!
// \brief Custom output stream for debugging [const unsigned char *]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, const unsigned char *data)
//{
//    debugPrintf("%s", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [unsigned char]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, unsigned char data)
//{
//    debugPrintf("%u", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [char]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, char data)
//{
//    debugPrintf("%c", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [short]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, short data)
//{
//    debugPrintf("%h", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [unsigned short]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, unsigned short data)
//{
//    debugPrintf("%h", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [int]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, int data)
//{
//    debugPrintf("%d", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [unsigned int]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, unsigned int data)
//{
//    debugPrintf("%u", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [long int]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, long int data)
//{
//    debugPrintf("%l", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [unsigned long int]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, unsigned long int data)
//{
//    debugPrintf("%ul", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging [double]
//*/
//ConsoleDebug &operator<<(ConsoleDebug &console, double data)
//{
//    debugPrintf("%f", data);
//    return console;
//}
//
///*!
// \brief Custom output stream for debugging
//*/
//ConsoleDebug &ConsoleDebug::operator<<(ConsoleDebug & (*data)(ConsoleDebug &))
//{
//    return (*data)(*this);
//}
//
//
//void Debug::test()
//{
//    //simple test for ITM
//    auto simpleTest = [](ConsoleDebug & dbg, uint32_t iter) -> void {
//        for (uint32_t i = 0; i < iter ; i++)
//        {
//            dbg << 123 <<  " " << i << " " << "1234567890\n";
//            dbg << 456 <<  " " << i << " " << "1234567890\n";
//            dbg << 789 <<  " " << i << " " << "1234567890\n";
//        }
//    };
//
//    simpleTest(this->dbg, 100);
//}
//
//void ipsDebugITM()
//{
//	debugInit();
//}
//
