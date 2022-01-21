/*
 * amazon_heap.h
 * is an part of ufw - C++ framework for microcontrollers.
 * Implements FreeRTOS heap management rewriten on C++ (C++17 preferred)
 *
 *  Created on: 29 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 *
 *  Heap 4 is an part of FreeRTOS Kernel V10.2.1
 *  Copyright(C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef LIBS_ALLOCATION_HEAP_AMAZON_HEAP_H_
#define LIBS_ALLOCATION_HEAP_AMAZON_HEAP_H_

#ifndef userBYTE_ALIGNMENT
#define userBYTE_ALIGNMENT 8
#endif

#ifndef userBYTE_ALIGNMENT_MASK
#define userBYTE_ALIGNMENT_MASK 0x0007
#endif

#ifndef userspaceHEAP_SIZE
#define userspaceHEAP_SIZE (100*1024)
#endif

#include <cstdlib>
#include <cstdint>

namespace heap
{
  struct BlockLink_t
  {
      BlockLink_t *pxNextFreeBlock;
      size_t xBlockSize;
  };
  
  /**
   * \class amazonHeap
   * \brief Amazon.com FreeRTOS heap4 likee heap manager rewriten on C++
   *
   */
  class amazonHeap
  {
    private:
      amazonHeap ();
      amazonHeap (const amazonHeap &other) = delete;
      amazonHeap (amazonHeap &&other) = delete;
      amazonHeap& operator= (const amazonHeap &other) = delete;
      amazonHeap& operator= (amazonHeap &&other) = delete;

//      amazonHeap* self=nullptr;

      void heapError ();

      void prvInsertBlockIntoFreeList (BlockLink_t *pxBlockToInsert);

      void prvHeapInit (void);
      void vPortInitialiseBlocks ();

      /* The size of the structure placed at the beginning of each allocated memory
       block must by correctly byte aligned. */
      const size_t xHeapStructSize = (sizeof(BlockLink_t) + ((size_t) ( userBYTE_ALIGNMENT - 1)))
	  & ~((size_t) userBYTE_ALIGNMENT_MASK);

      /* Create a couple of list links to mark the start and end of the list. */
      BlockLink_t xStart, *pxEnd = nullptr;

      /* Keeps track of the number of free bytes remaining, but says nothing about
       fragmentation. */
      size_t xFreeBytesRemaining = 0U;
      size_t xMinimumEverFreeBytesRemaining = 0U;

      /* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
       member of an BlockLink_t structure is set then the block belongs to the
       application.  When the bit is free the block is still part of the free heap
       space. */
      size_t xBlockAllocatedBit = 0;

    public:
      static amazonHeap& instance ();

      void* pvPortMalloc (size_t xWantedSize);
      void vPortFree (void *pv);
      size_t xPortGetFreeHeapSize ();
      size_t xPortGetMinimumEverFreeHeapSize ();
      void HeapFullHook();



      static constexpr
      size_t capacity ();
      constexpr uint8_t* begin ();
      constexpr uint8_t* end ();

      bool isOwned (uint8_t *ptr);

    protected:
      uint8_t baseHeap[ userspaceHEAP_SIZE] =
	{ };
  };

} /* namespace heap */

#endif /* LIBS_ALLOCATION_HEAP_AMAZON_HEAP_H_ */
