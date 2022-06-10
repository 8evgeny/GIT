
#include <cstring>
#include <cmath>
#include "callcontrol.h"
#include "../UDP_JSON/udp_multicast.h"

#include "callwaiting.h"
#include "duplexdirectcall.h"
#include "simplexdirectcall.h"
#include "groupcall.h"
#include "circularcall.h"
#include "conferencecall.h"
#include "telephonecall.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t pressedKey = 0; /*!< The variable stores a number of the pressed key */
uint8_t handleClick_count = 0; /*!< The variable is the counter to handle clicks */

uint8_t keyMode = 0, /*!< The variable stores a mode of the pressed key */
        func = 0;    /*!< The variable stores a function of the pressed key */


constexpr static uint16_t AUTO_ANSW_TIMEOUT {2000};

//uint8_t checkGroupCounter = 1;

//static OSTimer osTimer_;
//static Keys subjectKey_;


#ifdef __cplusplus
}
#endif

CallControl::CallControl(State *state) : state_(nullptr)
{
//    osTimer = &osTimer_;
//    subjectKey = &subjectKey_;

    dynamicStorage.reserve(10);
    dynamicStorageIter = dynamicStorage.begin();

    busyDynamicStorage.reserve(10);
    telephoneDynamicStorage.reserve(5);
//    inputBuff.reserve(10);

    for (uint8_t i = 0; i < sizeof (keypadArray); ++i) {
        keypadStructArray[i].n = keypadArray[i];
        keypadStructArray[i].i = i;
    }

    this->TransitionTo(state);
}

CallControl::~CallControl()
{
    delete state_;
}

void CallControl::TransitionTo(State *state)
{
    if (this->state_ != nullptr)
        delete this->state_;
    this->state_ = state;
    this->state_->set_context(this);
}


#ifndef SC2BOARD
void CallControl::button(uPackageRx pack)
{
#else
void CallControl::button(PackageRx pack)
{
#endif

    if (handleClick(pack)) {
        this->state_->handleButton();
    }
}

void CallControl::jsonMessage(JsonDocument &doc)
{
    messageData.parse(doc);
    this->state_->handleJsonMessage();
}

void CallControl::handleNotFixedButton()
{
    this->state_->handleButton();
}

void CallControl::hungUp()
{
    this->state_->handleLostLink();
}

void CallControl::checkRequest()
{
    this->state_->handleRepeatedRequestCallBack();
}

void CallControl::answerUnknownCall()
{
    this->state_->handleUnknownIncomingCallBack();
}

bool CallControl::detectSubject(uint16_t subject)
{
    uint16_t size = getSubjectData(Size);
    for(uint8_t i = 0; i < size; ++i ) {

        subjectKey = Json::getInstance()->thisStation.keysBuffer[i];
        if (subjectKey.assign == subject) {
            return true;
        }
    }
    return false;
}

uint8_t CallControl::getKey(const uint16_t subject)
{
    uint16_t id = 0;

    uint16_t size = getSubjectData(Size);
    for(uint8_t i = 0; i < size; ++i ) {

        id = getSubjectData(Assign, i);
        if (id == subject) {
            return static_cast<uint8_t>(getSubjectData(Key, i));
        }
    }
    return 0;
}

bool CallControl::detectSubjCallType(uint8_t func)
{
    uint16_t size = getSubjectData(Size);
    uint8_t foundFunc = 0;

    for(uint8_t i = 0; i < size; ++i ) {

        foundFunc = static_cast<uint8_t>(getSubjectData(Function, i));
        if (foundFunc == func) {
            assignedData.conferenceKey = static_cast<uint8_t>(getSubjectData(Key, i));
            assignedData.confPriority = static_cast<uint8_t>(getSubjectData(Priority, i));
            return true;
        }
    }
    return false;
}


uint16_t CallControl::getSubjectData(CallControl::SubjectData control, uint8_t i)
{
    uint16_t retVal = 0;
//    osMutexWait(mutexKeyBufferId_, osWaitForever);
    switch (control) {
    case Key:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].key;
        break;
    case Assign:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].assign;
        break;
    case Function:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].function;
        break;
    case Priority:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].priority;
        break;
    case Mode:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].mode;
        break;
    case Link:
        retVal = Json::getInstance()->thisStation.keysBuffer[i].directLinkMode;
        break;
    case Size:
        retVal = static_cast<uint8_t>(Json::getInstance()->thisStation.keysBuffer.size());
        break;
    }
