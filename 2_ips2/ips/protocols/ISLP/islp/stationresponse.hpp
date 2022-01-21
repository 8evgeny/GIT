#ifndef STATIONRESPONSE_H
#define STATIONRESPONSE_H

#include "basicmessage.hpp"
#include "intercomstation.hpp"

namespace ISLP {

  /**
   * @brief The stationResponse class
   * Represents an response message from station on @ref yellMsg "Yell()" request, and `Accept()` to @ref setStationMessage "setIP()" request.
   *
   * Contains serialized:
   * - Serial number.
   * - MAC address
   * - Type of station
   * - Network settings
   *
   * @note Masked by `key`
   */
  class stationResponse :public basicMessage
    {
    public:
      /**
       * @brief stationResponse Basic constructor from pointer to message.
       * @param data Pointer to input data
       * @param size Size of input data
       */
      stationResponse(uint8_t *data, size_t size);

      /**
       * @brief stationResponse Constructor for device support. Serializes message from intercomStation struct
       * @param station
       * @param key
       * @param is_accept sets fuction code Accept or StationInfo (default StationInfo)
       */

      stationResponse(const intercomStation &station, arcCrypt *key, bool is_accept=false);
      stationResponse(const std::vector<uint8_t> &other);
      stationResponse(std::vector<uint8_t> &&other);
      stationResponse(basicMessage &&other);
      stationResponse(const basicMessage &other);

      virtual ~stationResponse();

      stationResponse& operator=(const basicMessage &other);
      stationResponse& operator=(basicMessage &&other);

      stationResponse &operator=(const intercomStation &other);
      /**
       * @brief operator << moves object `basicMessage`.
       * @param other Movable object.
       * @return
       */
      stationResponse& operator<<(basicMessage &&other);

      /**
       * @brief operator () Returns pointer to @ref intercomStation "intercomStation" structure.
       * @param k Pointer to encryption object with the same key as in Yell() message.
       * @return Pointer to @ref intercomStation "intercomStation" if message is valid and `nullptr` otherwise.
       * @warning **Return structure dynamicly created. Don't forget delete it after use.**
       */
      intercomStation* operator ()(arcCrypt *k);
      /**
       * @brief isAccept Check is the message response to Yell or setIP.
       * @return `true` in case of response to setIP() `false` otherwise.
       */
      bool isAccept();
      /**
       * @brief validate Check is the message has valid size, structure, key and checksum.
       * @param key Pointer to encryption object with the same key as in Yell() message.
       * @return `true` in case of valid.
       */
      bool validate(arcCrypt *key);

#ifdef IPS_TESTING
    static std::vector<uint8_t> serialize(intercomStation station,bool is_valid=true);
#endif
    private:
      void serializeStation(const intercomStation &station,bool is_accept);
      bool isValid(arcCrypt *key);

    };

}
#endif // STATIONRESPONSE_H
