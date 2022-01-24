/*
 * device_config.cpp
 *
 *  Created on: 19 дек. 2020 г.
 *      Author: vader
 */

#include <system/configs/device_config.h>
#include <system/configs/ip_setings.h>
#include <devices/flash/flashI2C.h>
#include <devices/crypto/hw_hash.h>
#include <system/netservices/init_cfg.h>
#include <system/configs/config_types.h>
#include <system/configs/fallback/fallbacks.h>
#include <system/System.h>
static ufw::System& Self = ufw::System::instance();

char g_ufw_mac_valid=0;
uint8_t g_ufw_mac_settings[6];
char g_ufw_ip_valid=0;
uint8_t *g_ufw_ip_settings;
uint8_t *g_ufw_msk_settings;
uint8_t *g_ufw_gwsettings;

static ufw::flashI2C* l_storage=nullptr;

namespace ufw
{
  /**
   *
   * @return
   */
  DeviceConfig& DeviceConfig::Instance ()
  {
    static DeviceConfig s_config;
    return s_config;
  }

  /**
   *
   * @return
   */
  DeviceConfig* DeviceConfig::pointer ()
  {
    return this;
  }
  
  /**
   *
   */
  DeviceConfig::DeviceConfig ()
  :intercomStation()
  {
    l_storage=ufw::flashI2C::Instance().pointer();
    if(loadSys())
      {
	g_ufw_mac_valid=0xFA;
	memcpy(&g_ufw_mac_settings[0],&macaddr[0],sizeof(macaddr));
	m_valid=false;
      }
    else
      {
	g_ufw_mac_valid=0;
	//Write Fallback////////////////
	//ufw::initCFG::Instance();
	fbk::FlashFallbackSys();
	osDelay(100);
	Self.reboot();
	m_valid=false;
      }
    if(!loadIp())
      {
	//Write Fallback////////////////
	//Wait
//	fbk::FlashFallBackIP();
//	Self.reboot();
//	this->setIpSettings(IpSetings());
      }
    g_ufw_ip_settings=&ipaddr[0];
    g_ufw_gwsettings=&gateway[0];
    loadName();
  }
  
  /**
   *
   * @return
   */
  bool DeviceConfig::loadSys ()
  {
    ByteArray raw(staticConfig::size());
    md5 stored_key;
    l_storage->readSystem(raw.data(), raw.size(), CfgOffset::FactorySettings);
    l_storage->readSystem(stored_key.data(),stored_key.size(),CfgOffset::FactoryHash);
    if(!Self.Hash().check(raw.data(), raw.size(), stored_key))
      {
	Self.stddebug()<<"Fatal pizdez: factory nastroyki ne sovpadayt. Load fallback\n";
	return false;
      }
    staticConfig temp;
    this->setUnique(temp.Deserialize(raw.data()));
    return true;
  }
  
  /**
   *
   * @return
   */
  bool DeviceConfig::loadIp ()
  {
    ByteArray raw(IpSetings::size());
    md5 stored_key;
    l_storage->byteRead(raw.data(), CfgOffset::IpConfig, raw.size());
    l_storage->byteRead(stored_key.data(), CfgOffset::IpConfigHash, stored_key.size());
    IpSetings temp;
    if(!Self.Hash().check(raw.data(), raw.size(), stored_key))
      {
	Self.stddebug()<<"Error: checksum mismatch IP Settings.Load fallback\n";
	this->setIpSettings(temp);
	return false;
      }
    this->setIpSettings(IpSetings().Deserialize(raw.data()));
    return true;
  }
  
  /**
   *
   * @return
   */
  bool DeviceConfig::loadName ()
  {
    ByteArray raw(sizeof(name));
    md5 stored_key;
    l_storage->byteRead(raw.data(), CfgOffset::Name, raw.size());
    l_storage->byteRead(stored_key.data(), CfgOffset::NameHash, stored_key.size());
    if(!Self.Hash().check(raw.data(), raw.size(), stored_key))
          {
    	Self.stddebug()<<"Info:Name loading checksum mismatch \n";
    	return false;
          }
        this->setName((const char*)raw.data());
        return true;

  }
  
  /**
   *
   * @param other
   * @return
   */
  bool DeviceConfig::saveIp (IpSetings& other)
  {
//    IpSetings temp=*this;

    md5 key2store;
    Self.Hash().compute(other,&key2store);
    l_storage->byteWrite(other.Serialize().data(), CfgOffset::IpConfig, IpSetings::size());
    osDelay(100);
    l_storage->byteWrite(key2store.data(), CfgOffset::IpConfigHash, key2store.size());
    osDelay(100);
    return true;
  }
  
  bool DeviceConfig::saveIp (intercomStation &other)
  {
    IpSetings temp(other);
    md5 key2store;
    Self.Hash().compute(temp,&key2store);
    l_storage->byteWrite(temp.Serialize().data(), CfgOffset::IpConfig, IpSetings::size());
    osDelay(100);
    l_storage->byteWrite(key2store.data(), CfgOffset::IpConfigHash, key2store.size());
    osDelay(100);
    return true;
  }

  /**
   *
   * @param other
   * @return
   */
  bool DeviceConfig::saveName (intercomStation& other)
  {
    md5 key2store=Self.Hash().compute(&name[0],sizeof(name));
    l_storage->byteWrite(&name[0], CfgOffset::Name, sizeof(name));
    l_storage->byteWrite(key2store.data(), CfgOffset::NameHash, key2store.size());
    return true;
  }
  


  DeviceConfig::~DeviceConfig ()
  {
  }
  
  void DeviceConfig::syncSystem(intercomStation& other)
  {
    if(other==*this){
	saveIp(other);
	saveName(other);
	Self.reboot();
    }
  }
  


} /* namespace ufw */
