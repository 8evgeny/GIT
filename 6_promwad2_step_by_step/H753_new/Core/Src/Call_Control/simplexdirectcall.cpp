#include "simplexdirectcall.h"
#include "callwaiting.h"
#include "../UDP_JSON/udp_multicast.h"
#include "conferencecall.h"

void SimplexDirectCall::handleButton()
{
    if (context_->subjectKey.key == context_->assignedData.key) {

        stopRingTone();
        context_->microphone.stop();
        switchLed(context_->assignedData.key, false);

        if (context_->control == CallControl::Control::NONE ||
                context_->control == CallControl::Control::READY) {
            context_->removeRtp();
            context_->control = CallControl::Control::HANG_UP;
            context_->sendRequest(CallControl::Request::HANG_UP);
        } else {
            context_->removeRtp();
            context_->resetData();
            if(!context_->switchToConf())
                this->context_->TransitionTo(new CallWaiting);
        }
    }

}

void SimplexDirectCall::handleJsonMessage()
{
    switch (static_cast<CallControl::Request>(context_->messageData.field.linkData)) {
    case CallControl::Request::HANG_UP:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {


                stopRingTone();      //This function is used to stop the DTMF tone of the unknown incoming call
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->osTimer.stop(context_->osTimer.autoAnsw_timerId, context_->osTimer.autoAnsw_timerStatus);

                context_->sendRequest(CallControl::Request::ACK);

                context_->removeRtp();
                switchLed(context_->assignedData.key, false);
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {

                context_->microphone.stop();
                context_->sendRequest(CallControl::Request::ACK);
                context_->removeRtp();
                switchLed(context_->assignedData.key, false);
                context_->resetData();
                this->context_->TransitionTo(new CallWaiting);
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
                    switchLed(context_->assignedData.key, false);

                    context_->osTimer.stop(context_->osTimer.autoAnsw_timerId, context_->osTimer.autoAnsw_timerStatus);

//                    if (context_->rtpStatus == OK_RTP) {
                    context_->control = CallControl::Control::EXCH_CALL_TYPE;
                    context_->messageDataBuff.field = context_->messageData.field;                  //it is copying incoming json to the buffer
                    context_->copyRecvBuff(context_->messageDataBuff.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);

                    context_->sendRequest(CallControl::Request::HANG_UP);
                    context_->removeRtp();
//                    } else {
//                    context_->resetData();
//                    context_->setCallType();

//                    }


                } else {
                    uint8_t key = context_->getKey(context_->messageData.field.ownId);
                    if (key) {
                        context_->missedCall.add(key);
                    }
                    context_->sendRequest(CallControl::Request::BUSY);
                }
            } else {
                context_->sendRequest(CallControl::Request::ACK);
//                context_->retransmitMessage(context_->serviceData->recvBuffCopy, strlen(context_->serviceData->recvBuffCopy), CallControl::Request::ACK_ANSW);
            }
        }
        break;
    case CallControl::Request::ACK: {
        handleAck();
    }
    break;
    case CallControl::Request::ACK_ANSW:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->control = CallControl::Control::NONE;
                context_->copyRecvBuff(context_->messageData.recvMessageBuff, UdpJsonExch::getInstance()->recvBuff);
                switchLed(context_->assignedData.key, true);
//                context_->messageData.field.prevPriority = context_->messageData.field.distPriority;
                context_->microphone.start();
                context_->createRtp(Json::getInstance()->thisStation.id, CallControl::Simplex_send_type);
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        }
        break;
    case CallControl::Request::BUSY:
        if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->control = CallControl::Control::BUSY;
                context_->sendRequest(CallControl::Request::ACK);
                startRingTone(RingToneType::RING_BACK_BUSY_TONE);
                switchLed(context_->assignedData.key, true, 900, 100);
//                context_->serviceData->isBusy = true;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        }
        break;
    default:
        break;
    }
}

void SimplexDirectCall::handleLostLink()
{
    if (context_->isIncomingCall) {
        context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
        context_->microphone.stop();
        context_->removeRtp();
        switchLed(context_->assignedData.key, false);
        context_->resetData();
        if(!context_->switchToConf())
            context_->TransitionTo(new CallWaiting);
    }
}

void SimplexDirectCall::handleRepeatedRequestCallBack()
{
    if (context_->osTimer.request_timerStatus == osOK) {

        if (context_->requestCount < 3) {
            sendUdpMulticast(context_->messageData.txBuff, strlen(context_->messageData.txBuff));
            context_->requestCount++;
        } else {

            switch (context_->control) {
            case CallControl::Control::HANG_UP: {
                context_->resetData();
                if(!context_->switchToConf())
                    context_->TransitionTo(new CallWaiting);
            }
            break;
            case CallControl::Control::BUSY: {
                context_->control = CallControl::Control::NONE;
//                if (context_->serviceData->recvBuffBusyCopy != nullptr) {
//                    delete [] context_->serviceData->recvBuffBusyCopy;
//                    context_->serviceData->recvBuffBusyCopy = nullptr;
//                }
            }
            break;
            case CallControl::Control::READY: {
                if (!context_->isIncomingCall) {
                    startRingTone(RingToneType::RING_BACK_BUSY_TONE);
                    switchLed(context_->assignedData.key, true, 900, 100);
                }
            }
            break;
            case CallControl::Control::ANSWER: {
                context_->resetData();
                switchLed(context_->assignedData.key, false);
                if(!context_->switchToConf())
                    context_->TransitionTo(new CallWaiting);
            }
            break;
            case CallControl::Control::EXCH_CALL_TYPE: {
                context_->messageData.field = context_->messageDataBuff.field;
                std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));
                context_->resetData();
                context_->setCallType();
            }
            break;
            default:
                break;
            }

            context_->osTimer.stop(context_->osTimer.request_timerId,
                                   context_->osTimer.request_timerStatus);
        }

    }
}

void SimplexDirectCall::handleUnknownIncomingCallBack()
{
    if (context_->osTimer.autoAnsw_timerStatus == osOK) {
        stopRingTone();
        context_->createRtp(context_->messageData.field.prevOwnId, CallControl::Simplex_recv_type);
    }
}

void SimplexDirectCall::handleAck()
{
    switch (context_->control) {

    case CallControl::Control::HANG_UP: {
        if (Json::getInstance()->thisStation.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->control = CallControl::Control::NONE;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (Json::getInstance()->thisStation.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->control = CallControl::Control::NONE;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                this->context_->TransitionTo(new CallWaiting);
            }
        }
    }
    break;
    case CallControl::Control::EXCH_CALL_TYPE: {
        if ((Json::getInstance()->thisStation.id == context_->messageData.field.ownId && context_->messageData.field.distId == context_->messageData.field.prevDistId)
                || (Json::getInstance()->thisStation.id == context_->messageData.field.distId && context_->messageData.field.ownId == context_->messageData.field.prevOwnId)) {
            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

            context_->messageData.field = context_->messageDataBuff.field;
            std::memcpy(UdpJsonExch::getInstance()->recvBuff, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));

            context_->control = CallControl::Control::NONE;
            context_->resetData();
            context_->setCallType();
        }
    }
    break;
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
