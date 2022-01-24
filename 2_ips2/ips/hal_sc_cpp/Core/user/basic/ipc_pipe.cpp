/*
 * pipe.cpp
 *
 *  Created on: 5 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */
#include <ipc_pipe.hpp>
#include <ipssystem.hpp>
#include <cstring>
#ifdef NUCLEO_DEBUG
#include "main.h"
#endif

basicPipe::basicPipe (int size)
{
  bsize = size;
  buffer = (uint8_t*) system_cpp.getBuffer ();
  in_lock = osSemaphoreNew (1, 1, NULL);
  out_lock = osSemaphoreNew (1, 1, NULL);
  length = UFW_SYSTEM_GP_BUFFER_SIZE / bsize;
}

basicPipe::basicPipe (int size, uint8_t *buffer_ptr, int buffer_size)
{
  bsize = size;
  buffer = buffer_ptr;
  in_lock = osSemaphoreNew (1, 1, NULL);
  length = buffer_size / bsize;
  threshold = 3;
}

void basicPipe::push (uint8_t *num)
{
  isFull ();
  osSemaphoreAcquire (in_lock, osWaitForever);
  memcpy (buffer + write_pos * bsize, num, bsize);
  st_size++;
  write_pos = ((write_pos + 1) < length) ? (write_pos + 1) : 0;
  osSemaphoreRelease (in_lock);
  osSemaphoreRelease(out_lock);
}

void basicPipe::pushlk (uint8_t *num)
{
//	isFull();
//	osSemaphoreAcquire(in_lock, osWaitForever);
//  osSemaphoreRelease (in_lock);
  memcpy (buffer + write_pos * bsize, num, bsize);
  if (st_size < length)
    {
      st_size++;
      write_pos = ((write_pos + 1) < length) ? (write_pos + 1) : 0;
    }
  osSemaphoreRelease (in_lock);
  osSemaphoreRelease(out_lock);
}

void basicPipe::pop (uint8_t *dest)
{

  if (state_flag == IPC_PIPE_FLAG_READY)
    {
      if (st_size)
	{
	  read_pos = ((read_pos + 1) < length) ? (read_pos + 1) : 0;
	  st_size--;
	  osSemaphoreRelease (in_lock);
	  memcpy (dest, (buffer + ((read_pos != 0) ? (read_pos - 1) : (length - 1)) * bsize),
		  bsize);
	} else
	{
	  osSemaphoreRelease (in_lock);
	  memset (dest, 0, bsize);
	  state_flag = IPC_PIPE_FLAG_XRUN;
	}
    } else if (state_flag == IPC_PIPE_FLAG_XRUN)
    {
      memset (dest, 0, bsize);
      if (st_size >= threshold) state_flag = IPC_PIPE_FLAG_READY;
      osSemaphoreRelease (in_lock);
    }
}

void basicPipe::poplk (uint8_t *dest)
{

  while (st_size<=1)
    osDelay (1);
//  isEmpty();
  read_pos = ((read_pos + 1) < length) ? (read_pos + 1) : 0;
  st_size--;
  //osSemaphoreRelease (in_lock);
  memcpy (dest, (buffer + ((read_pos != 0) ? (read_pos - 1) : (length - 1)) * bsize), bsize);
  osSemaphoreRelease(in_lock);
  osSemaphoreRelease(out_lock);

}

void basicPipe::isFull ()
{
  if (st_size >= length)
    {
      osSemaphoreAcquire (in_lock, 1000);
    }
}

void basicPipe::isEmpty()
{
  if (st_size == 0)
    {
      osSemaphoreAcquire (out_lock, 1000);
    }
}

void basicPipe::clean ()
{
  osSemaphoreRelease (in_lock);
  st_size = 0;
  read_pos = 0;
  write_pos = 0;
}

int basicPipe::getSize ()
{
  return bsize;
}
