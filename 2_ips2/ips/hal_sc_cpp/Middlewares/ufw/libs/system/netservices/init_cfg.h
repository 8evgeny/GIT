/*
 * init_cfg.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 23 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_NETSERVICES_INIT_CFG_H_
#define UFW_LIBS_SYSTEM_NETSERVICES_INIT_CFG_H_

#include <ipcThread.h>

namespace ufw
{
  
  class initCFG :public ipcThread
    {
      public:
	initCFG ();
      	~initCFG ();
	static initCFG& Instance();
	initCFG* pointer();

	void taskRunFcn()override;

      private:

	initCFG (const initCFG &other) = delete;
	initCFG (initCFG &&other) = delete;
	initCFG& operator= (const initCFG &other) = delete;
	initCFG& operator= (initCFG &&other) = delete;

	bool proceed(uint8_t *data,size_t size);

    };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_NETSERVICES_INIT_CFG_H_ */
