
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
#include "rs232.h"
#include"rs232_printf.h"
extern uint8_t macAdr5;
extern char* fwInfo;
#ifdef __cplusplus
extern "C" {
#endif

uint16_t pressedKey = 0; /*!< The variable stores a number of the pressed key */
uint8_t handleClick_count = 0; /*!< The variable is the counter to handle clicks */
uint8_t ledIndicateAsterisk = 0;
uint8_t keyMode = 0, /*!< The variable stores a mode of the pressed key */
        func = 0;    /*!< The variable stores a function of the pressed key */

extern uint8_t boardType;
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
    telephoneDynamicStorage.reserve(3);
//    inputBuff.reserve(10);

    //Инициализация номеронабирателя
    for (uint8_t i = 0; i < sizeof (keypadArray); ++i)
    {
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

void CallControl::button(PackageRx pack)
{

    if (handleClick(pack))
    {
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
    for(uint8_t i = 0; i < size; ++i )
    {
        subjectKey = ThisStation_.keysBuffer[i];
        if (subjectKey.assign == subject) {
            return true;
        }
    }
    return false;
}

uint8_t CallControl::getKey(const uint16_t subject)
{
    uint16_t id = 0;
    uint8_t ret = 0;

    uint16_t size = getSubjectData(Size);
    for(uint8_t i = 0; i < size; ++i )
    {
        id = getSubjectData(Assign, i);
        if (id == subject)
        {
            ret = static_cast<uint8_t>(getSubjectData(Key, i));
        }
    }
    //Теперь проверяем контекст и если дуплекс - ищем правильное значение


    return ret;
}

bool CallControl::detectSubjCallType(uint8_t func)
{
    uint16_t size = getSubjectData(Size);
    uint8_t foundFunc = 0;

    for(uint8_t i = 0; i < size; ++i )
    {
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
        retVal = ThisStation_.keysBuffer[i].key;
        break;
    case Assign:
        retVal = ThisStation_.keysBuffer[i].assign;
        break;
    case Function:
        retVal = ThisStation_.keysBuffer[i].function;
        break;
    case Priority:
        retVal = ThisStation_.keysBuffer[i].priority;
        break;
    case Mode:
        retVal = ThisStation_.keysBuffer[i].mode;
        break;
    case Link:
        retVal = ThisStation_.keysBuffer[i].directLinkMode;
        break;
    case Size:
        retVal = static_cast<uint8_t>(ThisStation_.keysBuffer.size());
        break;
    }
//    osMutexRelease(mutexKeyBufferId_);
    return retVal;
}

void CallControl::setCallType()
{
    switch (messageData.field.callType)
    {
    case Direct:
        if (messageData.field.linkData == static_cast<uint8_t>(Request::LINK)) {

            if(detectSubject(messageData.field.ownId)) {

                missedCall.remove(subjectKey.key);
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = subjectKey.key;
                assignedData.priority = subjectKey.priority;

                missedCall.remove(assignedData.key);

                if (boardType != sl1) switchLed(subjectKey.key, true, 250, 250, 0, GPIO::GREEN);
                if (boardType == sl1) switchLed(subjectKey.key, true, 0, 0, 0, GPIO::GREEN);

                isIncomingCall = true;

                if (messageData.field.linkMode == Duplex) {

                    startRingTone(RingToneType::RING_TONE);
                    this->TransitionTo(new DuplexDirectCall);

                    retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK);
                    copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);

                } else if (messageData.field.linkMode == Simplex) {

                    retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK_ANSW);
                    copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
                    createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                    this->TransitionTo(new SimplexDirectCall);
                }
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                messageDataBuff.field.linkMode = messageData.field.linkMode;

                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);

                if (messageData.field.linkMode == Duplex) {

                    retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK);

                    startRingTone(RingToneType::RING_UNKNOWN_TONE);
                    osTimer.start(osTimer.autoAnsw_timerId, osTimer.autoAnsw_timerStatus, AUTO_ANSW_TIMEOUT);
                    this->TransitionTo(new DuplexDirectCall);

                } else if (messageData.field.linkMode == Simplex) {

                    retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK_ANSW);

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
                if (boardType != sl1) switchLed(subjectKey.key, true, 250, 250, 0, GPIO::GREEN);
                if (boardType == sl1) switchLed(subjectKey.key, true, 0, 0, 0, GPIO::GREEN);
//                retransmitJsonDoc(RecvBuff_, strlen(RecvBuff_), Request::ACK);
                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);

                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new GroupCall);
//            }
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
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
                switchLed(subjectKey.key, true, 250, 250, 0, GPIO::GREEN);//Цвет принимаемый циркулярный вызов

                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
                retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK_ANSW);

                createRtp(messageData.field.prevOwnId, Simplex_recv_type);
                this->TransitionTo(new CircularCall);
            } else {
                messageData.field.prevOwnId = messageData.field.ownId;
                messageData.field.prevPriority = messageData.field.priority;
                assignedData.key = 0;
                isIncomingCall = true;
                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
                retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK_ANSW);
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
                switchLed(assignedData.conferenceKey, true, 250, 250, 0, GPIO::GREEN); //Цвет запрос на прием вызова
                startRingTone(RingToneType::RING_TONE);

                retransmitMessage(RecvBuff_, strlen(RecvBuff_), Request::ACK);
