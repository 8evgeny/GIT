/*!
 \file call_control.cpp

*/

#include "call_control.h"
#include "cmsis_os.h"
#include "math.h"
#include "ethernetif.h"

#include "rs232.h"

#define START_AREA_CURRENT_FIRMWARE 0x08040000

osMutexDef (mutexState);
osMutexDef (mutexTimerState);
osMutexDef (mutexIncomingCall);
osMutexDef (mutexPadKey);
osMutexDef (mutexCallType);
osMutexDef (mutexDymanicStore);
osMutexDef (mutexKeyBuffer);
osMutexDef (mutex_outputJson);
osMutexDef (mutexCreateSend);
osMutexDef (mutexMissedCall);
osThreadDef(waitAnswerThread, waitAnswerThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE * 5);
osThreadDef(outgoingCallThread, outgoingCallThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 150);
osTimerDef (Timer4, Timer4_Callback);                      // define timers

//osPoolDef (keys_pool, 1, Keys);
osMessageQDef(message_call, 1, uint32_t); // Declare a message queue



static uint16_t ownId,          /*!< The variable that stores a station own ID */
       prevOwnId;               /*!< The variable that stores a previous station own ID */
static uint8_t  distId,         /*!< The variable that stores a station remote ID */
       linkStatus,              /*!< The variable that stores a link status */
       priority,                /*!< The variable that stores a call priority */
       prevPriority,            /*!< The variable that stores a previous call priority */
       incomingCallType,        /*!< The variable that stores a incoming call type */
       telephoneCounter = 0,    /*!< The counter for the telephone mode */
       linkMode;

static char sendInterruptBuff[200] = {0}; /*!< The buffer for the interrupted call */

static uint8_t hangUpKey;

#ifdef __cplusplus
extern "C" {
#endif
osMutexId mutexCreateSend_id;
#ifdef __cplusplus
}
#endif

Call_Control::Call_Control()
{
//    Call_Control_Init();
}

Call_Control *Call_Control::getInstance()
{
    if(!pInstance) pInstance = new Call_Control();
    return pInstance;
}

Call_Control *Call_Control::pInstance = nullptr;