//    osMutexRelease(mutexKeyBufferId_);
    return retVal;
}

void CallControl::setCallType()
{
    switch (messageData.field.callType) {
    case Direct:
        if (messageData.field.linkData == static_cast<uint8_t>(Request::LINK)) {

            if(detectSubject(messageData.field.ownId)) {

                missedCall.remove(subjectKey.key);
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = subjectKey.key;
                assignedData.priority = subjectKey.priority;

                missedCall.remove(assignedData.key);
                switchLed(subjectKey.key, true, 250, 250);

                isIncomingCall = true;

                if (messageData.field.linkMode == Duplex) {



                    startRingTone(RingToneType::RING_TONE);
                    this->TransitionTo(new DuplexDirectCall);

                    retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK);
                    copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);

                } else if (messageData.field.linkMode == Simplex) {

                    retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK_ANSW);
                    copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                    createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                    this->TransitionTo(new SimplexDirectCall);
                }
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                messageDataBuff.field.linkMode = messageData.field.linkMode;

                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);

                if (messageData.field.linkMode == Duplex) {

                    retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK);

                    startRingTone(RingToneType::RING_UNKNOWN_TONE);
                    osTimer.start(osTimer.autoAnsw_timerId, osTimer.autoAnsw_timerStatus, AUTO_ANSW_TIMEOUT);
                    this->TransitionTo(new DuplexDirectCall);

                } else if (messageData.field.linkMode == Simplex) {

                    retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK_ANSW);

                    startRingTone(RingToneType::RING_UNKNOWN_TONE);
                    osTimer.start(osTimer.autoAnsw_timerId, osTimer.autoAnsw_timerStatus, AUTO_ANSW_TIMEOUT);
                    this->TransitionTo(new SimplexDirectCall);
                }
            }
        }
        break;
    case Group:
        if (messageData.field.linkData == static_cast<uint8_t>(Request::LINK)) {

            if(detectSubject(messageData.field.ownId)) {

                messageData.field.prevOwnId  = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;

                isIncomingCall = true;

                assignedData.key = subjectKey.key;
                missedCall.remove(assignedData.key);
                switchLed(subjectKey.key, true, 250, 250);

//                retransmitJsonDoc(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK);
                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);

                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new GroupCall);
//            }
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                startRingTone(RingToneType::RING_UNKNOWN_TONE);
                osTimer.start(osTimer.autoAnsw_timerId, osTimer.autoAnsw_timerStatus, AUTO_ANSW_TIMEOUT);
//                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new GroupCall);
            }
        }
        break;
    case Circular:
        if (messageData.field.linkData == 0xFF) {

            if(detectSubject(messageData.field.ownId)) {

                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                isIncomingCall = true;
                assignedData.key = subjectKey.key;

//                missedCall.remove(assignedData.key);
                switchLed(subjectKey.key, true, 250, 250);

                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK_ANSW);

                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new CircularCall);
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK_ANSW);
                startRingTone(RingToneType::RING_UNKNOWN_TONE);
                osTimer.start(osTimer.autoAnsw_timerId, osTimer.autoAnsw_timerStatus, AUTO_ANSW_TIMEOUT);
