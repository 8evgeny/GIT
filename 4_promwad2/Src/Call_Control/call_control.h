/*!
 \file call_control.h

*/

#ifndef CALL_CONTROL_H
#define CALL_CONTROL_H

#include "json.h"


#include "can_stm32f7xx.h"
#include "../AUDIO/sai.h"
#include "udp_multicast.h"

#ifdef __cplusplus
extern "C" {
#endif
extern osMutexId mutexCreateSend_id;
#ifdef __cplusplus
}
#endif

/*!
 \brief Enumeration of FSM states

 \enum State
*/
enum State {
    STATE_RECV_UDP,
    STATE_USER_WAITING,
    STATE_CREATE_RTP,
    STATE_ANSWER_WAITING,
    STATE_BUSY
};

//enum portType {
//    UDP_PORT = 1,
//    RS232_PORT
//};
/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class Call_Control call_control.h "call_control.h"
*/
class Call_Control
{
private:
    static Call_Control * pInstance; /**< The static pointer to the Call_Control class */

    /*!
     \brief Base constructor. A default constructor.

     \fn Call_Control
    */
    Call_Control();

    /*!
     \brief Copy constructor.

     \fn Call_Control
     \param
    */
    Call_Control(const Call_Control&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return Call_Control &operator
    */
    Call_Control& operator=(Call_Control&);


    bool startConfFlag_ = false; /*!< Conference start flag */
    uint32_t jsonDocCapacity_ = 0; /*!< The variable that stores JSON document capacity */
    State state_ = STATE_RECV_UDP; /*!< The variable that stores FSM state */
    constexpr static uint8_t numKeypadKeys = 10; /*!< The const that stores the keypad number keys */
    std::array <uint8_t, 10> keypadArray_ = { /*!< The array that stores values of the keypad */
        30,
        17,18,19,
        21,22,23,
        25,26,27,
    };

    /*!
     \brief The struct stores numbers of key on the keypad and values those ones

     \class Keypad call_control.h "call_control.h"
    */
    struct Keypad {
        uint8_t i, /*!< The number of key on the keypad */
                n; /*!< The value of the keypad key */
    };
    std::array <Keypad, 10> keypadStructArray_; /*!< The array that stores Keypad struct elements */
    uint8_t func_ = 0,
            mode_ = 0;
    bool foundFlag = false;



public:

    /*!
     \brief The function prepares and sends JSON document through multicast UDP

     \fn sendJson
     \param doc Json document
     \param capacity Json document capacity
    */
    void sendJson(JsonDocument &doc, const size_t capacity/*, const portType port*/);


    /*!
     \brief The enumeration that stores switch speaker values

     \enum Switch
    */
    enum Switch {
        OFF = 0x88,
        ON = 0x00
    };

    /*!
     \brief The enumeration that stores calls type values

     \enum CallType
    */
    enum CallType {
        NoType = 0,
        Direct = 1,
        Group,
        Circular,
        Conference,
        Telephone,
        HungUp = 7
    };

    /*!
     \brief The enumeration that stores call priorities

     \enum CallType
    */
    enum CallPriorities {
        Group_priority = 4,
        Circular_priority = 4,
        Telephone_priority = 4,
        Direct_priority = 5,
        Conference_priority = 6,
    };

    /*!
     \brief The enumeration that stores key state values

     \enum KeyState
    */
    enum KeyState {
        None = 0,
        Fixed = 1,
        NoFixed = 2
    };

    /*!
     \brief The enumeration that stores values of the Asterisk and Hash keys

     \enum
    */
    enum {
        Asterisk = 29,
        Hash = 31
    };

    /*!
     \brief The enumeration that stores values to control hangUp

     \enum
    */
    enum HangUpControl {
        Dist = 0,
        User = 1
    };

    /*!
     \brief The enumeration that stores values needed to get data from KeysRingBuffer

     \enum GetKeysData
    */
    enum GetKeysData {
        Key = 1,
        Assign,
        Function,
        Priority,
        Mode,
        Link,
        Size
    };

    /*!
     \brief The enumeration that stores values of the connection modes

     \enum LinkField
    */
    enum LinkField {
        NoMode = 0,
        Simplex = 1,
        Duplex,
        DuplexAuto
    };

    /*!
     \brief The enumeration that stores values needed to get data from KeysRingBuffer

     \enum GetKeysData
    */
    enum ControlMissedCall {
        Save = 1,
        Erase
    };

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return Call_Control. The public static operation can be accessed easily by using the class name and operation name.
    */
    static Call_Control * getInstance();

    /*!
     \brief The function that controls FSM during incoming calls

     \fn fsmRecvControl
     \param doc JSON document that has been received through UDP
    */
    void fsmRecvControl(JsonDocument& doc);
    /*!
     \brief The function that controls FSM during outgoing calls

     \fn fsmSendControl
     \param key Struct that stores key parameters
    */
    void fsmSendControl(Keys& key);
    /*!
     \brief The function clears the dynamicStoreKeysBuff vector

     \fn clearDynamicStore
    */
    void clearDynamicStore();
    /*!
     \brief The function that deletes a key from the dynamicStoreKeysBuff vector

     \fn deleteDynamicStoreKey
    */
    void deleteDynamicStoreKey();
    /*!
     \brief The function sends a JSON document that stores the null link status.
     The function is used to disconnection during call termination

     \fn sendUnlinkUdp
    */
    void sendUnlinkUdp();
    /*!
     \brief The function processes type of a incoming call and creates the user waiting thread

     \fn waitingThread_Init
    */
    inline void waitingThread_Init();
    /*!
     \brief The function that controls FSM when user presses a key

     \fn fsmKeysControl
     \param key Number of a pressed key
    */
    void fsmKeysControl(uint8_t key, uint8_t func, uint8_t mode);
    /*!
     \brief The function implements actions when user hangs up:
     - sets FSM state;
     - remove RTP connection;
     - sends a disconnect JSON message.

     \fn hangUp
    */
    void hangUp(HangUpControl control);
    /*!
     \brief The function implements push to talk mode in the call modes with no key fixation

     \fn pushToTalk
    */
    void pushToTalk();
    /*!
     \brief The function adds a subscriber to the dynamicStoreKeysBuff
            in the Circular and Conference call modes

     \fn addSubscriber
     \param key Number of a pressed key
    */
    void addSubscriber(uint8_t key);
    /*!
     \brief The function switches the microphone

     \fn switchMicrophone
     \param control Value of the Switch enumeration ON or OFF
    */
    void switchMicrophone(Switch control);
    /*!
     \brief The function stops a chosen OS timer

     \fn stopTimer
     \param id chosen OS timer ID
     \param status Variable to store timer status
     \return osStatus
    */
    inline osStatus stopTimer(osTimerId id, osStatus& status);
    /*!
     \brief The function starts a chosen OS timer

     \fn startTimer
     \param id chosen OS timer ID
     \param status Variable to store timer status
     \param timerDelay value of the chosen OS timer
     \return osStatus
    */
    inline osStatus startTimer(osTimerId id, osStatus& status, uint16_t timerDelay);
    /*!
     \brief The function sets a call type

     \fn setCallType
     \param type Value CallType enumeration that sets the call type
    */
    void setCallType(uint8_t type);
    /*!
     \brief The function allows to get some parameters of the key:
      - number of key;
      - assigned ID;
      - function of key;
      - size of the KeysRingBuff

     \fn getKeysRingBuffData
     \param control Value GetKeysData enumeration that defines a getting parameter
     \param i Number of the KeysRingBuff element
     \return uint16_t Value of the getting parameter
    */
    uint16_t getKeysRingBuffData(Call_Control::GetKeysData control, uint8_t i = 0);
    /*!
     \brief The function initializes keypad to use in the telephone call mode

     \fn numberPad_Init
    */
    void numberPad_Init();
    /*!
     \brief The function initializes some variables of the Call_Control class

     \fn Call_Control_Init
    */
    void Call_Control_Init();
    /*!
     \brief The function that allow to set a FSM state

     \fn setFsmState
     \param state Value of the State enumeration that allows to set the FSM state
    */
    void setFsmState (State state);
    /*!
     \brief The function sends the JSON that contains the information about this station

     \fn sendInfoAboutStation
     \param doc JSON document
     \return bool field detection flag
    */
    bool sendInfoAboutStation(JsonDocument &doc);

    void setBufInput(uPackageRx pack);


    inline void createSend(Keys* arg);
    void controlMissedCall();
    void setMode(uint8_t func);
    bool dropMissedCall(uint8_t key);

    const static size_t outputJson_size = 2000; /*!< The size of the outputJson array  */
    char outputJson[outputJson_size];// = nullptr; /*!< The array that stores a JSON document for sanding */
    char outputJsonDes[outputJson_size];


    osMutexId mutexState_id,        /*!< The variable that stores ID of the mutexState */
              mutexIncomingCall_id, /*!< The variable that stores ID of the mutexIncomingCall */
              mutexTimerState_id,   /*!< The variable that stores ID of the mutexTimerState */
              mutexPadKey_id,       /*!< The variable that stores ID of the mutexPadKey */
              mutexCallType_id,     /*!< The variable that stores ID of the mutexCallType */
              mutexDymanicStore_id, /*!< The variable that stores ID of the mutexDymanicStore */
              mutexKeyBuffer_id,    /*!< The variable that stores ID of the mutexKeyBuffer */
              mutex_outputJson_id,
              mutexMissedCall_id;


    uint8_t assignedKey = 0, /*!< TODO: describe */
            pressedKey = 0, /*!< TODO: describe */
            noFixedKey = 0; /*!< TODO: describe */
    Keys foundKey; /*!< TODO: describe */
    osThreadId waitAnswerThreadId, /*!< waitAnswerThread ID */
               outgoingCallThreadId;

    osMessageQId message_call_id;           // Declare an ID for the message queue
//    osPoolId keys_pool_id;

    uint8_t keyMode = Fixed; /*!< The variable that stores key mode fixed or no fixed */
    int8_t callType = NoType; /*!< The variable that stores a value of the CallType enumeration  */

    bool incomingCall = false, /*!< The variable that is incoming call flag */
         outgoingTelCall = false;

    std::vector<uint8_t> missedCall; /*!< The buffer for storing missed calls  */
    std::vector<uint8_t>::iterator missedCallIter; /*!< The iterator of the storing missed calls buffer  */

    uint32_t* sramReadBuff = nullptr; /*!< The buffer pointer that points to the JSON document that has been downloaded from the EEPROM to the SRAM */

    uint8_t noFixedCount = 0; /*!< The counter for the no fixed key mode */
    uint8_t fixedCount = 0;
    uint8_t inputKeyMode = None;

    constexpr static uint8_t timer1_delay = 120, /*!< The constant that stores a value of the Timer 1 */
                             timer2_delay = 50; /*!< The constant that stores a value of the Timer 2 */

    osTimerId timer1_id, /*!< The variable that stores ID of the Timer 1 */
              timer2_id, /*!< The variable that stores ID of the Timer 2 */
              timer3_id, /*!< The variable that stores ID of the Timer 3 */
              timer4_id; /*!< The variable that stores ID of the Timer 3 */
    osStatus timer1_status = osErrorOS, /*!< The variable that stores the state of the Timer 1 */
             timer2_status = osErrorOS, /*!< The variable that stores the state of the Timer 2 */
             timer3_status = osErrorOS, /*!< The variable that stores the state of the Timer 3 */
             timer4_status = osErrorOS; /*!< The variable that stores the state of the Timer 3 */


    std::vector<uint16_t> dynamicStoreKeysBuff; /*!< The buffer for dynamic storing keys */
    std::vector<uint16_t>::iterator dynamicStoreIter; /*!< The iterator of the dynamic storing keys buffer */

    bool speakerOn = false, /*!< The variable that is state speaker flag */
         confHangUp = false; /*!< The variable that is the flag for removing subscribers while conference and circular call modes */

    char sendBuff[1200] = {0}; /*!< The buffer for sending through UDP multicast */

//    JsonDocument *sendDoc = nullptr; /*!< The pointer for sent JSON doc  */

    uint8_t numberPudKey = 0; /*!< The variable that stores pressed key of the keypad in the telephone call mode */

    bool emptyDynamicStoreFlag = false; /*!< The variable that stores dynamicStoreKeysBuff empty flag */

    uint8_t setBufInputCount = 0;
    uint8_t setBufInputButton = 0;




};

/*!
 \brief The OS thread function that sends JSON document
        and creates RTP connection when user answers a call

 \fn waitAnswerThread
 \param arg no used
*/
void waitAnswerThread(const void *arg);
/*!
 \brief The callback function for the timer

 \fn Timer1_Callback
 \param arg no used
*/
void Timer1_Callback(void const *arg);
/*!
 \brief The callback function for the timer

 \fn Timer2_Callback
 \param arg no used
*/
void Timer2_Callback(void const *arg);
/*!
 \brief The callback function for the timer

 \fn Timer3_Callback
 \param arg no used
*/
void Timer3_Callback(void const *arg);
/*!
 \brief The callback function for the timer

 \fn Timer4_Callback
 \param arg no used
*/
void Timer4_Callback  (void const *arg);
[[ noreturn ]]
/*!
 \brief The OS thread function sends JSON document through UDP

 \fn outgoingCallThread
 \param arg not used
*/
void outgoingCallThread(const void *arg);

#endif //CALL_CONTROL_H
