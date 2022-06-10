/*!
 \file callcontrol.h

*/
#ifndef CALLCONTROL_H
#define CALLCONTROL_H

#include "missedcall.h"
#include "messagedata.h"
#include "../AUDIO/microphone.h"
#ifndef SC2BOARD
#include "can_stm32f7xx.h"
#else
#include "gpio_sc2_board.h"
#endif
#include "../AUDIO/sai.h"
#include <memory>
#include "os_timers.h"

/*!
 \brief The enumeration that stores key types of the key modes

 \enum KeyState
*/
enum {
    Fixed = 1,
    NotFixed = 2
};



class alignas (4) CallControl;

/*!
 \brief The base State class declares methods that all Concrete State should
    implement and also provides a backreference to the Context object, associated
    with the State. This backreference can be used by States to transition the
    Context to another State.

 \class State callcontrol.h "callcontrol.h"
*/
class alignas (4) State
{
    /**
     * \var Context
     */
protected:
    CallControl *context_;

public:
    /*!
     \brief Virtual destructor.

     \fn ~State
    */
    virtual ~State()
    {
    }

    /*!
     \brief The method sets current context.

     \fn set_context
     \param context
    */
    void set_context(CallControl *context)
    {
        this->context_ = context;
    }

    /*!
     \brief The virtual method to handle a button press

     \fn handleButton
    */
    virtual void handleButton() = 0;
    /*!
     \brief The virtual method to handle a received Json message

     \fn handleJsonMessage
    */
    virtual void handleJsonMessage() = 0;
    /*!
     \brief The virtual method to handle a connection loss

     \fn handleLostLink
    */
    virtual void handleLostLink() = 0;
    /*!
     \brief The virtual method is used to check a response to the request and send a repeated request

     \fn handleLostLink
    */
    virtual void handleRepeatedRequestCallBack() = 0;
    /*!
     \brief The virtual method is used to handle an unknown incoming call data when the auto answer callback is called

    \fn handleUnknownIncomingCallBack
            */
    virtual void handleUnknownIncomingCallBack() = 0;

};

/*!
 \brief The struct stores numbers of key on the keypad and values those ones

 \class Keypad call_control.h "call_control.h"
*/
struct Keypad {
    uint8_t i, /*!< The number of a key on the keypad */
            n; /*!< The value of the keypad key */
};



/*!
 \brief This struct stores data fields about assigned keys

 \class AssignedData callcontrol.h "callcontrol.h"
*/
struct AssignedData {
    uint8_t key = 0,            /*!< The variable stores the number of the assigned key */
            priority = 0,       /*!< The variable stores the priority of the assigned priority */
            conferenceKey = 0,  /*!< The variable that stores the number of the conference key */
            confPriority = 0;
    uint16_t id = 0;            /*!< The variable stores the ID of an assigned subject */
};

/*!
 \brief The Context defines the interface of interest to clients. It also maintains a
        reference to an instance of a State subclass, which represents the current
        state of the Context.

 \class State callcontrol.h "callcontrol.h"
*/
class alignas (4) CallControl
{
    /**
     * \var State A reference to the current state of the Context.
     */
private:
    State *state_;

    osMutexId mutexKeyBufferId_;    /*!< The private variable that stores a mutex ID */
    bool foundKeyFlag_;             /*!< The private logical variable that stores the founded key flag to handle pressed keys */

public:

    /*!
     \brief The enumeration that contains call types.

     \enum
    */
    enum {
        Direct = 1,
        Group = 2,
        Circular = 3,
        Conference = 4,
        Telephone = 5,
        HungUp = 7
    };

    /*!
     \brief The enumeration contains types of the subject data to control getting subject data.

     \enum SubjectData
    */
    enum SubjectData {
        Key = 1,
        Assign,
        Function,
        Priority,
        Mode,
        Link,
        Size
    };

    /*!
     \brief The enumeration contains link types to control a creating RTP process.

     \enum
    */
    enum {
        Simplex_send_type = 0,
        Simplex_recv_type,
        Duplex_type,
        Simplex_callback_type,
    };

