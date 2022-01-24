/*
 * station_basic_config.h - a part of ufw++ object envirinment for uControllers
 * microcontrollers.
 *
 *  Created on: 9 дек. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <common/serializer.h>
#include <containers/ufw_vector.h>



namespace ips
{
  
  class StationBasicConfig :public ufw::iSerializable
  {
	public:
	  static StationBasicConfig& Instance ();

	  char serial_number[12]
		{ }; //!< Serial number - uniqe identificator of device programmed at factory.
	  uint8_t macaddr[6]
		{ }; //!< MAC-address of device (programmed at factory).
	  uint16_t board_type
		{ }; //!< @ref IPS::TypeID "Type" identificator - shows type of device.
	  uint8_t ipaddr[4]
		{ }; //! IP-address.
	  uint8_t netmask[4]
		{ }; //! IP Netmask.
	  uint8_t gateway[4]
		{ }; //! IP Gateway.
	  char name[12]
		{ }; //! Symbolic name of device.
	  uint32_t checksum;

	  void Serialize(ByteArray& destination);

	private:
//      StationBasicConfig* pointer();

	  StationBasicConfig ();
	  ~StationBasicConfig () = delete;
	  StationBasicConfig (const StationBasicConfig &other) = delete;
	  StationBasicConfig (StationBasicConfig &&other) = delete;
	  StationBasicConfig& operator= (const StationBasicConfig &other) = delete;
	  StationBasicConfig& operator= (StationBasicConfig &&other) = delete;
  };

} /* namespace ips */

