/*
 * ipssystem.hpp
 *
 *  Created on: 9 июл. 2020 г.
 *      Author: Donskikh Dmitriy
 */

#ifndef USER_INC_IPSSYSTEM_HPP_
#define USER_INC_IPSSYSTEM_HPP_

#include <system/System.h>

//#include "inttypes.h"
//
//#define IPS_SYSTEM_GP_BUFFER_SIZE 	2*1024
//#define IPS_SYSTEM_UNCACHEBLE_SIZE 	32*1024
//#define IPS_SYSTEM_GP_BUFFER_COUNT	16
//
////typedef uint8_t[IPS_GP_BUFFER_SIZE] gp_buffer_t;
//
//class ipcMemory
//{
//public:
//
//	ipcMemory(void);
//	void configMPU(void);
//	void* useGPBuffer();
//	void freeGPBuffer(void *addr);
//	void* allocateUncacheble(size_t size);
//	uint32_t getFreeUncacheble();
//
//	int	free_buffers=IPS_SYSTEM_GP_BUFFER_COUNT;
//
//	enum {
//		MemLocked,
//		MemFree,
//	};
//private:
//
//	uint8_t uncacheble[IPS_SYSTEM_UNCACHEBLE_SIZE];
//	uint8_t* uncacheble_pos=&uncacheble[0];
//	uint8_t gp_buffer_pool[IPS_SYSTEM_GP_BUFFER_COUNT][IPS_SYSTEM_GP_BUFFER_SIZE];
//	char	gp_buffer_state[IPS_SYSTEM_GP_BUFFER_COUNT];
//
//};
//
//class ipcSystem
//{
//public:
//	ipcSystem(void);
//	void memoryInit(void);
//	void* getBuffer();
//	void freeBuffer(void* buffer);
//	int getFreeBuffers();
//	void* allocateDmaBuffer(size_t size);
//	void reboot();
//private:
//	ipcMemory memory;
//};
//
//extern ipcSystem system_cpp;
extern ufw::System& system_cpp;

#endif /* USER_INC_IPSSYSTEM_HPP_ */
