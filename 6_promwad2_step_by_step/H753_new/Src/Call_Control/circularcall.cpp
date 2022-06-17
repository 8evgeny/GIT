#include "circularcall.h"
#include "../UDP_JSON/udp_multicast.h"
#include "callwaiting.h"

void CircularCall::handleButton()
{
    if (context_->isIncomingCall && context_->subjectKey.key == context_->assignedData.key) {

        hungUp();

    } else if (context_->subjectKey.function == CallControl::HungUp) {

        if (!context_->isIncomingCall)  {
            endCircularCall();
        } else {
            hungUp();
        }

    } else if (context_->subjectKey.key == context_->assignedData.key) {

        if (context_->subjectKey.mode == NotFixed) {
//            if (context_->microphone.micStatus == CallControl::OFF)
//                context_->microphone.stop();
            context_->microphone.start();
        } else {
            endCircularCall();
        }

    } else {

        if (context_->subjectKey.function == context_->Direct
                && !context_->isIncomingCall) {
//            context_->subjectKey.function = CallControl::Conference;

            if (!context_->popDynamicStorage(context_->busyDynamicStorage, context_->subjectKey.assign)) {

                context_->inputBuff.push(context_->subjectKey.assign);

                if (!context_->popDynamicStorage(context_->dynamicStorage, context_->subjectKey.assign)) {
                    if (context_->osTimer.request_timerStatus != osOK)
                        sendRequest(context_->inputBuff.first(), CallControl::Request::LINK, TIMEOUT);
                } else {
                    if (context_->osTimer.request_timerStatus != osOK) {
                        context_->control = CallControl::Control::HANG_UP;
                        sendRequest(context_->inputBuff.first(), CallControl::Request::HANG_UP, TIMEOUT);
                    }
                }
            }
        }
    }
}

