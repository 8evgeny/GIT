/*!
 \file json.h

*/

#ifndef JSON_H
#define JSON_H

#include "sram.h"

#include "fsforeeprom.h"
#include "../Debug/debug.h"
#include "CircularBuffer.h"
#include <cstdlib>
#include <cstring>
#include "ArduinoJson.h"
#include "vector"

/*!
 * \brief The struct that stores fields of the keys
 *
 * \class Keys json.h "json.h"
 */
struct Keys {
    uint8_t function = 0, /*!< The field stores a key function */
            key = 0, /*!< The field stores a key number */
            type = 0, /*!< The field stores a assign type */
            priority = 0, /*!< The field stores key priority */
            mode = 0, /*!< The field stores key mode */
            directLinkMode = 0;
    uint16_t assign = 0; /*!< The field stores ID remote station */
};

/*!
 * \brief The struct that stores fields of the groups
 *
 * \class Groups json.h "json.h"
 */
struct Groups {
    uint16_t name = 1, /*!< The field stores a key function */
             stantions[20] = {0}, /*!< The field stores a key number */
                             stSize = 0;
};

/*!
 * \brief The struct stores the station configuration
 *
 * \class ThisStation json.h "json.h"
 */
struct ThisStation {
    char ip[17] = "192.168.1.2", /**< The field stores IP address of this station */
                  mask[17] = "255.255.255.0", /**< The field stores IP mask of this station */
                             gateway[17] = "192.168.1.1"; /**< The field stores IP gateway of this station */
    uint8_t keysNumber = 0; /**< The field stores number of the configured keys of this station */
    uint8_t groupsNumber = 0;
    uint16_t id = 100; /**< The field stores own ID this station */
    std::vector <Keys>keysBuffer; /**< The ring buffer for storage keys */
    std::vector <Groups>groupsBuffer;
};



/*!
 * \brief
 *
 * \class Json json.h "json.h"
 */
class Json
{
private:
    static Json * pInstance; /**< The static pointer to the Json class */

    /*!
     \brief Base constructor. A default constructor.

     \fn Json
    */
    Json();

    /*!
     \brief Copy constructor.

     \fn Json
     \param
    */
    Json(const Json&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return Json &operator
    */
    Json& operator=(Json&);
    /*!
     * \brief The function reads a JSON file from EEPROM
     *
     * \fn read
     */
    void read();

public:

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return Json. The public static operation can be accessed easily by using the class name and operation name.
    */
    static Json * getInstance();

    /*!
     * \brief The function parses JSON file and configures this station
     *
     * \fn parseJson
     */
    void configStation();
    /*!
     * \brief The function parses JSON file and configures this station
     *
     * \fn parseJson
     * \param doc reference to the allocated memory for JSON document
     * \param jsonBuff pointer to the buffer that stores JSON document for deserialize
     * \param size size of buffer
     * \return bool deserialization completion flag
     */
    bool deserialize(JsonDocument& doc, void* jsonBuff, size_t size);

    /*!
     * \brief The function parses JSON file and configures this station
     *
     * \fn parseJson
     */
    void parseJson();
    /*!
     \brief The function prepares and sends JSON document through multicast UDP

     \fn sendJson
     \param doc Json document
     \param capacity Json document capacity
    */
//    void sendJson(JsonDocument &doc, const size_t capacity, portType port);


    ThisStation thisStation; /*!< The object of the ThisStation class */


    /*!
     \brief The enumeration stores values of the deserialization completion flags

     \enum JsonFlags
    */
    enum class JsonFlags {
        OK = 0,
        ERR = -1
    };

    JsonFlags deserializeJsonFlag = JsonFlags::ERR; /*!< The flag that is deserialization completion */
    uint32_t  fileSize = 0; /**< The variable that stores JSON file size */
    uint32_t modifiedfileSize = 0; /**< The variable that stores modified JSON file size */

private:

    char *tempBuff = nullptr; /**< The pointer to the temporary data buffer */
};

#endif // JSON_H