//                memcpy (messageData.field.recvBuffCopy, RecvBuff_, strlen(RecvBuff_)+1);
                copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
                this->TransitionTo(new ConferenceCall);
            }
        }
        break;

    case Telephone :
        if (messageData.field.linkData == static_cast<uint8_t>(Request::LINK))
        {
            messageData.field.prevOwnId = messageData.field.ownId;
            messageData.field.prevPriority = messageData.field.priority;
            assignedData.key = CallControl::Asterisk;
            assignedData.priority = 4;
            isIncomingCall = true;
            missedCall.remove(assignedData.key);
            switchLed(assignedData.key, true, 250, 250, 0, GPIO::GREEN);
            control = Control::NONE;
//            sendUdpMulticast(RecvBuff_, strlen(RecvBuff_));
            copyRecvBuff(messageData.recvMessageBuff, RecvBuff_);
            sendRequest(Request::ACK);
//            memcpy (messageData.field.recvBuffCopy, RecvBuff_, strlen(RecvBuff_)+1);

            for(uint8_t i = 0; i < getSubjectData(Size); ++i ) //Число задействованных клавиш в конфиге
            {
                if ((getSubjectData(Function, i)) == Telephone) //Определяем какой Led зажигать
                {
                    ledIndicateAsterisk = getSubjectData(Key, i);
                }
            }
            switchLed(ledIndicateAsterisk, true, 250, 250, 0, GPIO::GREEN);
            startRingTone(RingToneType::RING_TONE);
            this->TransitionTo(new TelephoneCall);
        }
        break;

    default:
        break;
    }
}