void CircularCall::handleJsonMessage()
{
    switch (static_cast<CallControl::Request>(context_->messageData.field.linkData)) {
    case CallControl::Request::HANG_UP:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

                context_->sendRequest(CallControl::Request::ACK);

                context_->removeRtp();
                switchLed(context_->subjectKey.key, false);

                context_->resetData();
                if (!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->seekDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId)) {

//            memcpy(context_->messageData.field.recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff) + 1);
//            context_->sendRequest(CallControl::Request::ACK);
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);

                context_->popDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId);

                if (context_->dynamicStorage.empty()) {

                    switchOffLeds(context_->busyDynamicStorage);
                    context_->busyDynamicStorage.clear();

                    context_->microphone.stop();
                    context_->removeRtp();
                    switchLed(context_->assignedData.key, false);
                    context_->resetData();
                    this->context_->TransitionTo(new CallWaiting);
                }
            }
        }
        break;
    case CallControl::Request::LINK:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId != context_->messageData.field.prevOwnId) {

//                if ((context_->assignedData.priority > context_->messageData.field.priority || context_->assignedData.priority == 0) &&
//                        (context_->messageData.field.prevPriority > context_->messageData.field.priority || context_->messageData.field.prevPriority == 0)) {

                if ((context_->isIncomingCall && (context_->messageData.field.prevPriority > context_->messageData.field.priority)) ||
                        (!context_->isIncomingCall && (context_->assignedData.priority > context_->messageData.field.priority))) {

                    if (context_->microphone.status == Microphone::micStatus::ON)
                        context_->microphone.stop();

                    context_->removeRtp();
                    switchLed(context_->assignedData.key, false);

                    if (context_->isIncomingCall) {

                        context_->control = CallControl::Control::EXCH_CALL_TYPE;
                        context_->messageDataBuff.field = context_->messageData.field;                  //it is copying incoming json to the buffer
                        context_->copyRecvBuff(context_->messageDataBuff.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);

                        context_->sendRequest(CallControl::Request::HANG_UP);

                    } else {

                        switchOffLeds(context_->busyDynamicStorage);
                        context_->busyDynamicStorage.clear();

                        switchOffLeds(context_->dynamicStorage);
                        context_->dynamicStorage.clear();

                        context_->resetData();
                        context_->setCallType();
                    }

                } else {
                    uint8_t key = context_->getKey(context_->messageData.field.ownId);
                    if (key) {
                        context_->missedCall.add(key);
                    }
                    context_->sendRequest(CallControl::Request::BUSY);
                }
            } else {
                context_->retransmitMessage(context_->messageData.recvMessageBuff, strlen(context_->messageData.recvMessageBuff), CallControl::Request::ACK_ANSW);
            }
        }
        break;

    case CallControl::Request::ACK:
        handleAck();
        break;
    case CallControl::Request::ACK_ANSW:

        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (!context_->inputBuff.isEmpty()) {
                if (context_->messageData.field.distId == context_->inputBuff.first()) {

                    if (context_->rtpStatus != OK_RTP)
                        context_->createRtp(Json::getInstance()->thisStation.id, context_->Simplex_send_type);

                    context_->pushDynamicStorage(CallControl::Circular, context_->dynamicStorage, context_->inputBuff.shift());

                    if (!context_->inputBuff.isEmpty()) {
                        context_->requestCount = 0;
                        sendRequest(context_->inputBuff.first(), CallControl::Request::LINK, TIMEOUT);
                        context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, TIMEOUT);
                        context_->requestCount++;
                    } else {
                        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    }
                }
            }

            if (!context_->busyDynamicStorage.empty()) {
                if (context_->popDynamicStorage(context_->busyDynamicStorage, context_->messageData.field.distId)) {
                    context_->pushDynamicStorage(CallControl::Circular, context_->dynamicStorage, context_->messageData.field.distId);
                }
            }
        }
        break;
    case CallControl::Request::BUSY:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->inputBuff.first()) {
                uint16_t assign = context_->inputBuff.shift();
                uint8_t key = context_->getKey(assign);
                context_->busyDynamicStorage.push_back(assign);
                switchLed(key, true, 900, 100);

//                context_->busyDynamicStorage.push_back(context_->inputBuff.shift());
//                switchLed(context_->subjectKey.key, true, 900, 100);
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
            }
        }
        break;
    default:
        break;
    }
}

void CircularCall::handleLostLink()
{
    if (context_->isIncomingCall) {
        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
        context_->microphone.stop();
        context_->removeRtp();
        switchLed(context_->subjectKey.key, false);
        context_->resetData();
        if(!context_->switchToConf())
            context_->TransitionTo(new CallWaiting);
    }
}

void CircularCall::handleRepeatedRequestCallBack()
{
    if (context_->osTimer.request_timerStatus == osOK) {

        if (context_->requestCount < 3) {
            sendUdpMulticast(context_->messageData.txBuff, strlen(context_->messageData.txBuff));
            context_->requestCount++;
        } else {

            switch (context_->control) {
            case CallControl::Control::HANG_UP: {
                if (context_->isIncomingCall) {
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    context_->resetData();
                    if (!context_->switchToConf())
                        this->context_->TransitionTo(new CallWaiting);
                } else {
                    context_->inputBuff.shift();
                    if (!context_->inputBuff.isEmpty()) {
                        sendRequest(context_->inputBuff.first(), CallControl::Request::HANG_UP, TIMEOUT);
                    } else {
                        context_->control = CallControl::Control::READY;
                        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    }
                }
            }
            break;
            case CallControl::Control::BUSY: {
                context_->control = CallControl::Control::READY;
//                if (context_->messageData.field.recvBuffBusyCopy != nullptr) {
//                    delete [] context_->messageData.field.recvBuffBusyCopy;
//                    context_->messageData.field.recvBuffBusyCopy = nullptr;
//                }
            }
            break;
            case CallControl::Control::READY: {

                if (!context_->isIncomingCall) {
                    uint16_t assign = context_->inputBuff.shift();
                    uint8_t key = context_->getKey(assign);
                    context_->busyDynamicStorage.push_back(assign);
                    switchLed(key, true, 900, 100);
                }

                if (!context_->inputBuff.isEmpty()) {
                    sendRequest(context_->inputBuff.first(), CallControl::Request::LINK, TIMEOUT);
                } else {
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                }
            }
            break;
            case CallControl::Control::ANSWER: {
                context_->resetData();
                switchLed(context_->subjectKey.key, false);
                context_->TransitionTo(new CallWaiting);
            }
            break;
            case CallControl::Control::EXCH_CALL_TYPE: {
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->messageData.field = context_->messageDataBuff.field;
                std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));
                context_->resetData();
                context_->setCallType();
            }
            break;
            default:
                break;
            }
        }
    }
}

