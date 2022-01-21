#include <tests/benchmark.h>
#include <iostream>

#include <tests/nanotimer/plf_nanotimer.h>

void printHeapTests(const testHeapResults &results)
{
  std::cout << results.myAlloc.size << " bytes allocate: " << results.myAlloc.alloc_time_us << "("
      << results.stdAlloc.alloc_time_us << ")";
  std::cout << " deallocate: " << results.myAlloc.free_time_us << "("
      << results.stdAlloc.free_time_us << ")\n";
}

testHeapResults testAmazonHeapPice(heap::amazonHeap &heap_manager, size_t size)
{
  plf::nanotimer timer;
  testHeapResults result;
  result.myAlloc.size = size;
  result.stdAlloc.size = size;
  uint8_t *my_alloct = nullptr;
  uint8_t *std_alloct = nullptr;
  timer.start ();
  my_alloct = (uint8_t*) heap_manager.pvPortMalloc (size);
  result.myAlloc.alloc_time_us = timer.get_elapsed_ns ();
  if(my_alloct == nullptr)
    {
      std::cout << "nullptr caught without error\n";
      throw std::bad_alloc();
      exit (-200);
    }
  timer.start ();
  heap_manager.vPortFree (my_alloct);
  result.myAlloc.free_time_us = timer.get_elapsed_ns ();

  timer.start ();
  std_alloct = (uint8_t*) std::malloc (size);
  result.stdAlloc.alloc_time_us = timer.get_elapsed_ns ();
  timer.start ();
  std::free (std_alloct);
  result.stdAlloc.free_time_us = timer.get_elapsed_ns ();

  printHeapTests (result);
  return result;
}
