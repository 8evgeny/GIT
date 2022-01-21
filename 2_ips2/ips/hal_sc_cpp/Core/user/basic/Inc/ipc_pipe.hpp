/*
 * pipe.hpp
 *
 *  Created on: 5 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_PIPE_HPP_
#define USER_BASIC_INC_PIPE_HPP_


//#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <cinttypes>

#define NUCLEO_DEBUG

#define IPC_PIPE_FLAG_READY	0
#define IPC_PIPE_FLAG_XRUN	(-1)

typedef void* Mutex_t;


class basicPipe {
public:
	basicPipe(int size);
	basicPipe(int size,uint8_t* buffer_ptr,int buffer_size);
	void push(uint8_t *num);
	void pop(uint8_t *dest);
	void poplk(uint8_t *dest);
	void pushlk (uint8_t *num);
	void clean();
	int getSize();
protected:
	osSemaphoreId_t in_lock,out_lock;
	int length;
	int bsize;
	int st_size=0;
	int read_pos=0;
	int write_pos=0;
	uint8_t *buffer=nullptr;
	void isFull();
	void isEmpty();
	int8_t state_flag=IPC_PIPE_FLAG_XRUN;
	int threshold=3;


};


#endif /* USER_BASIC_INC_PIPE_HPP_ */