bool CallControl::handleClick(PackageRx pack)
{
bool changeToAsterisk = false;
bool changeToHash = false;

    if (pack.packetType == GPIO::getInstance()->button)
    {

        if (pressedKey == pack.payloadData)
        {

            if (osTimer.button_timerStatus != osOK)
            {
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

            }
            else
            {
                osTimer.start(osTimer.button_timerId, osTimer.button_timerStatus, 200);
            }

        }
        else
        {
            pressedKey = pack.payloadData;

            handleClick_count = 1;
            osTimer.stop(osTimer.button_timerId, osTimer.button_timerStatus);
            foundKeyFlag_ = false;

            missedCall.isMissedKey = missedCall.seek(static_cast<uint8_t>(pressedKey));
            if (!missedCall.isMissedKey)
            {
                for(uint8_t i = 0; i < getSubjectData(Size); ++i ) //Число задействованных клавиш в конфиге
                {
                    if ((getSubjectData(Function, i)) == Telephone)
                    {
                        ledIndicateAsterisk = getSubjectData(Key, i);
                    }

                    if ((getSubjectData(Key, i)) == pressedKey)
                    {
                        subjectKey = ThisStation_.keysBuffer[i];

//Добавил переопределение клавиш для возможности принимать телефонный вызовы
                        if (subjectKey.function == Telephone)
                        {
                            changeToAsterisk = true;
//                            ledIndicateAsterisk = subjectKey.key;

                            subjectKey.key = Asterisk;
                        }

                        if (subjectKey.function == HungUp)
                        {
                            changeToHash = true;

                            subjectKey.key = Hash;
                        }

                        foundKeyFlag_ = true;
                        keyMode = subjectKey.mode;
                        func = subjectKey.function;
                        break;
                    }
                }

                // The code to support the telephone calls
                /*-------------------------------------------------------------------------*/
                if ((pressedKey == Asterisk )|| changeToAsterisk)
                {
                    switchLed(ledIndicateAsterisk, true, 0, 0, 0, GPIO::GREEN);
                    subjectKey.key = Asterisk;
                    func = subjectKey.function = Telephone;
                    keyMode = subjectKey.mode = NotFixed;
                    subjectKey.assign = 0;
                    subjectKey.type = 0;
                    subjectKey.priority = 4;
                    foundKeyFlag_ = true;
                } else if ((pressedKey == Hash)|| changeToHash)
                {
                    switchLed(ledIndicateAsterisk, false);
                    subjectKey.key = Hash;
                    subjectKey.function = Telephone;
                    subjectKey.mode = Fixed;
                    subjectKey.assign = 0;
                    subjectKey.type = 0;
                    foundKeyFlag_ = true;
                }

                if (func == Telephone && pressedKey != Asterisk )
                {
                    for (auto& var : keypadStructArray)
                        if (pressedKey == var.n)
                        {
                            subjectKey.key = static_cast<uint8_t>(pressedKey);
                            foundKeyFlag_ = true;
                        }
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
term("CallControl::sendInfoAboutStation")
    bool hasWho = doc.containsKey("who");
    if (hasWho) {

        uint8_t MACAddr[6] = {0};

        const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(6);
        StaticJsonDocument<capacity> infoDoc;

        infoDoc["ID"] = ThisStation_.id;
//        uint8_t *currentFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE);
//        uint8_t *currentSubFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE + 1);
//        infoDoc["Version"] = *currentFirmware;
//        infoDoc["Sub"] = *currentSubFirmware;
        infoDoc["Version"] = fwInfo;
        infoDoc["IP"] = ThisStation_.ip;

//        MACAddr[0] = 0x40;
//        MACAddr[1] = 0x8d;
//        MACAddr[2] = 0x5c;
//        MACAddr[3] = 0xba;
//        MACAddr[4] = 0xf6;
//        MACAddr[5] = 0x22;
        MACAddr[0] = 0x00;
        MACAddr[1] = 0x80;
        MACAddr[2] = 0xE1;
        MACAddr[3] = 0x00;
        MACAddr[4] = 0x00;
        MACAddr[5] = macAdr5;
//00:80:e1:00:00:67
//        for (auto &n : MACAddr) {
//            infoDoc["MAC"].add(n);
//        }
        char macAdr[18];
        sprintf(macAdr,"%02x:%02x:%02x:%02x:%02x:%02x",
                MACAddr[0],MACAddr[1],MACAddr[2],MACAddr[3],MACAddr[4],MACAddr[5]);

        RS232::getInstance().term << "macAdr: "<<macAdr <<"\r\n";
//        std::vector <std::string> macAdr;

//        for (auto &n : MACAddr)
//        {
//            macAdr.push_back(std::to_string(n));
//            macAdr.push_back(":");
//        }
//        for (auto &n : macAdr)
//        {
//            RS232::getInstance().term << n;
//        }
//        RS232::getInstance().term << "\r\n";

osDelay(200); //pev
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
term("callcontrol.cpp")
    if (rtpStatus != OK_RTP) {
        rtpStatus = rtpCreate(port, rtpType);
    }
}

ErrorCode CallControl::removeRtp()
{
term("callcontrol.cpp")
    if (rtpStatus == OK_RTP) {
        rtpRemove();
        rtpStatus = FAIL_RTP;
    }
    return rtpStatus;
}

bool CallControl::seekDynamicStorage(std::vector <uint16_t>& v, uint16_t subject)
{
term("callcontrol.cpp")
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
term("CallControl::pushDynamicStorage")
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
            switchLed(key, true, 0,0,0, GPIO::GREEN);
            return true;
        }
    }
    return false;
}

void CallControl::sendMessage(const uint16_t distId, const uint8_t func, const Request linkData)
{
term("CallControl::sendMessage")
    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    doc["Own_Id"] = ThisStation_.id;
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

    doc["Own_Id"] = ThisStation_.id;
    doc["Dist_Id"].add(subjectKey.assign);
    doc["Call_Type"] = subjectKey.function;
    doc["Priority"] = subjectKey.priority;
//    doc["Dist_Priority"] = 0;
    doc["Link_Data"] = static_cast<uint8_t>(linkData);
    if (subjectKey.function == Direct)
        doc["Direct_Link_Mode"] = subjectKey.directLinkMode;

    std::fill(messageData.txBuff, messageData.txBuff + messageData.txBuffSize, 0);
    if (serializeJson(doc, messageData.txBuff, capacity) > 0) {
//term2(messageData.txBuff)
        sendUdpMulticast(messageData.txBuff, strlen(messageData.txBuff));
    }
}

void CallControl::sendMessage(const uint16_t arr[], const uint16_t size, const Request linkData)
{
    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    doc["Own_Id"] = ThisStation_.id;
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
//term2(messageData.txBuff)
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
//term2(messageData.txBuff)
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
term("CallControl::retransmitMessage")
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
term("CallControl::copyRecvBuff")
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
//term1("Request") term((uint8_t)reqType)
        sendMessage(reqType);
        requestCount++;
    }
    break;
    case Group: {
        checkGroupIndex = 0;
        uint16_t size = ThisStation_.groupsBuffer[assignedData.id].stSize;
        sendMessage(ThisStation_.groupsBuffer[assignedData.id].stantions, size, reqType);
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
        retransmitMessage(RecvBuff_, strlen(RecvBuff_), CallControl::Request::ACK);
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
            retransmitMessage(RecvBuff_, strlen(RecvBuff_), messageData.field.distId, CallControl::Request::BUSY);
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
                switchLed(key, true, 0,0,0, GPIO::GREEN);
                osDelay(10);
            }
        }

        switchLed(assignedData.conferenceKey, true, 0,0,0, GPIO::GREEN);
        assignedData.key = assignedData.conferenceKey;
        assignedData.priority = assignedData.confPriority;
        TransitionTo(new ConferenceCall);

        if (isInterruptConf == InterruptConf::CALLER) {
            createRtp(ThisStation_.id, Duplex_type);
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

void switchLed(uint8_t ledNumber, bool ledOn, uint16_t timeOn, uint16_t timeOff, uint8_t repeatNum, GPIO::Color colour)
{
    GPIO::getInstance()->configLed(ledNumber, ledOn, timeOn, timeOff, repeatNum, colour);
}
