#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstddef>

#include <allocation/heap/amazon_heap.h>


struct testAllocateResults
  {
    size_t size = 0;
    double alloc_time_us = 0;
    double free_time_us = 0;
  };

struct testHeapResults
  {
    testAllocateResults myAlloc;
    testAllocateResults stdAlloc;
  };

void printHeapTests(const testHeapResults &results);

testHeapResults testAmazonHeapPice(heap::amazonHeap &heap_manager, size_t size);
