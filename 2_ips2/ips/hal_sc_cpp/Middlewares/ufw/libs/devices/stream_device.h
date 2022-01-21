/*
 * stream_device.hpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace ufw {
  namespace dev {
#define MAX_TERM_LINE (2*1024)

#define TERM_RES_VT100 80*24
    /*
     *
     */
    class streamDevice
      {
      protected:
	char *linebuffer = nullptr;
	size_t b_size = MAX_TERM_LINE;
      public:
	streamDevice();
	streamDevice(void *buf, size_t size);
	streamDevice(size_t size);

	virtual ~streamDevice();
	virtual void write_t(const char *buf, size_t size)=0;
	virtual void printfl(const char *fmt, ...);

	void cls();
	void endl();
	void flush();
	void s_close();
    protected:
        virtual void resizeBuffer(size_t size);
      };

  } /* namespace dev */
}
using ttY = ufw::dev::streamDevice;

ttY& operator<<(ttY &terminal, char data);
ttY& operator<<(ttY &terminal, unsigned char data);

ttY& operator<<(ttY &terminal, short data);
ttY& operator<<(ttY &terminal, unsigned short data);

ttY& operator<<(ttY &terminal, int data);
ttY& operator<<(ttY &terminal, unsigned int data);

ttY& operator<<(ttY &terminal, long int data);
ttY& operator<<(ttY &terminal, unsigned long int data);

ttY& operator<<(ttY &terminal, const char *data);
//friend ttY &operator<<(ttY &terminal, const std::string &data);

ttY& operator<<(ttY &terminal, const unsigned char *data);

ttY& operator<<(ttY &terminal, double data);
ttY& operator<<(ttY &terminal, void *data);