//                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new CircularCall);
            }
        }
        break;
    case Conference:
        if (messageData.field.linkData == 0xFF) {

            if(detectSubjCallType(Conference)) {

                missedCall.remove(assignedData.conferenceKey);
//                if (isInterruptConf != CallControl::InterruptConf::CALLED_PARTY) {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
//                }
                assignedData.priority = assignedData.confPriority;

                isIncomingCall = true;
                assignedData.key = assignedData.conferenceKey;

                missedCall.remove(assignedData.key);
                switchLed(assignedData.conferenceKey, true, 250, 250);
                startRingTone(RingToneType::RING_TONE);

                retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), Request::ACK);
//                memcpy (messageData.field.recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff)+1);
                copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                this->TransitionTo(new ConferenceCall);
            }
        }
        break;
#ifndef SC2BOARD
    case Telephone:
        if (messageData.field.linkData == static_cast<uint8_t>(Request::LINK)) {

            messageData.field.prevOwnId = messageData.field.ownId;
            messageData.field.prevPriority = messageData.field.priority;
            assignedData.key = CallControl::Asterisk;
            assignedData.priority = 4;
            isIncomingCall = true;
            missedCall.remove(assignedData.key);
            switchLed(assignedData.key, true, 250, 250);
            control = Control::NONE;
//            sendUdpMulticast(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff));
            copyRecvBuff(messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
            sendRequest(Request::ACK);
//            memcpy (messageData.field.recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff)+1);

            startRingTone(RingToneType::RING_TONE);
            this->TransitionTo(new TelephoneCall);
        }
        break;
#endif
    default:
        break;
    }
}
#ifndef SC2BOARD
bool CallControl::handleClick(uPackageRx pack)
{

    if (pack.s.packetType == CAN::getInstance()->button) {

        if (pressedKey == pack.s.payloadData) {

#else
bool CallControl::handleClick(PackageRx pack)
{
    if (pack.packetType == GPIO::getInstance()->button) {

        if (pressedKey == pack.payloadData) {
#endif

            if (osTimer.button_timerStatus != osOK) {
                handleClick_count++;

                if (handleClick_count > 2 && foundKeyFlag_) {

                    osTimer.start(osTimer.button_timerId, osTimer.button_timerStatus, 200);

                    if (missedCall.isMissedKey) {
                        missedCall.remove(static_cast<uint8_t>(pressedKey));
                        missedCall.isMissedKey = false;
                    } else {
                        return true;
                    }
                }

            } else {
                osTimer.start(osTimer.button_timerId, osTimer.button_timerStatus, 200);
            }

        } else {
#ifndef SC2BOARD
            pressedKey = pack.s.payloadData;
#else
            pressedKey = pack.payloadData;
#endif


            handleClick_count = 1;
            osTimer.stop(osTimer.button_timerId, osTimer.button_timerStatus);
            foundKeyFlag_ = false;

            missedCall.isMissedKey = missedCall.seek(static_cast<uint8_t>(pressedKey));
            if (!missedCall.isMissedKey) {

                for(uint8_t i = 0; i < getSubjectData(Size); ++i ) {

                    if ((getSubjectData(Key, i)) == pressedKey) {
                        subjectKey = Json::getInstance()->thisStation.keysBuffer[i];
                        foundKeyFlag_ = true;

                        keyMode = subjectKey.mode;
                        func = subjectKey.function;

                        break;
                    }
                }

                // The code to support the telephone calls
                /*-------------------------------------------------------------------------*/
                if (pressedKey == Asterisk ) {
                    subjectKey.key = Asterisk;
                    func = subjectKey.function = Telephone;
                    keyMode = subjectKey.mode = NotFixed;
                    subjectKey.assign = 0;
                    subjectKey.type = 0;
                    subjectKey.priority = 4;
                    foundKeyFlag_ = true;
                } else if (pressedKey == Hash) {
                    subjectKey.key = Hash;
                    subjectKey.function = Telephone;
                    subjectKey.mode = Fixed;
                    subjectKey.assign = 0;
                    subjectKey.type = 0;
                    foundKeyFlag_ = true;
                }

                if (func == Telephone && pressedKey != Asterisk )
                    for (auto& var : keypadStructArray)
                        if (pressedKey == var.n) {
                            subjectKey.key = static_cast<uint8_t>(pressedKey);
                            foundKeyFlag_ = true;
                        }
                /*-------------------------------------------------------------------------*/

            } else {
                foundKeyFlag_ = true;
            }

        }
    }
    return false;
}

bool CallControl::sendInfoAboutStation(JsonDocument &doc)
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

        std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
        if (serializeJson(infoDoc, messageData.txBuff, capacity) > 0) {
            sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
        }
//        sendJson(infoDoc, capacity);
        return hasWho;
    } else return hasWho;
}



void CallControl::createRtp(uint32_t port, uint8_t rtpType)
{
    if (rtpStatus != OK_RTP) {
        rtpStatus = rtpCreate(port, rtpType);
    }
}

ErrorCode CallControl::removeRtp()
{
    if (rtpStatus == OK_RTP) {
        rtpRemove();
        rtpStatus = FAIL_RTP;
    }
    return rtpStatus;
}



bool CallControl::seekDynamicStorage(std::vector <uint16_t>& v, uint16_t subject)
{
    if (!v.empty()) {
        for (uint8_t i = 0; i < v.size(); ++i) {
            if (*(v.begin() + i) == subject) {
                return true;
            }
        }
    }
    return false;
}


bool CallControl::popDynamicStorage(std::vector <uint16_t>& v, uint16_t subject)
{
    uint8_t key = 0;
    if (!v.empty()) {
        for (uint8_t i = 0; i < v.size(); ++i) {
            if (*(v.begin() + i) == subject) {
                key = getKey(subject);
                switchLed(key, false);
//                osDelay(5);
                v.erase((v.begin() + i));
                return true;
            }
        }
    }
    return false;
}


bool CallControl::pushDynamicStorage(uint8_t callType, std::vector <uint16_t>& v, uint16_t subject)
{
//    bool isRemoved = popDynamicStorage(v, subject);
    uint8_t n = 0;
    switch (callType) {
    case Circular:
        n = 10;
        break;
    case Conference:
        n = 5;
        break;
    default:
        break;
    }

    uint8_t key = 0;
//    if (!isRemoved)
    if (v.size() < n) {
        if (!seekDynamicStorage(v, subject)) {
            v.push_back(subject);
            key = getKey(subject);
            missedCall.remove(key);
            switchLed(key, true);
            return true;
        }
    }
    return false;
}

void CallControl::sendMessage(const uint16_t distId, const uint8_t func, const Request linkData)
{
    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    doc["Own_Id"] = Json::getInstance()->thisStation.id;
    doc["Dist_Id"].add(distId);
    doc["Call_Type"] = func;
    doc["Priority"] = assignedData.priority;
//    doc["Dist_Priority"] = 0;
    doc["Link_Data"] = static_cast<uint8_t>(linkData);
    if (func == Direct)
        doc["Direct_Link_Mode"] = subjectKey.directLinkMode;

    std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
    if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
        sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
    }


}

