/*
 * byte_stream.h - a part of ufw++ object envirinment for uControllers
 * microcontrollers.
 *
 *  Created on: 10 дек. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef LIBS_CONTAINERS_BYTE_STREAM_H_
#define LIBS_CONTAINERS_BYTE_STREAM_H_

#include "ufw_vector.h"
#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace ufw
{
  
  class byteStream : public ufwVector<uint8_t>
  {
	public:
	  byteStream ();
	  ~byteStream ();
	  byteStream (const byteStream &other) = delete;
	  byteStream (byteStream &&other) = delete;
	  byteStream& operator= (const byteStream &other) = delete;
	  byteStream& operator= (byteStream &&other) = delete;
  };

} /* namespace ufw */

#endif /* LIBS_CONTAINERS_BYTE_STREAM_H_ */
