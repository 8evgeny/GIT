/*
 * test_system.h - a part of ufw++ object envirinment for
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

#ifndef UFW_TESTSYSTEM_TEST_SYSTEM_H_
#define UFW_TESTSYSTEM_TEST_SYSTEM_H_

#include <devices/terminal/ITMacrocell.hpp>

namespace ufw
{
  
  class TestSystem
    {
      public:
	static TestSystem& Instance();
	int TestAll();
      private:
	ufw::ioITM& output=ufw::ioITM::Instance();

	TestSystem ();
	~TestSystem ();
	TestSystem (const TestSystem &other) = delete;
	TestSystem (TestSystem &&other) = delete;
	TestSystem& operator= (const TestSystem &other) = delete;
	TestSystem& operator= (TestSystem &&other) = delete;
	int TestContainers();
	int TestMemories();
	int TestSerDes();



    };

} /* namespace ufw */

#endif /* UFW_TESTSYSTEM_TEST_SYSTEM_H_ */