void CircularCall::handleUnknownIncomingCallBack()
{
    if (context_->osTimer.autoAnsw_timerStatus == osOK) {
        stopRingTone();
        context_->createRtp(context_->messageData.field.prevOwnId, CallControl::Simplex_recv_type);
    }
}

void CircularCall::endCircularCall()
{
    switchOffLeds(context_->busyDynamicStorage);
    context_->busyDynamicStorage.clear();

    switchOffLeds(context_->dynamicStorage);
    context_->dynamicStorage.clear();

    switchLed(context_->assignedData.key, false);
    context_->microphone.stop();
    context_->removeRtp();
    context_->resetData();
    context_->TransitionTo(new CallWaiting);

}

void CircularCall::hungUp()
{
    context_->microphone.stop();
    switchLed(context_->assignedData.key, false);
    context_->removeRtp();
    context_->control = CallControl::Control::HANG_UP;
    context_->sendRequest(CallControl::Request::HANG_UP);
    context_->requestCount++;
}

void CircularCall::switchOffLeds(std::vector <uint16_t> &v)
{
    uint8_t key = 0;
    if (!v.empty()) {
        for (auto &n : v) {
            key = context_->getKey(n);
            switchLed(key, false);
            osDelay(10);
        }
    }
}

void CircularCall::sendRequest(uint16_t distId, CallControl::Request reqType, uint16_t timeout)
{
    context_->requestCount = 0;
    context_->sendMessage(distId, CallControl::Circular, reqType);
    context_->requestCount++;
    context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, timeout);
}

void CircularCall::handleAck()
{
    switch (context_->control) {
    case CallControl::Control::HANG_UP: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                if (!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->inputBuff.first() == context_->messageData.field.distId) {
                context_->inputBuff.shift();
                if (!context_->inputBuff.isEmpty()) {
                    sendRequest(context_->inputBuff.first(), CallControl::Request::HANG_UP, TIMEOUT);
                    context_->requestCount = 0;
                    context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, TIMEOUT);
                    context_->requestCount++;
                } else {
                    context_->control = CallControl::Control::READY;
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                }
            }
        }
    }
    break;
    case CallControl::Control::EXCH_CALL_TYPE: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId
                && context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

            context_->messageData.field = context_->messageDataBuff.field;
            std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));

            context_->resetData();
            context_->setCallType();
        }
    }
    break;
    case CallControl::Control::BUSY: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            context_->control = CallControl::Control::READY;

            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
//            if (context_->messageData.field.recvBuffBusyCopy != nullptr) {
//                delete [] context_->messageData.field.recvBuffBusyCopy;
//                context_->messageData.field.recvBuffBusyCopy = nullptr;
//            }
        }
    }
    break;
    case CallControl::Control::ANSWER: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            context_->createRtp(context_->messageData.field.prevOwnId, context_->Duplex_type);
            switchLed(context_->subjectKey.key, true);
            context_->microphone.stop();
            context_->isAnsweredCall = true;
        }
    }
    break;
    default:

        break;
    }
}
