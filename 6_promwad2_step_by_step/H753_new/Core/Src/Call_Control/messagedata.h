#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include "cstdlib"
#include "cstdint"
#include "ArduinoJson.h"
#include "json.h"


struct MesssageFields {

    uint16_t ownId = 0,         /*!< The variable stores the ID of the calling device */
             prevOwnId = 0,     /*!< The variable stores the ID of the previous calling device */
             distId = 0,        /*!< The variable stores the ID of the called device */
             prevDistId = 0;    /*!< The variable stores the ID of the previous called device */
    uint8_t linkData = 0,       /*!< The variable stores the link status */
            priority = 0,       /*!< The variable stores the priority of the calling device */
            prevPriority = 0,   /*!< The variable stores the priority of the previous calling device */
            linkMode = 0,       /*!< The variable stores the link mode of the direct call (simplex or duplex) */
            callType = 0;       /*!< The variable stores a type of the incoming call */
//            distPriority = 0;   /*!< The variable stores a remote device priority */
};

class MessageData
{

public:
    MesssageFields field;           /*!< MesssageFields object */
    size_t distIdArrSize = 0;

    char * recvMessageBuff = nullptr;      /*!< The array that is used to store copy of the received buffer */
    char * busyMessageBuff = nullptr;

    const static size_t txBuffSize = 2048;  /*!< The size of the outputJson array  */
    char txBuff[txBuffSize];        /*!< The array that stores a JSON document to sand */

    /*!
     \brief Base constructor. A default constructor.

    \fn MessageData
            */
    MessageData() = default;

    /*!
     \brief Copy constructor.

    \fn MessageData
     \param
            */
    MessageData(const MessageData&) = default;

    /*!
     \brief Copy assignment operator

    \fn operator =
     \param
     \return MessageData &operator
        */
    MessageData& operator=(MessageData& arg) = default;
    /*!
     \brief The function parses received Json document and saves data to the variables to handle requests.

    \fn parse
     \param doc
     */
    void parse(JsonDocument&);

};

class MessageDataBuff
{

public:
    MesssageFields field;           /*!< MesssageFields object */

    char * recvMessageBuff = nullptr;      /*!< The array that is used to store copy of the received buffer */

    /*!
     \brief Base constructor. A default constructor.

    \fn MessageDataBuff
            */
    MessageDataBuff() = default;

    /*!
     \brief Base constructor. A default constructor.

    \fn MessageDataBuff
            */
    ~MessageDataBuff();

    /*!
     \brief Copy constructor.

    \fn MessageDataBuff
     \param
            */
    MessageDataBuff(const MessageDataBuff&) = default;

    /*!
     \brief Copy assignment operator

    \fn operator =
     \param
     \return MessageData &operator
        */
    MessageDataBuff& operator=(MessageDataBuff& arg) = default;
    /*!
     \brief The function clears the received message data.

    \fn clear
     \param
     \return
     */

    void clear();
};

#endif // MESSAGEDATA_H
