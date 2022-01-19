/*
 * ipssystem.cpp
 *
 *  Created on: 9 июл. 2020 г.
 *      Author: Donskikh Dmitriy
 */
#include "main.h"
#include "string.h"
#include "ipssystem.hpp"
//
//#define STM32_FLASH_BANK2	0x08100000
//
//extern uint32_t _iobufferss;
//
//ipcMemory::ipcMemory(void)
//    {
//    memset(&uncacheble, 0x00, sizeof(uncacheble));
//    memset(&gp_buffer_pool, 0x00, sizeof(gp_buffer_pool));
//    memset(&gp_buffer_state, ipcMemory::MemFree, sizeof(gp_buffer_state));
//    }
//
//void ipcMemory::configMPU(void)
//    {
//
//    MPU_Region_InitTypeDef MPU_InitStruct;
//
//    /* Disables the MPU */
//    HAL_MPU_Disable();
//
//    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//    MPU_InitStruct.Number = MPU_REGION_NUMBER4;
//    MPU_InitStruct.BaseAddress = (uint32_t) &uncacheble;
//    MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
//    MPU_InitStruct.SubRegionDisable = 0x0;
//    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
//    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
//
//    HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
////		MPU_InitStruct.Enable = MPU_REGION_ENABLE;
////		MPU_InitStruct.Number = MPU_REGION_NUMBER3;
////		MPU_InitStruct.BaseAddress = STM32_FLASH_BANK2;
////		MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
////		MPU_InitStruct.SubRegionDisable = 0x0;
////		MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
////		MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
////		MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
////		MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
////		MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
////		MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
////
////		HAL_MPU_ConfigRegion(&MPU_InitStruct);
//    /**Initializes and configures the Region and the memory to be protected for Ethernet descriptors
//     */
//    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
//    MPU_InitStruct.BaseAddress = 0x30000000;
//    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
//    MPU_InitStruct.SubRegionDisable = 0x0;
//    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
//
//    HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
//    /**Initializes and configures the Region and the memory to be protected for LwIP buffers
//     */
//    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
//    MPU_InitStruct.BaseAddress = 0x30004000;
//    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
//    MPU_InitStruct.SubRegionDisable = 0x0;
//    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
//    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//
//    HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
//    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
//    MPU_InitStruct.BaseAddress = 0x30000000;
//    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
//    MPU_InitStruct.SubRegionDisable = 0x0;
//    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
//    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//
//    HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
//    /* Enables the MPU */
//    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
//
//    }
//
//void* ipcMemory::useGPBuffer()
//    {
//    for (uint8_t i = 0; i < IPS_SYSTEM_GP_BUFFER_COUNT; i++)
//	{
//	if (gp_buffer_state[i] != ipcMemory::MemLocked)
//	    {
//	    gp_buffer_state[i] = ipcMemory::MemLocked;
//	    free_buffers--;
//	    return (void*) &gp_buffer_pool[i][0];
//	    }
//	}
//    return NULL;
//    }
//
//void ipcMemory::freeGPBuffer(void *addr)
//    {
//    for (uint8_t i = 0; i < IPS_SYSTEM_GP_BUFFER_COUNT; i++)
//	{
//	if ((addr >= &gp_buffer_pool[i][0])
//		&& (addr < &gp_buffer_pool[i][IPS_SYSTEM_GP_BUFFER_SIZE]))
//	    {
//	    gp_buffer_state[i] = ipcMemory::MemFree;
//	    memset(&gp_buffer_pool[i][0], 0x00, IPS_SYSTEM_GP_BUFFER_SIZE);
//	    free_buffers++;
//	    return;
//	    }
//	}
//
//    }
//
//void* ipcMemory::allocateUncacheble(size_t size)
//    {
//    if (getFreeUncacheble() > size)
//	{
//	uint8_t *l_buffer = uncacheble_pos;
//	uncacheble_pos += (size + size % 4);
//	return memset(l_buffer, 0, size);
//	}
//    else
//	{
//	return NULL;
//	}
//    }
//
//uint32_t ipcMemory::getFreeUncacheble()
//    {
//    return IPS_SYSTEM_UNCACHEBLE_SIZE - (uncacheble_pos - &uncacheble[0]);
//    }
//
//ipcSystem::ipcSystem(void)
//    {
//
//    }
//
//void ipcSystem::memoryInit(void)
//    {
//
//    memory.configMPU();
//    }
//
//void* ipcSystem::getBuffer()
//    {
//    return memory.useGPBuffer();
//    }
//
//void ipcSystem::freeBuffer(void *buffer)
//    {
//    memory.freeGPBuffer(buffer);
//    }
//
//int ipcSystem::getFreeBuffers()
//    {
//    return memory.free_buffers;
//    }
//
//void ipcSystem::reboot()
//    {
//    NVIC_SystemReset();
//    }
//
//void* ipcSystem::allocateDmaBuffer(size_t size)
//    {
//    return memory.allocateUncacheble(size);
//    }