void CallControl::sendMessage(const Request linkData)
{
    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    doc["Own_Id"] = Json::getInstance()->thisStation.id;
    doc["Dist_Id"].add(subjectKey.assign);
    doc["Call_Type"] = subjectKey.function;
    doc["Priority"] = subjectKey.priority;
//    doc["Dist_Priority"] = 0;
    doc["Link_Data"] = static_cast<uint8_t>(linkData);
    if (subjectKey.function == Direct)
        doc["Direct_Link_Mode"] = subjectKey.directLinkMode;

    std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
    if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
        sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
    }
}

void CallControl::sendMessage(const uint16_t arr[], const uint16_t size, const Request linkData)
{
    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    doc["Own_Id"] = Json::getInstance()->thisStation.id;
    for (uint16_t i = 0; i < size; ++i) {
        doc["Dist_Id"].add(arr[i]);
    }
    doc["Call_Type"] = subjectKey.function;
    doc["Priority"] = subjectKey.priority;
//    doc["Dist_Priority"] = 0;
    doc["Link_Data"] = static_cast<uint8_t>(linkData);
    if (subjectKey.function == Direct)
        doc["Direct_Link_Mode"] = subjectKey.directLinkMode;

    std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
    if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
        sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
    }
}

bool CallControl::retransmitMessage(char* arr, const size_t size, const Request linkData)
{
    bool ret = false;
    const int capacity = JSON_OBJECT_SIZE(6)  + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);
    ret = Json::getInstance()->deserialize(doc, reinterpret_cast<void *>(arr), size);
    if (ret) {
        doc["Link_Data"] = static_cast<uint8_t>(linkData);

        std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
        if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
            sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
        }
    }
    return ret;
}

