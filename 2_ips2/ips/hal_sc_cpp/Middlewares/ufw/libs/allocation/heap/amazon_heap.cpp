/*
 * amazon_heap.cpp
 *
 *  Created on: 29 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "amazon_heap.h"
#include <lib_asserts.h>
#include <system/System.h>

#ifdef PLATFORM_TEST

  #include <iostream>
  #define AMAZON_HEAP_FAILED  std::cout<<"heap fucked up\n";throw std::bad_alloc(); exit(-100);
#else

  #define AMAZON_HEAP_FAILED PLATFORM_MEM_EXCEPTION;
#endif

#define userheapASSERT(x) if((x)!=true) heapError();

#define userheapMINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize << 1 ) )
#define userheapBITS_PER_BYTE		( ( size_t ) 8 )



namespace heap {
  
  amazonHeap::amazonHeap()
  {
    prvHeapInit();
  }

  void amazonHeap::heapError()
  {
    AMAZON_HEAP_FAILED
  }
  
  /*
   * Inserts a block of memory that is being freed into the correct position in
   * the list of free memory blocks.  The block being freed will be merged with
   * the block in front it and/or the block behind it if the memory blocks are
   * adjacent to each other.
   */
  void amazonHeap::prvInsertBlockIntoFreeList(BlockLink_t *pxBlockToInsert)
  {
    BlockLink_t *pxIterator;
    uint8_t *puc;

    /* Iterate through the list until a block is found that has a higher address
     than the block being inserted. */
    for(pxIterator = &xStart;pxIterator->pxNextFreeBlock < pxBlockToInsert;
	pxIterator = pxIterator->pxNextFreeBlock)
      {
	/* Nothing to do here, just iterate to the right position. */
      }

    /* Do the block being inserted, and the block it is being inserted after
     make a contiguous block of memory? */
    puc = (uint8_t*) pxIterator;
    if((puc + pxIterator->xBlockSize) == (uint8_t*) pxBlockToInsert)
      {
	pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
	pxBlockToInsert = pxIterator;
      }

    /* Do the block being inserted, and the block it is being inserted before
     make a contiguous block of memory? */
    puc = (uint8_t*) pxBlockToInsert;
    if((puc + pxBlockToInsert->xBlockSize) == (uint8_t*) pxIterator->pxNextFreeBlock)
      {
	if(pxIterator->pxNextFreeBlock != pxEnd)
	  {
	    /* Form one big block from the two blocks. */
	    pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
	    pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
	  }else
	  {
	    pxBlockToInsert->pxNextFreeBlock = pxEnd;
	  }
      }else
      {
	pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
      }

    /* If the block being inserted plugged a gab, so was merged with the block
     before and the block after, then it's pxNextFreeBlock pointer will have
     already been set, and should not be set here as that would make it point
     to itself. */
    if(pxIterator != pxBlockToInsert)
      {
	pxIterator->pxNextFreeBlock = pxBlockToInsert;
      }
  }

  /*
   * Called automatically to setup the required heap structures the first time
   * pvPortMalloc() is called.
   */
  void amazonHeap::prvHeapInit(void)
  {
    BlockLink_t *pxFirstFreeBlock;
    uint8_t *pucAlignedHeap;
    size_t uxAddress;
    size_t xTotalHeapSize = userspaceHEAP_SIZE;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uxAddress = (size_t) baseHeap;

    if((uxAddress & userBYTE_ALIGNMENT_MASK) != 0)
      {
        uxAddress += ( userBYTE_ALIGNMENT - 1);
        uxAddress &= ~((size_t) userBYTE_ALIGNMENT_MASK);
	xTotalHeapSize -= uxAddress - (size_t) baseHeap;
      }

    pucAlignedHeap = (uint8_t*) uxAddress;

    /* xStart is used to hold a pointer to the first item in the list of free
     blocks.  The void cast is used to prevent compiler warnings. */
    xStart.pxNextFreeBlock = reinterpret_cast<BlockLink_t*>(pucAlignedHeap);
    xStart.xBlockSize = (size_t) 0;

    /* pxEnd is used to mark the end of the list of free blocks and is inserted
     at the end of the heap space. */
    uxAddress = ((size_t) pucAlignedHeap) + xTotalHeapSize;
    uxAddress -= xHeapStructSize;
    uxAddress &= ~((size_t) userBYTE_ALIGNMENT_MASK);
    pxEnd = reinterpret_cast<BlockLink_t*>(uxAddress);
    pxEnd->xBlockSize = 0;
    pxEnd->pxNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
     entire heap space, minus the space taken by pxEnd. */
    pxFirstFreeBlock = reinterpret_cast<BlockLink_t*>(pucAlignedHeap);
    pxFirstFreeBlock->xBlockSize = uxAddress - (size_t) pxFirstFreeBlock;
    pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

    /* Only one block exists - and it covers the entire usable heap space. */
    xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

    /* Work out the position of the top bit in a size_t variable. */
    xBlockAllocatedBit = ((size_t) 1) << ((sizeof(size_t) * userheapBITS_PER_BYTE) - 1);
  }
  
  amazonHeap& amazonHeap::instance()
  {
    static amazonHeap s_instance;
    return s_instance;
  }
  
  void* amazonHeap::pvPortMalloc(size_t xWantedSize)
  {
    BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = nullptr;

      {
	/* If this is the first call to malloc then the heap will require
	 initialisation to setup the list of free blocks. */
	userheapASSERT(pxEnd != nullptr)
	if((xWantedSize>xFreeBytesRemaining)||(xFreeBytesRemaining<userheapMINIMUM_BLOCK_SIZE))
	  {
	    HeapFullHook();
	    //return nullptr;
	  }
	/* Check the requested block size is not so large that the top bit is
	 set.  The top bit of the block size member of the BlockLink_t structure
	 is used to determine who owns the block - the application or the
	 kernel, so it must be free. */
	if((xWantedSize & xBlockAllocatedBit) == 0)
	  {
	    /* The wanted size is increased so it can contain a BlockLink_t
	     structure in addition to the requested amount of bytes. */
	    if(xWantedSize > 0)
	      {
		xWantedSize += xHeapStructSize;

		/* Ensure that blocks are always aligned to the required number
		 of bytes. */
		if((xWantedSize & userBYTE_ALIGNMENT_MASK) != 0x00)
		  {
		    /* Byte alignment required. */
		    xWantedSize += ( userBYTE_ALIGNMENT - (xWantedSize & userBYTE_ALIGNMENT_MASK));
		    userheapASSERT ((xWantedSize & userBYTE_ALIGNMENT_MASK) == 0);
		  }
	      }

	    if((xWantedSize > 0) && (xWantedSize <= xFreeBytesRemaining))
	      {
		/* Traverse the list from the start	(lowest address) block until
		 one	of adequate size is found. */
		pxPreviousBlock = &xStart;
		pxBlock = xStart.pxNextFreeBlock;
		while ((pxBlock->xBlockSize < xWantedSize) && (pxBlock->pxNextFreeBlock != NULL))
		  {
		    pxPreviousBlock = pxBlock;
		    pxBlock = pxBlock->pxNextFreeBlock;
		  }

		/* If the end marker was reached then a block of adequate size
		 was	not found. */
		if(pxBlock != pxEnd)
		  {
		    /* Return the memory space pointed to - jumping over the
		     BlockLink_t structure at its start. */
		    pvReturn = (void*) (((uint8_t*) pxPreviousBlock->pxNextFreeBlock)
			+ xHeapStructSize);

		    /* This block is being returned for use so must be taken out
		     of the list of free blocks. */
		    pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

		    /* If the block is larger than required it can be split into
		     two. */
		    if((pxBlock->xBlockSize - xWantedSize) > userheapMINIMUM_BLOCK_SIZE)
		      {
			/* This block is to be split into two.  Create a new
			 block following the number of bytes requested. The void
			 cast is used to prevent byte alignment warnings from the
			 compiler. */
			pxNewBlockLink = reinterpret_cast<BlockLink_t*>(((uint8_t*) pxBlock) + xWantedSize);
			userheapASSERT ((((size_t) pxNewBlockLink) & userBYTE_ALIGNMENT_MASK) == 0);

			/* Calculate the sizes of two blocks split from the
			 single block. */
			pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
			pxBlock->xBlockSize = xWantedSize;

			/* Insert the new block into the list of free blocks. */
			prvInsertBlockIntoFreeList (pxNewBlockLink);
		      }

		    xFreeBytesRemaining -= pxBlock->xBlockSize;

		    if(xFreeBytesRemaining < xMinimumEverFreeBytesRemaining)
		      {
			xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
		      }

		    /* The block is being returned - it is allocated and owned
		     by the application and has no "next" block. */
		    pxBlock->xBlockSize |= xBlockAllocatedBit;
		    pxBlock->pxNextFreeBlock = NULL;
		  }
	      }
	  }

      }

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
    	{
                if( pvReturn == nullptr )
    		{
    			//extern void vApplicationMallocFailedHook( void );
    			//vApplicationMallocFailedHook();
    		}
    	}
    	#endif

    userheapASSERT ((((size_t) pvReturn) & (size_t) userBYTE_ALIGNMENT_MASK) == 0);
    //ITM_SENDFRAME(xPortGetFreeHeapSize());
    return pvReturn;

  }
  
  void amazonHeap::vPortFree(void *pv)
  {
    if(!isOwned((uint8_t*)pv))return;
    uint8_t *puc = (uint8_t*) pv;
    BlockLink_t *pxLink;

    if(pv != NULL)
      {
	/* The memory being freed will have an BlockLink_t structure immediately
     before it. */
	puc -= xHeapStructSize;

	/* This casting is to keep the compiler from issuing warnings. */
	pxLink = reinterpret_cast<BlockLink_t*> (puc);

	/* Check the block is actually allocated. */
	if(!((pxLink->xBlockSize & xBlockAllocatedBit) != 0))return;
	userheapASSERT(pxLink->pxNextFreeBlock == nullptr);

	if((pxLink->xBlockSize & xBlockAllocatedBit) != 0)
	  {
	    if(pxLink->pxNextFreeBlock == nullptr)
	      {
		/* The block is being returned to the heap - it is no longer
	     allocated. */
		pxLink->xBlockSize &= ~xBlockAllocatedBit;
		{
		  /* Add this block to the list of free blocks. */
		  xFreeBytesRemaining += pxLink->xBlockSize;
		  prvInsertBlockIntoFreeList (((BlockLink_t*) pxLink));
		}
	      }
	  }
      }
    //ITM_SENDFRAME(xPortGetFreeHeapSize());
  }
  
  size_t amazonHeap::xPortGetFreeHeapSize(void)
  {
    return xFreeBytesRemaining;
  }
  
  size_t amazonHeap::xPortGetMinimumEverFreeHeapSize(void)
  {
    return xMinimumEverFreeBytesRemaining;
  }
  
  void amazonHeap::vPortInitialiseBlocks(void)
  {
#ifdef PLATFORM_TEST
    std::cout<<"vPortInitialiseBlocks(void)\n";/* This just exists to keep the linker quiet. */
#endif
  }
  
  constexpr size_t amazonHeap::capacity ()
  {
    return userspaceHEAP_SIZE;
  }
  
  constexpr uint8_t* amazonHeap::begin ()
  {
    return &baseHeap[0];
  }
  
  constexpr uint8_t* amazonHeap::end ()
  {
    return &baseHeap[capacity()];
  }
  
  void amazonHeap::HeapFullHook ()
  {
    ufw::System::instance().stddebug()<<"Hooked NOMEM\n";
  }

  bool amazonHeap::isOwned (uint8_t *ptr)
  {
    return ((ptr>=begin())&&(ptr<=end()));

  }

} /* namespace heap */
