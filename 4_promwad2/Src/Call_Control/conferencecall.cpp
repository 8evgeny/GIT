#include "conferencecall.h"
#include "callwaiting.h"
#include "../UDP_JSON/udp_multicast.h"

void ConferenceCall::handleButton()
{
    if (context_->isIncomingCall && context_->subjectKey.key == context_->assignedData.conferenceKey) {

        if (!context_->isAnsweredCall) {
            stopRingTone();
            context_->sendRequest(CallControl::Request::ACK_ANSW);
            context_->isAnsweredCall = true;
        } else {
            hungUp();
        }

    } else if (context_->subjectKey.function == CallControl::HungUp) {

        if (!context_->isIncomingCall)  {
            endConferenceCall();
        } else {
            hungUp();
        }

    } else if (context_->subjectKey.key == context_->assignedData.key) {

        if (context_->subjectKey.mode == NotFixed) {
            context_->microphone.start();
        } else {
            endConferenceCall();
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


void ConferenceCall::handleJsonMessage()
{
    switch (static_cast<CallControl::Request>(context_->messageData.field.linkData)) {
    case CallControl::Request::HANG_UP:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

                stopRingTone();
                context_->sendRequest(CallControl::Request::ACK);

                if (context_->rtpStatus == FAIL_RTP) {
                    context_->missedCall.add(context_->assignedData.key);
                } else {
                    context_->removeRtp();
                    switchLed(context_->assignedData.conferenceKey, false);
                    context_->microphone.stop();
                }

                if (context_->isInterruptConf != CallControl::InterruptConf::CALLED_PARTY) {
                    context_->calledPartyConfDataBuff.clear();
                }
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->seekDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId)) {

//            memcpy(context_->serviceData->recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff) + 1);
//            context_->sendRequest(CallControl::Request::ACK);
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);

                context_->popDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId);

                if (context_->dynamicStorage.empty()) {

                    switchOffLeds(context_->busyDynamicStorage);
                    context_->busyDynamicStorage.clear();

                    context_->microphone.stop();
                    context_->removeRtp();
                    switchLed(context_->assignedData.conferenceKey, false);
                    context_->resetData();
                    if(!context_->switchToConf())
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

                    stopRingTone();
                    context_->microphone.stop();

                    context_->removeRtp();
                    switchLed(context_->assignedData.conferenceKey, false);

                    if (!context_->isIncomingCall) {

                        switchOffLeds(context_->busyDynamicStorage);
                        switchOffLeds(context_->dynamicStorage);

//                        *(context_->serviceBuff) = *(context_->serviceData);
                        context_->isInterruptConf = CallControl::InterruptConf::CALLER;

                        context_->resetData();
                        context_->setCallType();
                    } else {

                        if (context_->control != CallControl::Control::CHANGE_CONF) {
//                        *(context_->serviceBuff) = *(context_->serviceData);
//                        context_->messageDataBuff
                            context_->messageDataBuff.field = context_->messageData.field;
                            context_->copyRecvBuff(context_->messageDataBuff.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);


                            context_->control = CallControl::Control::CHANGE_CONF;
                            context_->sendRequest(CallControl::Request::CHANGE_CONF);
                        }
                    }

                } else {
                    uint8_t key = context_->getKey(context_->messageData.field.ownId);
                    if (key) {
                        context_->missedCall.add(key);
                    }
                    context_->sendRequest(CallControl::Request::BUSY);
                }
            } else {
                if (!context_->isAnsweredCall) {
                    context_->sendRequest(CallControl::Request::ACK);

                } else {
                    context_->sendRequest(CallControl::Request::ACK_ANSW);

                }
            }
        }
        break;

    case CallControl::Request::ACK: {
        handleAck();
    }
    break;
    case CallControl::Request::ACK_ANSW:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
//            if (context_->seekDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId)) {
            bool isFound = false;
            uint8_t key = 0;

            if (context_->rtpStatus != OK_RTP)
                context_->createRtp(Json::getInstance()->thisStation.id, context_->Duplex_type);

            if (!context_->dynamicStorage.empty()) {
                for (auto &n : context_->dynamicStorage) {
                    if (n == context_->messageData.field.distId)
                        isFound = true;
                }
            }

            if (!isFound) {
                context_->dynamicStorage.push_back(context_->messageData.field.distId);
            }

            key = context_->getKey(context_->messageData.field.distId);
            switchLed(key, true, 300, 300, 2);

//            memcpy(context_->serviceData->recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff) + 1);
//            context_->sendRequest(CallControl::Request::ACK);
            context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);

