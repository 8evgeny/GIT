/*
 * test_system.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 20 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include "test_system.h"
#include "perf.hpp"
#include <lib_macros.hpp>
#include <allocation/heap/amazon_heap.h>
#include <containers/ufw_containers.h>
#include <system/configs/configplan/subscribers_list.h>

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#ifdef UFW_COUNT_ALLOCATIONS
extern unsigned int g_user_alloc_counts;
#else
static unsigned int g_user_alloc_counts=0;
#endif

#define UFW_CNT_SIZE (100)

static inline unsigned int getAllocs ()
{
  return g_user_alloc_counts;
}
static inline void clearAllocs ()
{
  g_user_alloc_counts = 0;
}

namespace ufw
{

  TestSystem& TestSystem::Instance ()
  {
    static TestSystem tests_inst;
    return tests_inst;
  }

  int TestSystem::TestAll ()
  {
    int result = 0;
    result += TestContainers ();
    result += TestMemories ();
    result += TestSerDes();
    return result;
  }

  TestSystem::TestSystem ()
  {
    output << "Test System: \nAllocations: " << getAllocs () << "\n";
    clearAllocs ();
  }

  TestSystem::~TestSystem ()
  {
  }

  int TestSystem::TestContainers ()
  {
    uint32_t it = UFW_CNT_SIZE;
    perf_t time_mark;
    output << "List Tests:\nC++17GNU, size" << it << "\n";
    std::list<uint32_t> t_list;
    output << "Fill..\n";
    PERFCHECK_START
    ;
    for (uint32_t i = 0; i < it; i++)
      {
	t_list.push_back (i);
      }
    PERFCHECK_STOP(time_mark);
    output << "Fill performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
    output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
    clearAllocs ();

    output << "Clear..\n";
    PERFCHECK_START
    ;
    t_list.clear ();
    PERFCHECK_STOP(time_mark);
    output << "Clear performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
    output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
    clearAllocs ();

    output << "Vector Tests:\nC++17GNU, size" << it << "\n";
    std::vector<uint32_t> t_vector;
    output << "Fill..\n";
    PERFCHECK_START;
    for (uint32_t i = 0; i < it; i++)
      {
	t_vector.push_back (i);
      }
    PERFCHECK_STOP(time_mark);
    output << "Fill performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
    output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
    clearAllocs ();

    output << "Clear..\n";
    PERFCHECK_START;
    t_vector.clear ();
    PERFCHECK_STOP(time_mark);
    output << "Clear performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
    output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
    clearAllocs ();

    output << "Vector Tests:\nUFW Lib v0.5, size" << it << "\n";
        ufw::ufwVector<uint32_t> t_ufvect; //vector<uint32_t> t_vector;
        output << "Fill..\n";
        PERFCHECK_START;
        for (uint32_t i = 0; i < it; i++)
          {
            t_ufvect.push_back (i);
          }
        PERFCHECK_STOP(time_mark);
        output << "Fill performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
        output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
    	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
        clearAllocs ();

        output << "Clear..\n";
        PERFCHECK_START;
        t_ufvect.clear ();
        PERFCHECK_STOP(time_mark);
        output << "Clear performed in:" << time_mark.us << "us(" << time_mark.tick << " CPU ticks)\n";
        output << "Allocation counts: " << getAllocs () << " \nFree Heap: "
    	<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
        clearAllocs ();

        return 0;

  }

  int TestSystem::TestMemories ()
  {
    output<<"Heap avaible: "<< heap::amazonHeap::instance ().xPortGetFreeHeapSize () << "\n";
    return 0;
  }
  
  int TestSystem::TestSerDes ()
  {
    output << "\nSerialisation ComPlan: \n";
    SubscribersList t_subscribers;
    uint32_t initial_ip=10000;
    uint32_t initial_cid=1000;
    for(int i=0;i<10;i++)
      {
	t_subscribers.add(initial_ip+i,initial_cid+i);
      }
    for(int i=0;i<10;i++)
      {
	output<<"IP="<<t_subscribers[i].ip()<<" CID=CID"<<t_subscribers[i].cid()<<"\n";
      }
    ByteArray t_serial;
    t_subscribers.Serialize(t_serial);
    SubscribersList t_deserialized;
    t_deserialized.Deserialize(t_serial.data());
    for(int i=0;i<10;i++)
      {
	output<<"IP="<<t_deserialized[i].ip()<<" CID=CID"<<t_deserialized[i].cid()<<"\n";
      }
    output<<"End test\n\n";
    return 0;
  }

} /* namespace ufw */
