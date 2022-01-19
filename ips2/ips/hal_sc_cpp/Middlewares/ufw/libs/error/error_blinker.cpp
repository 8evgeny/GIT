/*
 * error_blinker.cpp - a part of ufw++ object envirinment for
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

#define BLINKER1_LED	0x0108
#define BLINKER2_LED	0x010A

#include <error/error_blinker.h>

namespace ufw
{
  
  blinker::blinker ()
  :ipcThread("ErrorBlinker", 64, osPriorityLow3)
  {
  }
  
  blinker::~blinker ()
  {
    vTaskDelete(getTaskHandle());
  }
  
  void blinker::taskRunFcn ()
  {
    beg_iter=error_led.begin();
    iter=error_led.begin();
    out.setDO(BLINKER1_LED);
    out.resetDO(BLINKER2_LED);
    while(1){
	osDelay(100);
	tick();
    }
  }
  
  void blinker::tick ()
  {

    if((++iter)>error_led.end())iter=error_led.begin();
    if(*iter)
      {
	out.setDO(BLINKER2_LED);out.resetDO(BLINKER1_LED);
	sysLedNormToggle();
      }
    else
      {
	out.setDO(BLINKER1_LED);out.resetDO(BLINKER2_LED);
      }
  }

} /* namespace ufw */