//bool CallControl::retransmitMessage(char* arr, const size_t size,/* const uint8_t distPriority,*/ const Request linkData)
//{
//    bool ret = false;
//    const int capacity = JSON_OBJECT_SIZE(6)  + JSON_ARRAY_SIZE(100);
//    DynamicJsonDocument doc (capacity);
//    ret = Json::getInstance()->deserialize(doc, reinterpret_cast<void *>(arr), size);
//    if (ret) {
////        doc["Dist_Priority"] = distPriority;
//        doc["Link_Data"] = static_cast<uint8_t>(linkData);

//        std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
//        if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
//            sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
//        }
//    }
//    return ret;
//}

bool CallControl::retransmitMessage(char* arr, const size_t size, const uint16_t distId, const Request linkData)
{
    bool ret = false;
    const int capacity = JSON_OBJECT_SIZE(6)  + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);
    ret = Json::getInstance()->deserialize(doc, reinterpret_cast<void *>(arr), size);
    if (ret) {
        doc["Dist_Id"].clear();
        doc["Dist_Id"].add(distId);
        doc["Link_Data"] = static_cast<uint8_t>(linkData);

        std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
        if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
            sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
        }
    }
    return ret;
}

void CallControl::resetData()
{
//    messageData.field.prevOwnId = 0;                      //commented to check conf returning
//    messageData.field.prevPriority = 0;                   //commented to check conf returning
    assignedData.key = 0;
    assignedData.priority = 0;
    isIncomingCall = false;
    isAnsweredCall = false;
    //    messageData.field.isAnswerToRequest = false;
//    messageData.field.isHangUp = false;
    control = Control::READY;
    requestCount = 0;                                           //added the request count
//    messageData.field.requestCount = 0;

//    if (messageData.field.recvBuffCopy != nullptr) {
//        delete [] messageData.field.recvBuffCopy;
//        messageData.field.recvBuffCopy = nullptr;
//    }
}

void CallControl::copyRecvBuff(char* &copyBuff, const char* buff)
{
    if (copyBuff != nullptr)
        delete [] copyBuff;

    size_t size = strlen(buff) + 1;
    copyBuff = new char [size];
    memcpy (copyBuff, buff, size);
}

void CallControl::sendRequest(uint8_t callType, Request reqType, uint16_t timeout)
{
    requestCount = 0;

    switch (callType) {
    case Direct: {
        sendMessage(reqType);
        requestCount++;
    }
    break;
    case Group: {
        checkGroupIndex = 0;
        uint16_t size = Json::getInstance()->thisStation.groupsBuffer[assignedData.id].stSize;
        sendMessage(Json::getInstance()->thisStation.groupsBuffer[assignedData.id].stantions, size, reqType);
    }
    break;
    default:
        break;
    }
    osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, timeout);
}

constexpr static uint16_t HANG_UP_TIMEOUT {100};
constexpr static uint16_t TIMEOUT {300};

