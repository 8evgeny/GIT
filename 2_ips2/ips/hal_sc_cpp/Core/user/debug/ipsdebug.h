/*!
 \file debug.h

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


#pragma once

#include <cstdint>
#include "CircularBuffer.h"
#include <string>
#include <array>

//class Debug;

//class ConsoleDebug
//{
//
//
//
//public:
//
//    ConsoleDebug() {}
//
//    ~ConsoleDebug() {}
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, char data);
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned char data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, short data);
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned short data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, int data);
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned int data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, long int data);
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, unsigned long int data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, const char *data);
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, const std::string &data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, const unsigned char *data);
//
//    friend ConsoleDebug &operator<<(ConsoleDebug &console, double data);
//
//    ConsoleDebug &operator<<(ConsoleDebug & (*data)(ConsoleDebug &));
//};
///*
//class DebugDestroyer
//{
//private:
//    Debug *pInstance;
//public:
//
//    ~DebugDestroyer();
//
//    void initialize(Debug *p);
//};
//*/
//
//class Debug
//{
//private:
//    static Debug *pInstance; /*! Pointer to instance */
//    //static DebugDestroyer destroyer; /*! Destroyer for Debug */
//protected:
//
//    Debug();
//    Debug(const Debug &);
//    Debug &operator=(Debug &);
//    ~Debug() { }
//    //friend class DebugDestroyer;
//public:
//    ConsoleDebug dbg; /*! Object for ITM */
//
//
//    static constexpr uint32_t sizeCircularBuffer = 512;
//    CircularBuffer <char, sizeCircularBuffer> ringBuffer;
//
//    static Debug &getInstance();
//
//    void test();
//};


#ifdef __cplusplus
extern "C" {
#endif

void debugInit(void);

#ifdef __cplusplus
}
#endif

void ipsDebugITM();