void Call_Control::Call_Control_Init()
{
    missedCall.reserve(32);
    dynamicStoreKeysBuff.reserve(10);
    missedCallIter =  missedCall.begin();
    dynamicStoreIter = dynamicStoreKeysBuff.begin();

    numberPad_Init();


    message_call_id = osMessageCreate(osMessageQ(message_call), nullptr);


    mutexMissedCall_id = osMutexCreate(osMutex(mutexMissedCall));
    if (mutexMissedCall_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexCreateSend_id = osMutexCreate(osMutex(mutexCreateSend));
    if (mutexCreateSend_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutex_outputJson_id = osMutexCreate(osMutex(mutex_outputJson));
    if (mutex_outputJson_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexKeyBuffer_id = osMutexCreate(osMutex(mutexKeyBuffer));
    if (mutexKeyBuffer_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexDymanicStore_id = osMutexCreate(osMutex(mutexDymanicStore));
    if (mutexDymanicStore_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexCallType_id = osMutexCreate(osMutex(mutexCallType));
    if (mutexCallType_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexIncomingCall_id = osMutexCreate(osMutex(mutexIncomingCall));
    if (mutexIncomingCall_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexPadKey_id = osMutexCreate(osMutex(mutexPadKey));
    if (mutexPadKey_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexState_id = osMutexCreate  (osMutex (mutexState));
    if (mutexState_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    mutexTimerState_id = osMutexCreate(osMutex( mutexTimerState));
    if ( mutexTimerState_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    uint16_t size = getKeysRingBuffData(Size);
    for (int i = 0; i < size; ++i) {
        if (getKeysRingBuffData(Function, i) == 7) {/*7 is HungUp field of the CallType enum*/
            hangUpKey = getKeysRingBuffData(Key, i);
            break;
        }
    }

    if ((outgoingCallThreadId = osThreadCreate(osThread(outgoingCallThread), nullptr)) == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((timer4_id = osTimerCreate (osTimer(Timer4), osTimerPeriodic, &inputKeyMode)) == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

void Call_Control::numberPad_Init()
{
    for (uint8_t i = 0; i < sizeof (keypadArray_); ++i) {
        keypadStructArray_[i].n = keypadArray_[i];
        keypadStructArray_[i].i = i;
    }
}

inline void Call_Control::waitingThread_Init()
{
    if (state_ != STATE_CREATE_RTP && incomingCallType != Conference && incomingCallType != Telephone)
        switchMicrophone(ON);

    bool noFoundKeyFlag = false;
    uint8_t numberOfKey = 0;

    uint16_t size =  getKeysRingBuffData(Size);
    for(uint8_t i = 0; i <size; ++i ) {

        numberOfKey = i;

        if (incomingCallType == Conference) {
            if (getKeysRingBuffData(Function, i) == Conference) {
                noFoundKeyFlag = true;
//                keyMode =
                if (getKeysRingBuffData(Mode, i) == Fixed)
                    switchMicrophone(ON);
                break;
            }
        } else if (incomingCallType == Direct) {
            if (getKeysRingBuffData(Assign, i) == ownId && linkMode == Duplex) {
                noFoundKeyFlag = true;
                break;
            } else if (getKeysRingBuffData(Assign, i) == ownId && linkMode == Simplex/* &&
                       getKeysRingBuffData(Link, i) == Simplex*/) {

                CAN::getInstance()->configLed(getKeysRingBuffData(Key, i), true, 250, 250);

                assignedKey = Json::getInstance()->thisStation.keysRingBuffer[i].key;
                break;
            }

        } else if (incomingCallType == Telephone) {
            noFoundKeyFlag = true;
            //            setCallType(Telephone);

            CAN::getInstance()->configLed(Asterisk, true, 250, 250);

            assignedKey = Asterisk;
            break;
        } else {
            if (getKeysRingBuffData(Assign, i) == ownId) {

                CAN::getInstance()->configLed(getKeysRingBuffData(Key, i), true, 250, 250);

                assignedKey = Json::getInstance()->thisStation.keysRingBuffer[i].key;
                break;
            }

        }

    }
    if (noFoundKeyFlag) {
        setFsmState (STATE_USER_WAITING);
        if (incomingCallType != Telephone) {

            CAN::getInstance()->configLed(getKeysRingBuffData(Key, numberOfKey), true, 250, 250);

            assignedKey = getKeysRingBuffData(Key, numberOfKey);

            dropMissedCall(assignedKey);

        }
//            startRingTone();

//        startRingTone();
        if (( Call_Control::getInstance()->waitAnswerThreadId = osThreadCreate(osThread(waitAnswerThread), (void *)sendBuff)) == nullptr) {
            while(1)
                Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
//        break;
    }

    if (!noFoundKeyFlag && incomingCallType != Conference) {
//        if (incomingCallType == Direct)
//            sendUdpMulticast(Call_Control::getInstance()->outputJson, Call_Control::getInstance()->outputJson_size);
        setFsmState (STATE_CREATE_RTP);
//        osDelay(10);
        rtpCreate(ownId, 1);
    }
}

void Call_Control:: fsmRecvControl(JsonDocument &doc)
{
    ownId = doc["ownId"];
    linkStatus = doc["linkStatus"];
    priority = doc["priority"];
    linkMode = doc["linkMode"];
    incomingCallType = doc["callType"];

    size_t distIdArrSize = doc["distId"].size();
    if (distIdArrSize > 1) {
        for (uint32_t i = 0; i < distIdArrSize; ++i) {
            if (Json::getInstance()->thisStation.id == doc["distId"][i]) {
                distId = doc["distId"][i];
                break;
            } else distId = 0;
        }
    } else {
        distId = doc["distId"][0];
    }

    switch (state_) {
    case STATE_RECV_UDP:
        if (Json::getInstance()->thisStation.id == distId && linkStatus == 0xFF) {

            osMutexWait(Call_Control::getInstance()->mutex_outputJson_id, osWaitForever);
            for (uint16_t i = 0; i < sizeof (sendBuff); ++i)
                outputJson[i] = sendBuff[i];
            osMutexRelease(Call_Control::getInstance()->mutex_outputJson_id);

            prevPriority = priority;
            prevOwnId = ownId;

            waitingThread_Init();

//            startRingTone();
        } else if (Json::getInstance()->thisStation.id == distId && linkStatus == 0x00 && incomingCallType == Telephone) {
            incomingCallType = 0;
        }
        break;
    case STATE_USER_WAITING:
        if (Json::getInstance()->thisStation.id == distId && linkStatus == 0x00 && prevOwnId == ownId) {

//            stopRingTone();

            setFsmState (STATE_RECV_UDP);
            switchMicrophone(OFF);


            if (incomingCallType != Telephone) {
//#ifndef SC2BOARD
                CAN::getInstance()->configLed( assignedKey, true, 100, 900);

                osMutexWait( mutexMissedCall_id, osWaitForever);
                Call_Control::getInstance()->missedCall.push_back(assignedKey);
                osMutexRelease( mutexMissedCall_id);
            } else {
//                CAN::getInstance()->configLed( assignedKey, false);
                hangUp(Dist);

            }
            osThreadTerminate( waitAnswerThreadId);

        } else if (Json::getInstance()->thisStation.id == distId && linkStatus == 0xFF) {
            doc["linkStatus"] = 0x00;
            serializeJson(doc, sendBuff);
            sendUdpMulticast(sendBuff, sizeof (sendBuff));
        }
        break;
    case STATE_ANSWER_WAITING:
        if (Json::getInstance()->thisStation.id == ownId && linkStatus == 0xFF) {
            setFsmState (STATE_CREATE_RTP);
            osDelay(1);

            if (incomingCallType == Telephone)

                CAN::getInstance()->configLed(Asterisk, true);

            assignedKey = pressedKey;
//            stopRingBackTone();
            rtpCreate(ownId, 2);

        } else if (Json::getInstance()->thisStation.id == ownId && linkStatus == 0x00) {
            setFsmState (STATE_BUSY);
//            stopRingBackTone();
//            startRingBackTone(BusyTone);

            CAN::getInstance()->configLed(pressedKey, true, 900, 100);

        }
        break;
    case STATE_CREATE_RTP:
        if (Json::getInstance()->thisStation.id == distId && linkStatus == 0xFF && priority < prevPriority) {

            rtpRemove();
//            osDelay(10);

            CAN::getInstance()->configLed( assignedKey, false);

            osDelay(50);
            doc["ownId"] = prevOwnId;
            doc["linkStatus"] = 0x00;
            serializeJson(doc, sendInterruptBuff);
            sendUdpMulticast(sendInterruptBuff, sizeof (sendInterruptBuff));
            prevOwnId = ownId;
            waitingThread_Init();

        } else if (Json::getInstance()->thisStation.id == distId && linkStatus == 0xFF && priority >= prevPriority) {

            doc["linkStatus"] = 0x00;
            serializeJson(doc, sendBuff);
            sendUdpMulticast(sendBuff, sizeof (sendBuff));

        } else if (Json::getInstance()->thisStation.id == ownId && linkStatus == 0x00) {

            if (incomingCallType == Circular || incomingCallType == Conference) {
                deleteDynamicStoreKey();
            } else {
                hangUp(Dist);
            }

        } else if (Json::getInstance()->thisStation.id == distId && linkStatus == 0x00 && prevOwnId == ownId) {

            CAN::getInstance()->configLed(assignedKey, false);

//            rtpRemove();
            hangUp(Dist);

//            setFsmState (STATE_RECV_UDP);
//            switchMicrophone(OFF);

        } else if (Json::getInstance()->thisStation.id == ownId && linkStatus == 0xFF && incomingCallType == Conference) {

            osMutexWait(mutexDymanicStore_id, osWaitForever);
            for (auto &n : dynamicStoreKeysBuff) {
                if (n == distId) {
                    size_t size = getKeysRingBuffData(Size);
                    for (uint16_t i = 0; i < size; ++i) {
                        if (getKeysRingBuffData(Assign, i) == distId) {

                            CAN::getInstance()->configLed(getKeysRingBuffData(Key, i), true, 300, 300, 2);

                            break;
                        }
                    }
                    break;
                }
            }
            osMutexRelease(mutexDymanicStore_id);
        }

        break;

    default:
        break;
    }
}

void Call_Control::sendJson(JsonDocument &doc, const size_t capacity/*, const portType port*/)
{
    osMutexWait(Call_Control::getInstance()->mutex_outputJson_id, osWaitForever);
    std::fill(outputJson, outputJson + outputJson_size, 0);
    serializeJson(doc, outputJson, capacity);
//    switch (port) {
//    case UDP_PORT:
    sendUdpMulticast(outputJson, strlen(outputJson));
//        break;
//    case RS232_PORT: {
//        uint16_t size = strlen(outputJson);
//        RS232::getInstance().write((uint8_t*)outputJson, size);
//    }
//    break;
//    }

    osMutexRelease(Call_Control::getInstance()->mutex_outputJson_id);
}

void Call_Control::fsmSendControl(Keys &key)
{

//    if (callType == Group) {
//        jsonDocCapacity_ = JSON_OBJECT_SIZE(5) + JSON_ARRAY_SIZE(100);
//    } else {
//        jsonDocCapacity_ = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(1);
//    }

    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
//    StaticJsonDocument<capacity>doc;
    DynamicJsonDocument doc (capacity);

    switch (callType) {
    case Group: {

        doc["ownId"] = Json::getInstance()->thisStation.id;
        uint16_t size = Json::getInstance()->thisStation.groupsRingBuffer[key.assign].stSize;
        for (uint16_t i = 0; i < size; ++i) {
            doc["distId"].add(Json::getInstance()->thisStation.groupsRingBuffer[key.assign].stantions[i]);
        }
        doc["callType"] = callType;
        doc["linkStatus"] = 0xFF;
        doc["priority"] = key.priority;
        sendJson(doc, /*jsonDocCapacity_*/capacity/*, UDP_PORT*/);
        setFsmState (STATE_CREATE_RTP);
        delete[] sramReadBuff;
    }
    break;
    case Circular: {
        doc["ownId"] = Json::getInstance()->thisStation.id;
        doc["distId"].add(key.assign);
        doc["callType"] = callType;
        if (confHangUp)
            doc["linkStatus"] = 0x00;
        else doc["linkStatus"] = 0xFF;
        doc["priority"] = key.priority;
        sendJson(doc, /*jsonDocCapacity_*/capacity/*, UDP_PORT*/);
        setFsmState (STATE_CREATE_RTP);
    }
    break;
    case Conference: {
        doc["ownId"] = Json::getInstance()->thisStation.id;
        doc["distId"].add(key.assign);
        doc["callType"] = callType;
        if (confHangUp)
            doc["linkStatus"] = 0x00;
        else doc["linkStatus"] = 0xFF;
        doc["priority"] = key.priority;
        sendJson(doc, /*jsonDocCapacity_*/capacity/*, UDP_PORT*/);
        setFsmState (STATE_CREATE_RTP);
    }
    break;
    case Direct: {
        doc["ownId"] = Json::getInstance()->thisStation.id;
        doc["distId"].add(key.assign);
        doc["callType"] = callType;
        doc["priority"] = key.priority;
        doc["linkMode"] = key.link;
        doc["linkStatus"] = 0xFF;
        sendJson(doc, /*jsonDocCapacity_*/capacity/*, UDP_PORT*/);
        if (key.link == Duplex) {
            setFsmState (STATE_ANSWER_WAITING);
//            startRingBackTone(RingTone);
        } else if (key.link == Simplex)
            setFsmState (STATE_CREATE_RTP);
    }
    break;
    case Telephone: {
        doc["ownId"] = Json::getInstance()->thisStation.id;
        osMutexWait(mutexDymanicStore_id, osWaitForever);
        uint16_t dynamicStoreSize = dynamicStoreKeysBuff.size(),
                 dist =0;

        for (uint8_t i = 0; i < dynamicStoreSize; ++i) {
            dist +=  pow(10, (dynamicStoreSize-i-1)) * dynamicStoreKeysBuff[i];
        }
        dynamicStoreKeysBuff.clear();
        osMutexRelease(mutexDymanicStore_id);
        doc["distId"].add(dist);
        doc["callType"] = callType;

        const uint16_t keysRingBuffSize = getKeysRingBuffData(Size);
        bool foundKey_flag = false;
        uint8_t foundKey_priority = 0;
        for (uint16_t i = 0; i < keysRingBuffSize; ++i)
            if (getKeysRingBuffData(Assign, i) == dist) {
                foundKey_flag = true;
                foundKey_priority = getKeysRingBuffData(Priority, i);
                break;
            } else foundKey_priority = Telephone_priority;
        doc["priority"] = foundKey_priority;
        doc["linkStatus"] = 0xFF;
        sendJson(doc, /*jsonDocCapacity_*/capacity/*, UDP_PORT*/);
//        startRingBackTone(RingTone);
        setFsmState (STATE_ANSWER_WAITING);
    }
    break;
    default:
        break;
    }
    if (state_ == STATE_CREATE_RTP) {
        osDelay(1);
        if (callType == Group || callType == Direct) {
            osDelay(10);
            rtpCreate(Json::getInstance()->thisStation.id, 0);
        } else if  (!startConfFlag_) {
            startConfFlag_ = true;
            osDelay(10);
            rtpCreate(Json::getInstance()->thisStation.id, 2);
        }
        osDelay(1);
    }
}

void Call_Control::hangUp(HangUpControl control)
{
    if (callType != NoType) {

        stopTimer(timer1_id, timer1_status);

        osMutexWait	( mutexState_id, osWaitForever);
        noFixedCount = 0;
        osMutexRelease ( mutexState_id);

//        keyMode = Fixed;
        incomingCallType = 0;
        outgoingTelCall = false;
        if (callType == Circular || callType == Conference) startConfFlag_ = false;
        if (callType == Telephone)

            CAN::getInstance()->configLed(assignedKey, false);
        else
            CAN::getInstance()->configLed(pressedKey, false);
    } else {
        incomingCallType = 0;
        CAN::getInstance()->configLed(assignedKey, false);
    }

    switchMicrophone(OFF);
    if (control == User)
        sendUnlinkUdp(); // sendTo UDP

    if (state_ == STATE_CREATE_RTP)
        rtpRemove();

    setFsmState (STATE_RECV_UDP);
    setCallType(NoType);



}

void Call_Control::clearDynamicStore()
{
    osMutexWait(mutexDymanicStore_id, osWaitForever);
    uint16_t size = getKeysRingBuffData(Size);
    for(uint8_t i = 0; i < size; ++i ) {

        for(auto &n : dynamicStoreKeysBuff) {
            if (getKeysRingBuffData(Assign, i) == n) {
                osDelay(80);

                CAN::getInstance()->configLed(getKeysRingBuffData(Key, i), false);
                break;
            }
        }

    }
    dynamicStoreKeysBuff.clear();
    osMutexRelease(mutexDymanicStore_id);
}

void Call_Control::pushToTalk()
{
    if (/*timer1_status_*/timer1_status != osOK) {
        startTimer(timer1_id, timer1_status, timer1_delay);
        speakerOn = true;
        switchMicrophone(ON);
    } else {
        osMutexWait	( mutexState_id, osWaitForever);
        noFixedCount = 0;
        osMutexRelease ( mutexState_id);

        if (!speakerOn) {
            speakerOn = true;
            switchMicrophone(ON);
        }
    }
}

void Call_Control::addSubscriber(uint8_t key)
{
    uint16_t sizeOfKeyRingBuff = getKeysRingBuffData(Size);
    for(uint8_t i = 0; i < sizeOfKeyRingBuff; ++i) {

        osMutexWait(mutexDymanicStore_id, osWaitForever);
        size_t sizeDynamicStore = dynamicStoreKeysBuff.size();
        emptyDynamicStoreFlag= dynamicStoreKeysBuff.empty();
        osMutexRelease(mutexDymanicStore_id);

        if (getKeysRingBuffData(Key, i) == key && sizeDynamicStore <= 5) {

            osMutexWait(mutexKeyBuffer_id, osWaitForever);
            foundKey = Json::getInstance()->thisStation.keysRingBuffer[i];
            osMutexRelease(mutexKeyBuffer_id);

            uint8_t key = getKeysRingBuffData(Key, i);
            uint8_t assign = getKeysRingBuffData(Assign, i);
            uint8_t n = 0;

            if (emptyDynamicStoreFlag)
                confHangUp = false;
            else {
                osMutexWait(mutexDymanicStore_id, osWaitForever);
                while(n < sizeDynamicStore) {
                    if (*(dynamicStoreIter + n) == assign) {
                        confHangUp = true;
                        break;
                    } else {
                        confHangUp = false;
                    }
                    ++n;
                }
                osMutexRelease(mutexDymanicStore_id);
            }

            osMutexWait(mutexDymanicStore_id, osWaitForever);
            if (confHangUp) {

                CAN::getInstance()->configLed(key, false);
                dynamicStoreKeysBuff.erase(dynamicStoreIter + n);
            } else {
                CAN::getInstance()->configLed(key);
                dynamicStoreKeysBuff.push_back(assign);
            }
            osMutexRelease(mutexDymanicStore_id);
//            osDelay(100);
            createSend(&foundKey);
            break;
        }
    }
}

void Call_Control::setBufInput(uPackageRx pack)
{


    if (pack.s.packetType == CAN::getInstance()->button) {

        if (setBufInputButton == pack.s.payloadData) {

            if (timer4_status != osOK) {
                setBufInputCount++;

                if (setBufInputCount > 2 /*&& foundFlag*/) {
                    Call_Control::getInstance()->fsmKeysControl(setBufInputButton, func_, mode_);
                    setBufInputCount = 0;
                    startTimer(timer4_id, timer4_status, 150);
                }


            } else {
                startTimer(timer4_id, timer4_status, 150);
            }

        } else {
            setBufInputButton = pack.s.payloadData;
            setBufInputCount = 1;
            stopTimer(timer4_id, timer4_status);
            foundFlag = false;

            for(uint8_t i = 0; i < getKeysRingBuffData(Size); ++i ) {

                if ((getKeysRingBuffData(Key, i)) == setBufInputButton) {

                    if ((func_ = getKeysRingBuffData(Function, i)) != Telephone)
                        foundFlag = true;

                    if (func_ == Group || func_ == Direct || func_ == Telephone) {
                        osMutexWait(mutexKeyBuffer_id, osWaitForever);
                        foundKey = Json::getInstance()->thisStation.keysRingBuffer[i];
                        osMutexRelease(mutexKeyBuffer_id);
                    }

                    inputKeyMode = mode_ = getKeysRingBuffData(Mode, i);
                    break;
                }
            }

//            if (setBufInputButton == Asterisk || setBufInputButton == Hash)
//                foundFlag = true;

        }

//        if (setBufInputCount > 2) {

//            if (timer4_status != osOK) {
//                Call_Control::getInstance()->fsmKeysControl(setBufInputButton, func_, mode_);
//                setBufInputCount = 0;
//                startTimer(timer4_id, timer4_status, 150);
//                if (inputKeyMode == NoFixed)
//                    switchMicrophone(ON);
    }
}

void Call_Control::fsmKeysControl(uint8_t key, uint8_t func, uint8_t mode)
{

    switch (callType) {
    case Direct:
        if (pressedKey == key) {
            hangUp(User);
            //            stopRingBackTone();
        }
        break;
    case Group:
        if (pressedKey == key)
            hangUp(User);
        break;
    case Circular: {

        osMutexWait(mutexDymanicStore_id, osWaitForever);
        emptyDynamicStoreFlag= dynamicStoreKeysBuff.empty();
        osMutexRelease(mutexDymanicStore_id);

        if (key == hangUpKey) {
            hangUp(User);
            clearDynamicStore();
        } else if (pressedKey != key)
            addSubscriber(key);
        else if (pressedKey == key && mode == NoFixed && !emptyDynamicStoreFlag)
            switchMicrophone(ON);
////            pushToTalk();
//        else if (pressedKey == key) {
//            hangUp(User);
//            clearDynamicStore();
//        }
    }
    break;
    case Conference: {
        osMutexWait(mutexDymanicStore_id, osWaitForever);
        emptyDynamicStoreFlag = dynamicStoreKeysBuff.empty();
        osMutexRelease(mutexDymanicStore_id);
        if (key == hangUpKey) {
            startConfFlag_ = false;
            hangUp(User);
            clearDynamicStore();
        } else if (pressedKey != key)
            addSubscriber(key);
        else if (pressedKey == key && mode == NoFixed && !emptyDynamicStoreFlag)
            switchMicrophone(ON);
//            pushToTalk();
//        else if (pressedKey == key) {
//            hangUp(User);
//            clearDynamicStore();
//        }
    }
    break;
    case Telephone:
        if (key == Asterisk)
            switchMicrophone(ON);
//            pushToTalk();
        else if (key == Hash) {
//            stopRingBackTone();
            hangUp(User);
            osMutexWait(mutexDymanicStore_id, osWaitForever);
            dynamicStoreKeysBuff.clear();
            osMutexRelease(mutexDymanicStore_id);
        } else if (!outgoingTelCall && incomingCallType != Telephone) {
            if (key == numberPudKey) {
                osMutexWait	( mutexState_id, osWaitForever);
                noFixedCount = 0;
                osMutexRelease ( mutexState_id);
            } else for (auto &var : keypadStructArray_) {
                    if (key == var.n) {
                        if (!numberPudKey) {

                            startTimer(timer3_id, timer3_status, 3000);
                            osMutexWait(mutexPadKey_id, osWaitForever);
                            telephoneCounter = 0;
                            numberPudKey = var.n;
                            osMutexRelease(mutexPadKey_id);

                            osMutexWait(mutexDymanicStore_id, osWaitForever);
                            dynamicStoreKeysBuff.push_back(var.i);
                            osMutexRelease(mutexDymanicStore_id);

                            startTimer(timer2_id, timer2_status, timer2_delay);
                            CAN::getInstance()->configLed(numberPudKey);
                        }
                    }
                }
        }
        break;
    case NoType:
        switch (state_) {
        case STATE_USER_WAITING:

            if (incomingCallType == Telephone && key == Hash) {
//                stopRingTone();
                hangUp(User);
            } else if (assignedKey == key) {
                if (incomingCallType == Telephone) setCallType(Telephone);
//                stopRingTone();
                osSignalSet(waitAnswerThreadId, 0x09);
                CAN::getInstance()->configLed(assignedKey);
            }
            break;
        case STATE_CREATE_RTP:
            if (incomingCallType == Telephone && key == Hash)
                hangUp(User);
            else /*if (incomingCallType == Conference && key == 32)
                hangUp();
            else*/ if (assignedKey == key && incomingCallType == Conference && mode == NoFixed) {
                    pushToTalk();
                } else if (assignedKey == key && incomingCallType == Direct && linkMode != Simplex)
                    hangUp(User);
            break;
        case STATE_RECV_UDP:
            if (key == hangUpKey) {}
            else if (key == Asterisk) {
                setCallType(Telephone);
                CAN::getInstance()->configLed(Asterisk);
                assignedKey = pressedKey = Asterisk;

            } else { /*for(uint8_t i = 0; i < getKeysRingBuffData(Size); ++i ) {*/

                if (!dropMissedCall(setBufInputButton)) {

                    CAN::getInstance()->configLed(setBufInputButton, false);
                    break;
                }

                pressedKey = key;
                if (foundFlag)
                    setMode(func);
            }
            break;
        case STATE_ANSWER_WAITING:
            if (pressedKey == key)
                hangUp(User);
//            stopRingBackTone();
            break;
        case STATE_BUSY:
            if (pressedKey == key) {
                setFsmState (STATE_RECV_UDP);
//                stopRingBackTone();
                CAN::getInstance()->configLed(pressedKey, false);
            }
            break;
        }
        break;
    }
}

void Call_Control::sendUnlinkUdp()
{
    const int capacity =  JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc(capacity);
//    StaticJsonDocument<capacity>doc;

    osMutexWait(Call_Control::getInstance()->mutex_outputJson_id, osWaitForever);
    uint16_t tempSize = strlen(outputJson);
    char temp[tempSize];
    std::fill(temp, temp + tempSize, 0);
    for (uint16_t i = 0; i < tempSize; ++i)
        temp[i] = outputJson[i];
    osMutexRelease(Call_Control::getInstance()->mutex_outputJson_id);

    if (Json::getInstance()->deserialize(doc, temp, strlen(outputJson))) {
        if (callType == Circular || callType == Conference) {
            doc["distId"].clear();

            osMutexWait(mutexDymanicStore_id, osWaitForever);
            for (uint16_t i = 0; i < dynamicStoreKeysBuff.size(); ++i) {
                doc["distId"].add(dynamicStoreKeysBuff[i]);
            }
            osMutexRelease(mutexDymanicStore_id);
        }
        doc["linkStatus"] = 0x00;
        sendJson(doc, capacity/*, UDP_PORT*/);
        //delete[] outputJson;

    }
}

void waitAnswerThread(const void *arg)
{
    (void)arg;

    while(true) {

        osSignalWait(0x09, osWaitForever);
//        stopRingTone();
        Call_Control::getInstance()->setFsmState (STATE_CREATE_RTP);

        osMutexWait(Call_Control::getInstance()->mutex_outputJson_id, osWaitForever);
        sendUdpMulticast(Call_Control::getInstance()->outputJson, strlen(Call_Control::getInstance()->outputJson));
        osMutexRelease(Call_Control::getInstance()->mutex_outputJson_id);

        osDelay(10);
        rtpCreate(ownId, 2);
        osDelay(10);

        osThreadTerminate( Call_Control::getInstance()->waitAnswerThreadId);
    }
}

void Call_Control::deleteDynamicStoreKey()
{
    osMutexWait(mutexDymanicStore_id, osWaitForever);
    size_t size = dynamicStoreKeysBuff.size();
    for (uint8_t n = 0; n < size; ++n) {
        if ( *(dynamicStoreIter + n) == distId) {
            size_t size = getKeysRingBuffData(Size);
            for (uint16_t i = 0; i < size; ++i) {
                if (getKeysRingBuffData(Assign, i) == distId) {
                    CAN::getInstance()->configLed( getKeysRingBuffData(Key, i), false);
                    dynamicStoreKeysBuff.erase(dynamicStoreIter + n);
                    break;
                }
            }
            break;
        }
    }
    emptyDynamicStoreFlag = dynamicStoreKeysBuff.empty();
    osMutexRelease(mutexDymanicStore_id);

}

void Timer1_Callback(void const *arg)
{
    (void)arg;

    if (Call_Control::getInstance()->noFixedCount < 6) {
        osMutexWait	( Call_Control::getInstance()->mutexState_id, osWaitForever);
        Call_Control::getInstance()->noFixedCount++;
        osMutexRelease ( Call_Control::getInstance()->mutexState_id);
    } else {

        if (Call_Control::getInstance()->callType == Call_Control::Group ||
                Call_Control::getInstance()->callType == Call_Control::Direct) {
            Call_Control::getInstance()->hangUp(Call_Control::User);
//            stopRingBackTone();
        } else {
            if (Call_Control::getInstance()->speakerOn) {
                Call_Control::getInstance()->speakerOn = false;
                Call_Control::getInstance()->switchMicrophone(Call_Control::OFF);

            }
        }
    }
}

void Timer2_Callback(void const *arg)
{
    (void)arg;
    if (Call_Control::getInstance()->noFixedCount < 7) {
        osMutexWait	( Call_Control::getInstance()->mutexState_id, osWaitForever);
        Call_Control::getInstance()->noFixedCount++;
        osMutexRelease ( Call_Control::getInstance()->mutexState_id);
    } else {
        CAN::getInstance()->configLed(Call_Control::getInstance()->numberPudKey, false);

        osMutexWait(Call_Control::getInstance()->mutexPadKey_id, osWaitForever);
        Call_Control::getInstance()->numberPudKey = 0;
        osMutexRelease(Call_Control::getInstance()->mutexPadKey_id);

        Call_Control::getInstance()->stopTimer(Call_Control::getInstance()->timer2_id, Call_Control::getInstance()->timer2_status);

        osMutexWait	( Call_Control::getInstance()->mutexState_id, osWaitForever);
        Call_Control::getInstance()->noFixedCount = 0;
        osMutexRelease ( Call_Control::getInstance()->mutexState_id);
    }
}

void Timer3_Callback(void const *arg)
{
    (void)arg;
    osMutexWait(Call_Control::getInstance()->mutexDymanicStore_id, osWaitForever);
    Call_Control::getInstance()->emptyDynamicStoreFlag= Call_Control::getInstance()->dynamicStoreKeysBuff.empty();
    size_t sizeDynamicStore = Call_Control::getInstance()->dynamicStoreKeysBuff.size();
    osMutexRelease(Call_Control::getInstance()->mutexDymanicStore_id);
    Call_Control::getInstance()->outgoingTelCall = true;

    if ( !Call_Control::getInstance()->emptyDynamicStoreFlag&& sizeDynamicStore > 2) {

        Call_Control::getInstance()->createSend(nullptr);
        CAN::getInstance()->configLed( Call_Control::Asterisk, true, 300, 300);
    } else {
        CAN::getInstance()->configLed( Call_Control::Asterisk, false);
        Call_Control::getInstance()->setCallType (Call_Control::NoType);

        Call_Control::getInstance()->outgoingTelCall = false;

        osMutexWait(Call_Control::getInstance()->mutexDymanicStore_id, osWaitForever);
        Call_Control::getInstance()->dynamicStoreKeysBuff.clear();
        osMutexRelease(Call_Control::getInstance()->mutexDymanicStore_id);
    }
    Call_Control::getInstance()->stopTimer(Call_Control::getInstance()->timer3_id, Call_Control::getInstance()->timer3_status);

}

void Timer4_Callback(void const *arg)
{
    (void)arg;
//    Call_Control::KeyState mode = *((Call_Control::KeyState*)(arg));
//    if (Call_Control::getInstance()->inputKeyMode == Call_Control::Fixed) {
    Call_Control::getInstance()->stopTimer(Call_Control::getInstance()->timer4_id, Call_Control::getInstance()->timer4_status);


    if (/*(Call_Control::getInstance()->inputKeyMode == Call_Control::NoFixed &&
            incomingCallType != Call_Control::Direct) ||*/
        (Call_Control::getInstance()->inputKeyMode == Call_Control::NoFixed &&
         Call_Control::getInstance()->callType != Call_Control::Direct &&
         Call_Control::getInstance()->assignedKey == Call_Control::getInstance()->setBufInputButton &&
         incomingCallType != Call_Control::Direct) ||
        Call_Control::getInstance()->assignedKey == Call_Control::Asterisk) {

        Call_Control::getInstance()->switchMicrophone(Call_Control::OFF);

        if (Call_Control::getInstance()->callType == Call_Control::Group ||
                Call_Control::getInstance()->callType == Call_Control::Direct) {
            Call_Control::getInstance()->hangUp(Call_Control::User);
        }
    }
    Call_Control::getInstance()->setBufInputCount = 0;
    Call_Control::getInstance()->setBufInputButton = 0;
}

[[ noreturn ]]
void outgoingCallThread(const void *arg)
{
    (void)(arg);
    osEvent  evt;
    Keys* key = nullptr;
    while (true) {
        evt = osMessageGet(Call_Control::getInstance()->message_call_id, osWaitForever);  // wait for message
        if (evt.status == osEventMessage) {
            key = (Keys*)evt.value.p;
            Call_Control::getInstance()->fsmSendControl(*key);
            osDelay(1);
        }
    }
}

void Call_Control::switchMicrophone(Switch control)
{
    I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00);
    I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, control);
}

void Call_Control::setCallType (uint8_t type)
{
    osMutexWait	(mutexCallType_id, osWaitForever);
    callType = type;
    osMutexRelease (mutexCallType_id);
}

void Call_Control::setFsmState (State state)
{
    osMutexWait	(mutexCallType_id, osWaitForever);
    state_ = state;
    osMutexRelease (mutexCallType_id);
}

uint16_t Call_Control::getKeysRingBuffData(Call_Control::GetKeysData control, uint8_t i)
{
    uint16_t retVal = 0;
    osMutexWait(mutexKeyBuffer_id, osWaitForever);
    switch (control) {
    case Key:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].key;
        break;
    case Assign:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].assign;
        break;
    case Function:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].function;
        break;
    case Priority:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].priority;
        break;
    case Mode:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].mode;
        break;
    case Link:
        retVal = Json::getInstance()->thisStation.keysRingBuffer[i].link;
        break;
    case Size:
        retVal = Json::getInstance()->thisStation.keysRingBuffer.size();
        break;
    }
    osMutexRelease(mutexKeyBuffer_id);
    return retVal;
}

inline osStatus Call_Control::stopTimer(const osTimerId id, osStatus& status)
{
    if (status == osOK) {
        osMutexWait	(mutexTimerState_id, osWaitForever);
        status = osTimerStop (id);            // timer stopping
        osMutexRelease (mutexTimerState_id);
        if (status != osOK)  {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
        status = osErrorOS;
    }
    return status;
}

inline osStatus Call_Control::startTimer(const osTimerId id, osStatus& status, uint16_t timerDelay)
{
    osMutexWait(mutexTimerState_id, osWaitForever);
    status = osTimerStart (id, timerDelay); // timer starting
    osMutexRelease(mutexTimerState_id);
    if (status != osOK)  {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    return status;
}

bool Call_Control::sendInfoAboutStation(JsonDocument &doc)
{
    bool hasWho = doc.containsKey("who");
    if (hasWho) {

        uint8_t MACAddr[6] = {0};

        const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(6);
        StaticJsonDocument<capacity> infoDoc;

        infoDoc["ID"] = Json::getInstance()->thisStation.id;
        uint8_t *currentFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE);
        uint8_t *currentSubFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE + 1);
        infoDoc["Version"] = *currentFirmware;
        infoDoc["Sub"] = *currentSubFirmware;
        infoDoc["IP"] = Json::getInstance()->thisStation.ip;

        MACAddr[0] = 0x40;
        MACAddr[1] = 0x8d;
        MACAddr[2] = 0x5c;
        MACAddr[3] = 0xba;
        MACAddr[4] = 0xf6;
        MACAddr[5] = 0x22;

        for (auto &n : MACAddr) {
            infoDoc["MAC"].add(n);
        }

        sendJson(infoDoc, capacity/*, UDP_PORT*/);
        return hasWho;
    } else return hasWho;
}

inline void Call_Control::createSend(Keys* arg)
{
    osMessagePut( message_call_id, (uint32_t)arg, osWaitForever);

    //    osDelay(10);
//    Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
//    osMutexWait(mutexCreateSend_id, osWaitForever);

//    do {
//        if ((UdpJsonExch::getInstance()->sendUdpThreadId = osThreadCreate(osThread(sendUdpThread), (void*)arg)) == nullptr) {
////
//            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
//            osDelay(10);
//        }
//    } while(UdpJsonExch::getInstance()->sendUdpThreadId == nullptr);
//    osMutexRelease(mutexCreateSend_id);
}

void Call_Control::setMode(uint8_t func)
{
    setCallType(func);
//    noFixedKey = pressedKey;
//    keyMode = getKeysRingBuffData(Mode, i);

//    if (keyMode == Fixed) {
    switchMicrophone(ON);

//    }



    createSend(&foundKey);

    assignedKey = pressedKey;
//}

    CAN::getInstance()->configLed(pressedKey);
}

bool Call_Control::dropMissedCall(uint8_t key)
{
    bool retVal = true;
    osMutexWait( mutexMissedCall_id, osWaitForever);
    if (!missedCall.empty()) {
        for (uint8_t j = 0; j < missedCall.size(); ++j) {
            if ( *(missedCallIter + j) == key) {
                missedCall.erase(missedCallIter + j);
                retVal = false;
                break;
            }
        }
    }
    osMutexRelease( mutexMissedCall_id);
    return retVal;
}