//            }
        }
        break;
    case CallControl::Request::BUSY:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->inputBuff.first()) {
                uint16_t assign = context_->inputBuff.shift();
                uint8_t key = context_->getKey(assign);
                context_->busyDynamicStorage.push_back(assign);
                switchLed(key, true, 900, 100);
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
            }
        }
        break;
    case CallControl::Request::CHANGE_CONF:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->isInterruptConf == CallControl::InterruptConf::NONE) {
                if (context_->seekDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId)) {
                    context_->popDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId);
                    uint8_t key = context_->getKey(context_->messageData.field.distId);
                    context_->busyDynamicStorage.push_back(context_->messageData.field.distId);
                    switchLed(key, true, 900, 100);
                    context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
                }
            }
        }
        break;
    case CallControl::Request::RETURN_CONF:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->seekDynamicStorage(context_->busyDynamicStorage, context_->messageData.field.distId)) {
                context_->popDynamicStorage(context_->busyDynamicStorage, context_->messageData.field.distId);
                if ( context_->pushDynamicStorage(CallControl::Conference, context_->dynamicStorage, context_->messageData.field.distId)) {
                    context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
                }
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::ACK);
            }
        }
        break;
//    default:
//        break;
    }
}

void ConferenceCall::handleLostLink()
{
    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
    context_->microphone.stop();
    context_->removeRtp();
    switchLed(context_->assignedData.conferenceKey, false);

    if (!context_->isIncomingCall) {

        switchOffLeds(context_->busyDynamicStorage);
        context_->busyDynamicStorage.clear();

        switchOffLeds(context_->dynamicStorage);
        context_->dynamicStorage.clear();

    }

    context_->resetData();
    context_->TransitionTo(new CallWaiting);
}

void ConferenceCall::handleRepeatedRequestCallBack()
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
                    if(!context_->switchToConf())
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
            case CallControl::Control::CONF_HANG_UP: {
                context_->popDynamicStorage(context_->dynamicStorage, *(context_->dynamicStorageIter));
                if ((context_->dynamicStorageIter) != context_->dynamicStorage.end()) {
                    confHungUpReq();
                } else {
                    context_->dynamicStorageIter = context_->dynamicStorage.begin();
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    context_->resetData();
                    this->context_->TransitionTo(new CallWaiting);
                }
            }

            break;
            case CallControl::Control::BUSY: {
                context_->control = CallControl::Control::READY;
//                if (context_->serviceData->recvBuffBusyCopy != nullptr) {
//                    delete [] context_->serviceData->recvBuffBusyCopy;
//                    context_->serviceData->recvBuffBusyCopy = nullptr;
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
                if(!context_->switchToConf())
                    context_->TransitionTo(new CallWaiting);
            }
            break;
            case CallControl::Control::CHANGE_CONF: {

                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);// added the timer stop function

                context_->messageData.field = context_->messageDataBuff.field;
                std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));

                context_->messageDataBuff.clear();

                context_->resetData();
                context_->setCallType();
            }
            break;
            case CallControl::Control::RETURN_CONF: {
                if (context_->isIncomingCall) {
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    context_->microphone.stop();
                    context_->removeRtp();
                    context_->resetData();
                    switchLed(context_->assignedData.conferenceKey, false);
                    if(!context_->switchToConf())
                        context_->TransitionTo(new CallWaiting);
                } else {
                    context_->popDynamicStorage(context_->dynamicStorage, *context_->dynamicStorageIter);
                    if ((context_->dynamicStorageIter) != context_->dynamicStorage.end()) {
                        sendRequest(*context_->dynamicStorageIter, CallControl::Request::RETURN_CONF, TIMEOUT);
                    } else {
                        context_->control = CallControl::Control::READY;
                        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

                        if (context_->dynamicStorage.empty()) {

                            switchOffLeds(context_->busyDynamicStorage);
                            context_->busyDynamicStorage.clear();

                            context_->microphone.stop();
                            context_->removeRtp();
                            switchLed(context_->assignedData.conferenceKey, false);
                            context_->resetData();
                            this->context_->TransitionTo(new CallWaiting);
                        }
                    }
                }
            }
            break;
            default:
                break;
            }



        }
    }
}

void ConferenceCall::handleUnknownIncomingCallBack()
{

}

void ConferenceCall::endConferenceCall()
{
    switchOffLeds(context_->busyDynamicStorage);
    context_->busyDynamicStorage.clear();
    switchLed(context_->assignedData.conferenceKey, false);
    context_->microphone.stop();
    context_->removeRtp();

    if (!context_->dynamicStorage.empty()) {
//        context_->subjectKey.function = CallControl::Conference;
        context_->control = CallControl::Control::CONF_HANG_UP;
        context_->dynamicStorageIter = context_->dynamicStorage.begin();
        sendRequest(*context_->dynamicStorageIter, CallControl::Request::HANG_UP, TIMEOUT);
    } else {
        context_->resetData();
        if(!context_->switchToConf())
            context_->TransitionTo(new CallWaiting);
    }
}

