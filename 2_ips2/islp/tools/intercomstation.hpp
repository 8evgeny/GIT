#ifndef INTERCOMSTATION_H
#define INTERCOMSTATION_H

#include <cstdint>
#include <vector>
#include <QString>
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
    void setupData(uint8_t *input);
    void setName(const char *newname);

    /**
     * @brief copy copies editable(by device) data fields
     * @param other reference to another structure
     */
    void copy(const intercomStation &other);

    void getSerial(char *data) const;
    void getMAC(uint8_t *data) const;
    uint32_t getIPaddr(uint8_t *data) const;

    QString getSerialStr() const;
    QString getMACStr() const;
    QString getTypeStr() const;
    QString getIPaddr() const;
    QString getNetmask() const;
    QString getGateway() const;
    QString getName() const;

    void print();

    std::vector<uint8_t> serialize();
    /**
     * @brief serialize Serialize structure to an vector given as pointer.
     * @param output
     * @warning without checks to nullptr. Without object creation.
     */
    void serialize(std::vector<uint8_t> *output) const;

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
    uint8_t key[4]
      { };
    bool isvalid = false;

  };

bool operator==(const intercomStation &lv, const intercomStation &rv);
bool operator!=(const intercomStation &lv, const intercomStation &rv);

#endif // INTERCOMSTATION_H
