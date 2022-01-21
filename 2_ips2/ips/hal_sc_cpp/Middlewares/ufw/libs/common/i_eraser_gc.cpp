/*
 * i_eraser_gc.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 25 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <common/i_eraser_gc.h>
#include <system/System.h>

namespace ufw
{
  
  i_EraserGC::~i_EraserGC ()
  {
    ufw::System::instance().stddebug()<<__PRETTY_FUNCTION__<<"\n";
  }

} /* namespace ufw */
