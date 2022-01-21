/*
 * print_stream.cpp
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "print_stream.h"
#include <cstring>

#ifdef PLATFORM_TEST
#include <cstdio>
#include <exception>
#define PRINTSTREAM_TEST_SIZE (2*1024)
#define PRINTSTREAM_TEST_FAIL(msg) printf(msg);throw std::bad_exception();
#else
#endif

#ifdef PLATFORM_TEST
namespace ufw
    {

    namespace dev
	{

	printStream::printStream() :
		ufw::dev::streamDevice(malloc(PRINTSTREAM_TEST_SIZE),
			PRINTSTREAM_TEST_SIZE)
	    {
	    memset(linebuffer, 0x00, b_size);
	    }

	void printStream::resizeBuffer(size_t size)
	    {
	    PRINTSTREAM_TEST_FAIL("could not resize buffer\n")
	    }

	void printStream::write_t(const char *buf, size_t size)
	    {
	    printf("%s", buf);
	    }

	printStream::~printStream()
	    {
	    free(linebuffer);
	    }

	} /* namespace dev */
    }
#endif
