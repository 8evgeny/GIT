/*
 * ips_globals.hpp
 *
 *  Created on: 11 июн. 2020 г.
 *      Author: Donskikh Dmitry
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

#ifndef INC_IPS_GLOBALS_HPP_
#define INC_IPS_GLOBALS_HPP_

//#include <ttY.h>
#include <system/System.h>
#include <devices/stream_device.h>
#include <telnetServer.h>
#include <ipsUartDev.h>
#include <monitor.h>
#include <perf.hpp>

namespace ipsSystem
    {
    extern ufw::System& sys;
    extern ttY *seial;
    extern ttY *systerm;
    extern ttY *monterm;
    extern monitorTask *sysmonitor;
    extern ttY *debug;

    }

#endif /* INC_IPS_GLOBALS_HPP_ */
