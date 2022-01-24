#ifndef SETSTATIONMESSAGE_H
#define SETSTATIONMESSAGE_H

#include <protocols/basic_message.h>
//#include <devices/crypto/arcstylecrypt.hpp>
#include <system/configs/device_config.h>
//#include <containers/ufw_vector.h>

namespace ISLP {

  /**
   * @brief The setStationMessage class
   * Message to change station IP/Mask/Gateway and symbolic name. Sends back @ref intercomStation "intercomStation" structure. If station have the same Serial Number, it changes their settings, otherwise ignore the message.
   * @warning You must use the same Serial Number and Key as given from Yell response.
   *
   */
  class setStationMessage :public basicMessage
    {
    public:
      /**
       * @brief setStationMessage constructs message from an intercomStation structure.
       * @param station
       * @warning don't change Serial Number and MAC-address.
       */
      setStationMessage( intercomStation &station);

      /**
       * @brief operator () returns a pointer to serialised message.
       * @return
       */
      uint8_t* operator()();

      friend int getStationFromMsg(intercomStation &station, setStationMessage &t_msg,
				   ufw::arcCrypt *decoder);

      virtual ~setStationMessage();
#ifdef IPS_TESTING
    void modMAC(const IPS::MAC& mac_addr);
    void modSerial(const IPS::SerialNumber& ser_num);
    void modType(uint16_t st_type);
    uint8_t* getKey();
    static intercomStation validate(setStationMessage *t_msg);
#endif
    private:
      intercomStation& m_station;
    };

  int getStationFromMsg(intercomStation &station, setStationMessage &t_msg, ufw::arcCrypt *decoder);
}
#endif // SETSTATIONMESSAGE_H
