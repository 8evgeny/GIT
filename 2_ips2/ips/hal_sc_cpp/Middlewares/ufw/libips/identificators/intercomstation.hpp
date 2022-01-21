#pragma once

#include <containers/ufw_vector.h>
#include <stddef.h>
#include <sys/_stdint.h>
#include <io/net/network_helpers.h>

namespace ufw
{
  class IpSetings;
  class staticConfig;
} /* namespace ufw */

/**
 * @brief The intercomStation structure.
 *
 * Serialized data structure:
 * |Serial number|MAC-address| Type  |   IP   |  Mask  | Gateway |  Name  |
 * |:-----------:|:---------:|:-----:|:------:|:------:|:-------:|:------:|
 * |12 bytes     |6 bytes    |2 bytes|4 bytes |4 bytes |4 bytes  |12 bytes|
 */
class intercomStation
  {
  public:
    /**
     * @brief intercomStation Constructs structure filled by zeros.
     */
    intercomStation();
    /**
     * @brief intercomStation Constructs structure with data deserialized from input ptr.
     * @param input
     * @warning without any checks of size or content.
     */
    intercomStation(uint8_t *input);
    /**
     * @brief setupData Deserialize data from pointer to structure.
     * @param input
     * @warning without any checks of size or content.
     */

    static constexpr size_t size()
    {
      return sizeof(intercomStation); //44 b
    }
    void setupData(uint8_t *input);
    void setName(const char *newname);
    void setIpSettings(ufw::IpSetings& config);

    /**
     * @brief copy copies editable(by device) data fields
     * @param other reference to another structure
     */
    void copy(const intercomStation &other);


    void getSerial(char *data) const;
    void getMAC(uint8_t *data) const;
    uint16_t type() const;
    uint32_t getIPaddr(uint8_t *data) const;

//    QString getSerialStr() const;
//    QString getMACStr() const;
//    QString getTypeStr() const;
//    QString getIPaddr() const;
//    QString getNetmask() const;
//    QString getGateway() const;
//    QString getName() const;

//    void print();


    /**
     * @brief serialize Serialize structure to an vector given as pointer.
     * @param output
     * @warning without checks to nullptr. Without object creation.
     */
    void serialize(ByteArray *output) const;
    ByteArray serialize();

    ufw::setial_t_view serial;
    ufw::mac_t_view mac;


    uint8_t ipaddr[4]
      { }; //! IP-address.
    uint8_t netmask[4]
      { }; //! IP Netmask.
    uint8_t gateway[4]
      { }; //! IP Gateway.
    char name[12]
      { }; //! Symbolic name of device.
//    uint8_t key[4]
//      { };
//    bool isvalid = false;
  protected:
    char serial_number[12]
      { }; //!< Serial number - uniqe identificator of device programmed at factory.
    uint8_t macaddr[6]
      { }; //!< MAC-address of device (programmed at factory).
    uint16_t board_type
      { }; //!< @ref IPS::TypeID "Type" identificator - shows type of device.


    void setUnique(ufw::staticConfig& config);

  };

bool operator==(const intercomStation &lv, const intercomStation &rv);
bool operator!=(const intercomStation &lv, const intercomStation &rv);

