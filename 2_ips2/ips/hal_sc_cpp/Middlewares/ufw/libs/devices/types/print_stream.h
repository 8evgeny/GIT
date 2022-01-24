/*
 * print_stream.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */



#ifndef LIBS_DEVICES_TYPES_PRINT_STREAM_H_
#define LIBS_DEVICES_TYPES_PRINT_STREAM_H_


#ifdef PLATFORM_TEST
#include <devices/stream_device.h>

namespace ufw {
  namespace dev {

    /*
     *
     */
    class printStream :public ufw::dev::streamDevice
      {
      public:
	printStream();


	void write_t(const char *buf, size_t size) override;
	//virtual void printfl(const char *fmt, ...) final;
	~printStream();
      private:
        void resizeBuffer(size_t size) override;

	printStream(const printStream &other) = delete;
	printStream(printStream &&other) = delete;
	printStream& operator=(const printStream &other) = delete;
	printStream& operator=(printStream &&other) = delete;
      };

  } /* namespace dev */
}
#endif
#endif /* LIBS_DEVICES_TYPES_PRINT_STREAM_H_ */