void CallControl::sendRequest(Request reqType)
{
    switch (reqType) {
    case Request::HANG_UP:
        requestCount = 0;
        retransmitMessage(messageData.recvMessageBuff, strlen(messageData.recvMessageBuff), Request::HANG_UP);
        osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, HANG_UP_TIMEOUT);
        requestCount++;
        break;
    case Request::ACK:
        retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
        break;
    case Request::ACK_ANSW:
        requestCount = 0;
        control = Control::ANSWER;
        retransmitMessage(messageData.recvMessageBuff, strlen(messageData.recvMessageBuff), Request::ACK_ANSW);
        osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, TIMEOUT);
        requestCount++;
        break;
    case Request::BUSY:
        if (messageData.distIdArrSize == 1) {
            requestCount = 0;
            control = Control::BUSY;
            retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), messageData.field.distId, CallControl::Request::BUSY);
            osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, TIMEOUT);
            requestCount++;
        }
        break;
    case Request::CHANGE_CONF:
        requestCount = 0;
        retransmitMessage(messageData.recvMessageBuff, strlen(messageData.recvMessageBuff), Request::CHANGE_CONF);
        osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, TIMEOUT);
        requestCount++;
        break;
    case Request::RETURN_CONF:
        requestCount = 0;
        switch (isInterruptConf) {
        case InterruptConf::CALLED_PARTY:
            retransmitMessage(messageData.recvMessageBuff, strlen(messageData.recvMessageBuff), Request::RETURN_CONF);
            break;
        case InterruptConf::CALLER:
            dynamicStorageIter = dynamicStorage.begin();
            sendMessage(*dynamicStorageIter, CallControl::Conference, Request::RETURN_CONF);
            break;
        default:
            break;
        }
        osTimer.start(osTimer.request_timerId, osTimer.request_timerStatus, TIMEOUT);
        requestCount++;
        break;
    default:
        break;
    }
}

bool CallControl::switchToConf()
{
    if(isInterruptConf != InterruptConf::NONE) {

        if (isInterruptConf == InterruptConf::CALLED_PARTY) {
            isIncomingCall = true;
            isAnsweredCall = true;
//            assignedData.key = assignedData.conferenceKey;
            messageData.field = calledPartyConfDataBuff.field;
            copyRecvBuff(messageData.recvMessageBuff, calledPartyConfDataBuff.recvMessageBuff);
//            calledPartyConfDataBuff.clear();


            //clear recvMessageBuff of the calledPartyConfDataBuff object
//            if (calledPartyConfDataBuff.recvMessageBuff != nullptr) {
//                delete [] calledPartyConfDataBuff.recvMessageBuff;
//                calledPartyConfDataBuff.recvMessageBuff = nullptr;
//            }
        } else {
            messageData.field = {0};
        }
//        *serviceData = *serviceBuff;

        if(!dynamicStorage.empty()) {
            uint8_t key = 0;
            for (auto &n : dynamicStorage) {
                key = getKey(n);
                switchLed(key, true);
                osDelay(10);
            }
        }

        switchLed(assignedData.conferenceKey, true);
        assignedData.key = assignedData.conferenceKey;
        assignedData.priority = assignedData.confPriority;
        TransitionTo(new ConferenceCall);

        if (isInterruptConf == InterruptConf::CALLER) {
            createRtp(Json::getInstance()->thisStation.id, Duplex_type);
        } else if (isInterruptConf == InterruptConf::CALLED_PARTY) {
            createRtp(messageData.field.prevOwnId, Duplex_type);
            control = Control::RETURN_CONF;
            sendRequest(Request::RETURN_CONF);
        }

        microphone.start();
        isInterruptConf = InterruptConf::NONE;
        return true;
    }
    return false;
}

void switchLed(uint8_t ledNumber, bool ledOn, uint16_t timeOn, uint16_t timeOff, uint8_t repeatNum)
{
#ifndef SC2BOARD
    CAN::getInstance()->configLed(ledNumber, ledOn, timeOn, timeOff, repeatNum);
#else
    GPIO::getInstance()->configLed(ledNumber, ledOn, timeOn, timeOff, repeatNum);
#endif
}