void ConferenceCall::hungUp()
{
    stopRingTone();
    context_->microphone.stop();
    switchLed(context_->assignedData.conferenceKey, false);
    context_->removeRtp();
    if (context_->isInterruptConf != CallControl::InterruptConf::CALLED_PARTY) {
        context_->calledPartyConfDataBuff.clear();
    }

    context_->control = CallControl::Control::HANG_UP;
    context_->sendRequest(CallControl::Request::HANG_UP);
}

void ConferenceCall::confHungUpReq()
{
    context_->requestCount = 0;
    context_->sendMessage(*context_->dynamicStorageIter, CallControl::Conference, CallControl::Request::HANG_UP);
    context_->requestCount++;
}

void ConferenceCall::sendRequest(uint16_t distId, CallControl::Request reqType, uint16_t timeout)
{
    context_->requestCount = 0;
    context_->sendMessage(distId, CallControl::Conference, reqType);
    context_->requestCount++;
    context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, timeout);
}

void ConferenceCall::switchOffLeds(std::vector <uint16_t> &v)
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

void ConferenceCall::handleAck()
{
    switch (context_->control) {
    case CallControl::Control::READY: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (!context_->inputBuff.isEmpty())
                if (context_->messageData.field.distId == context_->inputBuff.first()) {

                    context_->pushDynamicStorage(CallControl::Conference, context_->dynamicStorage, context_->inputBuff.shift());

                    if (!context_->inputBuff.isEmpty()) {
                        sendRequest(context_->inputBuff.first(), CallControl::Request::LINK, TIMEOUT);
                        context_->requestCount = 0;
                        context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, TIMEOUT);
                        context_->requestCount++;
                    } else {
                        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                    }
                }
        }
    }
    break;
    case CallControl::Control::HANG_UP: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                if(!context_->switchToConf())
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
    case CallControl::Control::CONF_HANG_UP: {
        if (*context_->dynamicStorageIter == context_->messageData.field.distId) {

            context_->popDynamicStorage(context_->dynamicStorage, *(context_->dynamicStorageIter));
            if ((context_->dynamicStorageIter) != context_->dynamicStorage.end()) {
                confHungUpReq();
                context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, TIMEOUT);
            } else {
                context_->dynamicStorageIter = context_->dynamicStorage.begin();
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                this->context_->TransitionTo(new CallWaiting);
            }

        }
    }
    break;
    case CallControl::Control::BUSY: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            context_->control = CallControl::Control::READY;

            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
//            if (context_->serviceData->recvBuffBusyCopy != nullptr) {
//                delete [] context_->serviceData->recvBuffBusyCopy;
//                context_->serviceData->recvBuffBusyCopy = nullptr;
//            }
        }
    }
    break;
    case CallControl::Control::ANSWER: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
//                context_->control = Control::None;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->createRtp(context_->messageData.field.prevOwnId, context_->Duplex_type);
                switchLed(context_->subjectKey.key, true);
                context_->microphone.start();
                context_->isAnsweredCall = true;

                if (context_->isInterruptConf == CallControl::InterruptConf::NONE) {
                    context_->calledPartyConfDataBuff.field = context_->messageData.field;
                    context_->copyRecvBuff(context_->calledPartyConfDataBuff.recvMessageBuff, context_->messageData.recvMessageBuff);
                }
            }
        }
    }
    break;
    case CallControl::Control::CHANGE_CONF: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->isInterruptConf = CallControl::InterruptConf::CALLED_PARTY;

//                *(context_->serviceData) = *(context_->serviceBuff);

                context_->messageData.field = context_->messageDataBuff.field;
                std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));

                context_->messageDataBuff.clear();
//                if (context_->messageDataBuff.recvMessageBuff != nullptr) {
//                    delete [] context_->messageDataBuff.recvMessageBuff;
//                    context_->messageDataBuff.recvMessageBuff = nullptr;
//                }

//                context_->serviceData->incomingCallType = 1;

                context_->resetData();
                context_->setCallType();
            }
        } /*else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
context_->isInterruptConf = CallControl::Caller;
}*/


    }
    break;
    case CallControl::Control::RETURN_CONF: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->seekDynamicStorage(context_->dynamicStorage, context_->messageData.field.distId)) {

                context_->dynamicStorageIter++;
                if ((context_->dynamicStorageIter) != context_->dynamicStorage.end()) {
                    sendRequest(*context_->dynamicStorageIter, CallControl::Request::RETURN_CONF, TIMEOUT);
                } else {
                    context_->control = CallControl::Control::READY;
                    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                }
            }
        }
    }
    break;
    default:

        break;
    }
}
