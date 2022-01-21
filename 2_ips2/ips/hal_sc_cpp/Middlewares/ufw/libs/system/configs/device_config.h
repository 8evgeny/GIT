/*
 * device_config.h
 *
 *  Created on: 19 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_SYSTEM_CONFIGS_DEVICE_CONFIG_H_
#define UFW_LIBS_SYSTEM_CONFIGS_DEVICE_CONFIG_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <identificators/intercomstation.hpp>


namespace ufw
{

  class DeviceConfig :public intercomStation
    {
      public:
	static DeviceConfig& Instance();
	DeviceConfig* pointer();

	void syncSystem(intercomStation& other);

	bool saveIp(IpSetings& other);
      private:
	bool loadSys();
	bool loadIp();
	bool loadName();

	bool saveIp(intercomStation& other);
	bool saveName(intercomStation& other);
	bool m_valid=false;

//	staticConfig *factory=nullptr;
//	IpSetings *ipcfg=nullptr;

	DeviceConfig ();
	~DeviceConfig ();
	DeviceConfig (const DeviceConfig &other) = delete;
	DeviceConfig (DeviceConfig &&other) = delete;
	DeviceConfig& operator= (const DeviceConfig &other) = delete;
	DeviceConfig& operator= (DeviceConfig &&other) = delete;
    };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_CONFIGS_DEVICE_CONFIG_H_ */