    /*!
     \brief The enumeration that contains values of the connection modes

     \enum
    */
    enum {
        NoMode = 0,
        Simplex = 1,
        Duplex,
        DuplexAuto
    };

    /*!
     \brief The enumeration that contains values of the Asterisk and Hash keys

     \enum
    */
    enum {
        Asterisk = 29,
        Hash = 31
    };

    /*!
     \brief The enumeration that contains identifiers to control an interrupting conference call process

     \enum InterruptConf
    */
    enum class InterruptConf {
        NONE = 0,
        CALLER = 1,
        CALLED_PARTY = 2
    };

    /*!
     \brief The enumeration that contains identifiers to control a request process

     \enum Request
    */
    enum class Request {
        HANG_UP = 0x00,
        ACK = 0x11,
        ACK_ANSW = 0x22,
        BUSY = 0x33,
        CHANGE_CONF = 0x44,
        RETURN_CONF = 0x55,
        LINK = 0xFF
    };

    /*!
     \brief The enumeration that contains identifiers to control an acknowledgment handling process

     \enum Control
    */
    enum class Control {
        NONE = 0,
        HANG_UP = 1,
        EXCH_CALL_TYPE = 2,
        ANSWER = 3,
        BUSY = 4,
        READY = 5,
        CONF_HANG_UP = 6,
        CHANGE_CONF = 7,
        RETURN_CONF = 8
    };

    /*!
     \brief Base constructor.

     \fn CallControl
     \param state pointer to the concrete state
    */
    CallControl(State *state);

    /*!
     \brief Base destructor. A default destructor.

     \fn ~CallControl
    */
    ~CallControl();

    /*!
     \brief The method allows changing the State object at runtime.

     \fn TransitionTo
     */
    void TransitionTo(State *state);

    /**
     * The Context delegates part of its behavior to the current State object.
     */

#ifndef SC2BOARD
    /*!
     \brief The method handles a key press.

     \fn button
     \param pack data of the pressed buttons from the CAN bus.
     */
    void button(uPackageRx pack);
#else
    /*!
     \brief The method handles a key press.

     \fn button
     \param pack data of the pressed buttons.
     */
    void button(PackageRx pack);
#endif
    /*!
     \brief The method handles received Json message.

     \fn jsonMessage
     \param doc reference to the Json document
    */
    void jsonMessage(JsonDocument &doc);
    /*!
     \brief The method handles a not fixed key press.

     \fn handleNotFixedButton
    */
    void handleNotFixedButton();

    /*!
     \brief This method is called when the RTP link is lost

     \fn hungUp
    */
    void hungUp();

    /*!
     \brief This method is used to check a response to the request and handle that

     \fn checkRequest
    */
    void checkRequest();

    /*!
     \brief This method is used to call the handleUnknownIncomingCallBack() method of the concrete state

    \fn checkRequest
            */
    void answerUnknownCall();

    /*!
     \brief The function detects the presence of a subject.

     \fn detectSubject
     \param subject is subject ID
     \return bool is state of the detection
    */
    bool detectSubject(uint16_t subject);
    /*!
     \brief The function detects the type of a call.

     \fn detectSubjCallType
     \param func is a type of the call
     \return bool is state of the detection
    */
    bool detectSubjCallType(uint8_t func);
    /*!
     \brief The function allows to get data about a subject.

     \fn getSubjectData
     \param control is a type of the data
     \param i is iterator
     \return uint16_t is the data
    */
    uint16_t getSubjectData(CallControl::SubjectData control, uint8_t i = 0);

    /*!
     \brief The function sets up a type of the call

     \fn setCallType
    */
    void setCallType();
#ifndef SC2BOARD
    /*!
     \brief The function handles button presses

     \fn handleClick
     \param pack is data of the pressed buttons from the CAN bus.
     \return bool is state of the detection
    */
    bool handleClick(uPackageRx pack);
#else
    /*!
     \brief The function handles button presses

     \fn handleClick
     \param pack is data of the pressed buttons.
     \return bool is state of the detection
    */
    bool handleClick(PackageRx pack);
#endif
    /*!
     \brief The function sends Json message by UDP

     \fn sendJson
     \param doc is reference to the Json document
     \param capacity is size of the Json data
    */
    void sendJson(JsonDocument &doc, const size_t capacity);

