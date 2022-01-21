#include "groupcall.h"
#include "callwaiting.h"
#include "../UDP_JSON/udp_multicast.h"

void GroupCall::handleButton()
{
    if (context_->subjectKey.key == context_->assignedData.key) {

        stopRingTone();
        context_->microphone.stop();

//        if (!context_->isIncomingCall) {
//            stopRingBackTone();
//            context_->control = CallControl::Control::HANG_UP;
//            context_->sendRequest(CallControl::Group, CallControl::Request::HANG_UP, TIMEOUT);
//        } else {
        context_->removeRtp();
        switchLed(context_->assignedData.key, false);
//        context_->isIncomingCall = false;
        context_->resetData();
        if(!context_->switchToConf())
            this->context_->TransitionTo(new CallWaiting);
//        }
    }
}

void GroupCall::handleJsonMessage()
{
    switch (static_cast<CallControl::Request>(context_->messageData.field.linkData)) {
    case CallControl::Request::HANG_UP:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

//                context_->retransmitMessage(context_->serviceData->recvBuffCopy, strlen(context_->serviceData->recvBuffCopy), context_->messageData.field.prevDistId, CallControl::Request::ACK);
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), context_->messageData.field.prevDistId, CallControl::Request::ACK);

                context_->removeRtp();
                switchLed(context_->assignedData.key, false);
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {

        }
        break;
    case CallControl::Request::LINK:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId != context_->messageData.field.prevOwnId) {
                if ((context_->assignedData.priority > context_->messageData.field.priority || context_->assignedData.priority == 0) &&
                        (context_->messageData.field.prevPriority > context_->messageData.field.priority || context_->messageData.field.prevPriority == 0)) {

                    context_->microphone.stop();
                    context_->removeRtp();
                    switchLed(context_->assignedData.key, false);

//                    if (!context_->isIncomingCall) {

//                        if (context_->control != CallControl::Control::HANG_UP &&
//                                context_->control != CallControl::Control::EXCH_CALL_TYPE)
//                            context_->sendRequest(CallControl::Group, CallControl::Request::HANG_UP, TIMEOUT);

//                        context_->control = CallControl::Control::EXCH_CALL_TYPE;
//                        *(context_->serviceBuff) = *(context_->serviceData);                  //it is copying incoming json to the buffer
//                        context_->serviceBuff->sizeRecvCopyBuff = strlen(UdpJsonExch::getInstance()->recvBuff)+1;
//                        context_->serviceBuff->recvBuffCopy = new char [context_->serviceBuff->sizeRecvCopyBuff];
//                        memcpy(context_->serviceBuff->recvBuffCopy, UdpJsonExch::getInstance()->recvBuff, context_->serviceBuff->sizeRecvCopyBuff);

//                    } else {
                    context_->resetData();
                    context_->setCallType();
//                    }


                } else  {
                    uint8_t key = context_->getKey(context_->messageData.field.ownId);
                    if (key) {
                        context_->missedCall.add(key);
                    }

                    context_->sendRequest(CallControl::Request::BUSY);

//                    context_->control = CallControl::Control::BUSY;
//                    context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), CallControl::Request::BUSY);
//                    context_->osTimer.start(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus, TIMEOUT);
                }
            } else {
                context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), context_->messageData.field.distId, CallControl::Request::ACK);
            }
        }
        break;
    case CallControl::Request::ACK:
        handleAck();
        break;
    case CallControl::Request::ACK_ANSW:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {

//            stopRingBackTone();
//            context_->createRtp(Json::getInstance()->thisStation.id, CallControl::Duplex_type);
//            context_->microphone.start();
//            context_->requestCount = 0;
//            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
        }
        break;
    case CallControl::Request::BUSY:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            context_->isAnswerToRequest = true;
            context_->retransmitMessage(UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff), context_->messageData.field.distId, CallControl::Request::ACK);
        }
        break;
    default:
        break;
    }
}

void GroupCall::handleLostLink()
{
    if (context_->isIncomingCall) {
        context_->microphone.stop();
        context_->removeRtp();
        switchLed(context_->assignedData.key, false);
        context_->resetData();
        if(!context_->switchToConf())
            context_->TransitionTo(new CallWaiting);
    }
}

void GroupCall::handleRepeatedRequestCallBack()
{
    if ((context_->requestCount < 1 && !context_->isAnswerToRequest) && context_->checkGroupIndex != 0) {

        sendUdpMulticast(context_->messageData.txBuff, strlen(context_->messageData.txBuff));
        context_->requestCount++;

    } else {
        uint16_t sizeBuff = Json::getInstance()->thisStation.groupsBuffer[context_->subjectKey.assign].stSize;
        if (context_->checkGroupIndex < sizeBuff) {
            uint16_t groupDistId = Json::getInstance()->thisStation.groupsBuffer[context_->assignedData.id].stantions[context_->checkGroupIndex];
            context_->sendMessage(groupDistId, CallControl::Group, CallControl::Request::LINK);

            context_->requestCount = 0;
            context_->isAnswerToRequest = false;
            context_->checkGroupIndex++;
        } else {
            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

            if (!subscriberNum) {
                if (!context_->isIncomingCall) {
                    startRingTone(RingToneType::RING_BACK_BUSY_TONE);
                    switchLed(context_->assignedData.key, true, 900, 100);
                }
            }

        }
    }
}

void GroupCall::handleUnknownIncomingCallBack()
{
    if (context_->osTimer.autoAnsw_timerStatus == osOK) {
        stopRingTone();
        context_->createRtp(context_->messageData.field.prevOwnId, CallControl::Simplex_recv_type);
    }
}

void GroupCall::handleAck()
{
    switch (context_->control) {
    case CallControl::Control::READY: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            context_->isAnswerToRequest = true;
            subscriberNum++;
//                if (context_->rtpStatus != OK_RTP) {
            context_->microphone.start();
            switchLed(context_->assignedData.key, true);
        }
//                }
    }
    break;
//        case CallControl::Control::HANG_UP: {
//            if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId)
//                context_->serviceData->isAnswerToRequest = true;
//        }
//        break;
    case CallControl::Control::BUSY: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            context_->control = CallControl::Control::NONE;
            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
//            if (context_->serviceData->recvBuffBusyCopy != nullptr) {
//                delete [] context_->serviceData->recvBuffBusyCopy;
//                context_->serviceData->recvBuffBusyCopy = nullptr;
//            }
        }
    }
    break;
    default:
        break;
    }
}