    /*!
     \brief The function sends Json data about itself by UDP

     \fn sendInfoAboutStation
     \param doc is reference to the Json document
     \return bool is state of the operation
    */
    bool sendInfoAboutStation(JsonDocument &doc);

    /*!
     \brief The function creates RTP link.

     \fn removeRtp
     \return ErrorCode is code of an error
    */
    void createRtp(uint32_t port, uint8_t rtpType);
    /*!
     \brief The function removes RTP link.

     \fn removeRtp
     \return ErrorCode is code of an error
    */
    ErrorCode removeRtp();

    /*!
     \brief The function pushes data to the dynamic storage

     \fn pushDynamicStorage
     \param callType is a type of the call
     \param v is reference to the dynamic storage
     \param subject is subject ID
     \return bool is state of the operation
    */
    bool pushDynamicStorage(uint8_t callType, std::vector <uint16_t>& v, uint16_t subject);
    /*!
     \brief The function pops data from the dynamic storage

     \fn popDynamicStorage
     \param v is reference to the dynamic storage
     \param subject is subject ID
     \return bool is state of the operation
    */
    bool popDynamicStorage(std::vector <uint16_t>& v, uint16_t subject);

    /*!
     \brief The function pops data from the dynamic storage

     \fn popDynamicStorage
     \param v is reference to the dynamic storage
     \param subject is subject ID
     \return bool is state of the operation
    */
    bool seekDynamicStorage(std::vector <uint16_t>& v, uint16_t subject);

    /*!
     \brief The function allows to get a button assigned to the object

     \fn getKey
     \param subject is subject ID
     \return uint8_t is the number of a button
    */
    uint8_t getKey(const uint16_t subject);

    /*!
     \brief The function creates and sends Json document by UDP

     \fn createJsonDoc
     \param linkStatus is a status of the link
    */
    void sendMessage(const Request linkData);

    /*!
     \brief The function creates and sends Json document by UDP

     \fn sendMessage
     \param arr[] is container that stores subject IDs
     \param size is size of the container
     \param linkData is used to send a link control request
    */
    void sendMessage(const uint16_t arr[], const uint16_t size, const Request linkData);

    /*!
     \brief The function creates and sends Json document by UDP

     \fn sendMessage
     \param distId is an ID of the remote device
    \param func is used to send call type to the method
    \param linkData is used to send a link control request
    */
    void sendMessage(const uint16_t distId, const uint8_t func, const Request linkData);

    /*!
     \brief The function allows to change the link status and/or the busy and retransmits Json document by UDP

     \fn retransmitMessage
     \param arr is pointer to the container that stores Json document
     \param size is the size of the pointer
     \param linkData is used to send a link control request
     \return bool is the state of the operation
    */
    bool retransmitMessage(char* arr, const size_t size, const Request linkData);
    /*!
     \brief The function allows to change the link status and/or the busy and retransmits Json document by UDP

     \fn retransmitMessage
     \param arr is pointer to the container that stores Json document
     \param size is the size of the pointer
     \param distId is the ID of a remote subject
    \param linkData is used to send a link control request
     \return bool is the state of the operation
    */
    bool retransmitMessage(char* arr, const size_t size, const uint16_t distId, const Request linkData);
    /*!
     \brief The function allows to change the link status and/or the busy and retransmits Json document by UDP

    \fn retransmitMessage
     \param arr is pointer to the container that stores Json document
     \param size is the size of the pointer
     \param distPriority is used to send a priority of the called subject
    \param linkData is used to send a link control request
     \return bool is the state of the operation
        */
//    bool retransmitMessage(char* arr, const size_t size, const Request linkData);

    /*!
     \brief This function is used to copy received data to another buff

     \fn copyRecvBuff
     \param copyBuff is used to send the pointer to the intermediate buffer to the function
     \param buff is used to send the pointer to the received data buffer
    */
    void copyRecvBuff(char* &copyBuff, const char* buff);
    /*!
     \brief The function resets data about pressed keys and priorities

     \fn resetData
    */
    void resetData();

    /*!
     \brief The function allows to configure LEDs on the keyboard

     \fn switchLed
     \param ledNumber is a number of the key
     \param ledOn
     \param timeOn
     \param timeOff
     \param repeatNum is used to send blink repeat number to the function
    */
    void sendRequest(uint8_t callType, Request reqType,  uint16_t timeout);
    /*!
     \brief This method is used to exchange requests between subjects

     \fn sendRequest
     \param reqType is used to send a type of the request to the function
    */
    void sendRequest(Request reqType);

    /*!
     \brief This method is used to return to the conference call

     \fn switchToConf
     \return bool is state of the returning
    */
    bool switchToConf();

    OSTimer osTimer;             /*!< The pointer to the OSTimer object */
    Keys subjectKey;            /*!< The object of the Keys type is used to store subject data that assigned to the key  */


    static constexpr uint32_t START_AREA_CURRENT_FIRMWARE {0x08040000}; /*!< The constant that stores data to compute the firmware address  */

    ErrorCode rtpStatus = FAIL_RTP;     /*!< The variable stores the error code of the RTP link  */

    std::vector <uint8_t> telephoneDynamicStorage;  /*!< The container that is used to store telephone calls dynamic data */
    std::vector <uint16_t> dynamicStorage;  /*!< The container that is used to store some dynamic data */
    std::vector <uint16_t> busyDynamicStorage;  /*!< The container that is used to store some dynamic data */
    std::vector <uint16_t>::iterator dynamicStorageIter;

    constexpr static uint16_t inputBufferSize = 10; /**< The variable stores size of the inputBuffer */
    CircularBuffer <uint16_t, inputBufferSize> inputBuff; /*!< The container that is used to store input data */

    MessageData messageData; /*!< Object of the MessageData class */
    MessageDataBuff messageDataBuff, /*!< Object of the MessageData class which is used for intermediate data storage */
                    calledPartyConfDataBuff; /*!< Object of the MessageData class which is used for intermediate data storage of the called party in the conference */

//    constexpr static uint16_t calledPartyBufferSize = 3; /**< The variable stores size of the inputBuffer */
//    CircularBuffer <MessageDataBuff, calledPartyBufferSize> calledPartyConfDataBuff;

    AssignedData assignedData; /*!< Object of the AssignedData class */
    MissedCall missedCall; /*!< Object of the MissedCall class */
    Microphone microphone; /*!< Object of the Microphone class */


    bool isIncomingCall = false,        /*!< The flag of the incoming call presence */
         isAnsweredCall = false;


    constexpr static uint8_t numKeypadKeys = 10;    /*!< The const that stores the keypad number keys */
    alignas (4) std::array <uint8_t, 10> keypadArray = {        /*!< The array that stores values of the keypad */
        30,
        17,18,19,
        21,22,23,
        25,26,27,
    };


    std::array <Keypad, 10> keypadStructArray; /*!< The array that stores Keypad struct elements */

    InterruptConf isInterruptConf = InterruptConf::NONE; /*!< The variable that stores values to control the interrupting conference call process */



    uint8_t requestCount = 0; /*!< The variable is the request counter */
    bool isAnswerToRequest = false;
    uint8_t checkGroupIndex = 0; /*!< The variable is used to index subscribers in a group */

    Control control = Control::READY; /*!< The object that stores identifier to control an acknowledgment handling process */

};

/*!
 \brief This method is used to control LED states

 \fn switchLed
 \param ledNumber is used to send LED number to the function
 \param ledOn is used to send a LED light state to the function
 \param timeOn is used to send a LED light time to the function
 \param timeOff is used to send a LED off time to the function
 \param repeatNum is used to send blink repeat number to the function
*/
void switchLed(uint8_t ledNumber,
               bool ledOn = true,
               uint16_t timeOn = 0,
               uint16_t timeOff = 0,
               uint8_t repeatNum = 0);




#endif // CALLCONTROL_H
