#include "telephonecall.h"
#include "callwaiting.h"
#include "../UDP_JSON/udp_multicast.h"
#include "rs232.h"
#include "callcontrol.h"

//extern osTimerId telephone_timerId_;
//extern osStatus  telephone_timerStatus_;
extern uint8_t ledIndicateAsterisk;
void TelephoneCall::handleButton()
{
term2("TelephoneCall::handleButton")
    if (context_->subjectKey.key == CallControl::Hash)
    {
        stopRingTone();
        context_->microphone.stop();
        context_->ordinaryTelephoneCall = false;
        context_->simplexTelephoneCall = false;
//switchLed(context_->assignedData.key, false);

        if (!context_->telephoneDynamicStorage.empty())
            context_->telephoneDynamicStorage.clear();

        if (context_->control == CallControl::Control::NONE) {
            context_->removeRtp();
            context_->control = CallControl::Control::HANG_UP;
            context_->sendRequest(CallControl::Request::HANG_UP);

        } else {
            context_->resetData();
            this->context_->TransitionTo(new CallWaiting);
        }

    }
    else if (context_->subjectKey.key == CallControl::Asterisk)
    {
        if (!context_->isAnsweredCall && context_->isIncomingCall)
        {
            stopRingTone();
            context_->isAnsweredCall = true;
            context_->sendRequest(CallControl::Request::ACK_ANSW);

        } else if (context_->subjectKey.mode == NotFixed) {
            context_->microphone.start();
        }
    }
    else
    {
        //старый тлф вызов
        if ((context_->rtpStatus != OK_RTP) && context_->ordinaryTelephoneCall)
            for (auto& var : context_->keypadStructArray)
                if (context_->subjectKey.key == var.n)
                {
                    if (context_->telephoneDynamicStorage.size() < 3)
                    {
                        context_->telephoneDynamicStorage.push_back(var.i);
                        switchLed(context_->subjectKey.key, true, 0,0,0, GPIO::GREEN);
                        context_->osTimer.start(context_->osTimer.telephone_timerId, context_->osTimer.telephone_timerStatus, DIALING_TIMEOUT);
                        startDtmfTone(var.i);
                        break;
                    }
                }
        //Добавил - симплекс тлф
        if ((context_->rtpStatus != OK_RTP) && context_->simplexTelephoneCall)
            for (auto& var : context_->keypadStructArray)
                if (context_->subjectKey.key == var.n)
                {
                    if (context_->telephoneDynamicStorage.size() < 3)
                    {
                        context_->telephoneDynamicStorage.push_back(var.i);
                        context_->osTimer.start(context_->osTimer.telephone_timerId, context_->osTimer.telephone_timerStatus, DIALING_TIMEOUT);
                        startDtmfTone(var.i);
                        break;
                    }
                }
    }
}

void TelephoneCall::handleJsonMessage()
{
    switch (static_cast<CallControl::Request>(context_->messageData.field.linkData)) {
    case CallControl::Request::HANG_UP:
        if (ThisStation_.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {

                stopRingTone();
                switchLed(ledIndicateAsterisk, false); //Здесь может быть индикация пропущенного вызова

                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

                context_->sendRequest(CallControl::Request::ACK);


                if (context_->rtpStatus == FAIL_RTP) {
                    context_->missedCall.add(context_->assignedData.key);
                } else {
                    context_->removeRtp();
                    switchLed(context_->assignedData.key, false);
                    context_->microphone.stop();
                }

                context_->resetData();

                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (ThisStation_.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                stopRingTone();
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
        if (ThisStation_.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId != context_->messageData.field.prevOwnId) {
//                if ((context_->assignedData.priority > context_->messageData.field.priority || context_->assignedData.priority == 0) &&
//                        (context_->messageData.field.prevPriority > context_->messageData.field.priority || context_->messageData.field.prevPriority == 0)) {

                if ((context_->isIncomingCall && (context_->messageData.field.prevPriority > context_->messageData.field.priority)) ||
                        (!context_->isIncomingCall && (context_->assignedData.priority > context_->messageData.field.priority))) {

                    context_->osTimer.stop(context_->osTimer.telephone_timerId, context_->osTimer.telephone_timerStatus);

                    stopRingTone();
                    switchLed(context_->assignedData.key, false);
                    context_->microphone.stop();

                    if (context_->telephoneDynamicStorage.empty()) {
                        context_->control = CallControl::Control::EXCH_CALL_TYPE;
                        context_->messageDataBuff.field = context_->messageData.field;                  //it is copying incoming json to the buffer
                        context_->copyRecvBuff(context_->messageDataBuff.recvMessageBuff, RecvBuff_);

                        context_->sendRequest(CallControl::Request::HANG_UP);
                        context_->removeRtp();
                    } else {
                        context_->telephoneDynamicStorage.clear();
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
        if (ThisStation_.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                stopRingTone();
                context_->sendRequest(CallControl::Request::ACK);
                context_->createRtp(ThisStation_.id, CallControl::Duplex_type);
                switchLed(context_->assignedData.key, true, 300, 300, 2, GPIO::GREEN);
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        }
        break;
    case CallControl::Request::BUSY:
        if (ThisStation_.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->sendRequest(CallControl::Request::ACK);
                startRingTone(RingToneType::RING_BACK_BUSY_TONE);
                switchLed(context_->assignedData.key, true, 900, 100, 0, GPIO::GREEN);
//                context_->serviceData->isBusy = true;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        }
        break;
    default:
        break;
    }
}

void TelephoneCall::handleLostLink()
{
    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
    context_->microphone.stop();
    context_->removeRtp();
    switchLed(context_->assignedData.key, false);
    context_->resetData();
    if(!context_->switchToConf())
        context_->TransitionTo(new CallWaiting);
}

void TelephoneCall::handleRepeatedRequestCallBack()
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
                if (!context_->isIncomingCall)
                {
                    startRingTone(RingToneType::RING_BACK_BUSY_TONE);
                    switchLed(context_->assignedData.key, true, 900, 100, 0, GPIO::GREEN);
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
                std::memcpy(RecvBuff_, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));
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

void TelephoneCall::handleUnknownIncomingCallBack()
{

}

void TelephoneCall::handleAck()
{
    switch (context_->control) {
    case CallControl::Control::READY: {
        if (ThisStation_.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->control = CallControl::Control::NONE;
                context_->copyRecvBuff(context_->messageData.recvMessageBuff, RecvBuff_);
//                switchLed(context_->assignedData.key, true, 0,0,0, GPIO::GREEN);
//                context_->messageData.field.prevPriority = context_->messageData.field.distPriority;
                startRingTone(RingToneType::RING_BACK_TONE);
//                context_->requestCount = 0;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
            }
        }
    }
    break;
    case CallControl::Control::HANG_UP: {
        if (ThisStation_.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->control = CallControl::Control::NONE;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        } else if (ThisStation_.id == context_->messageData.field.ownId) {
            if (context_->messageData.field.distId == context_->messageData.field.prevDistId) {
                context_->control = CallControl::Control::NONE;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->resetData();
                if(!context_->switchToConf())
                    this->context_->TransitionTo(new CallWaiting);
            }
        }
    }
    break;
    case CallControl::Control::EXCH_CALL_TYPE: {
        if ((ThisStation_.id == context_->messageData.field.ownId && context_->messageData.field.distId == context_->messageData.field.prevDistId)
                || (ThisStation_.id == context_->messageData.field.distId && context_->messageData.field.ownId == context_->messageData.field.prevOwnId)) {

            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);

            context_->messageData.field = context_->messageDataBuff.field;
            std::memcpy(RecvBuff_, context_->messageDataBuff.recvMessageBuff, std::strlen(context_->messageDataBuff.recvMessageBuff));
            context_->control = CallControl::Control::NONE;

            context_->resetData();
            context_->setCallType();
        }
    }
    break;
    case CallControl::Control::BUSY: {
        if (ThisStation_.id == context_->messageData.field.distId) {
            context_->control = CallControl::Control::NONE;
            context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
//            if (context_->serviceData->recvBuffBusyCopy != nullptr) {
//                delete [] context_->serviceData->recvBuffBusyCopy;
//                context_->serviceData->recvBuffBusyCopy = nullptr;
//            }
        }
    }
    break;
    case CallControl::Control::ANSWER: {
        if (ThisStation_.id == context_->messageData.field.distId) {
            if (context_->messageData.field.ownId == context_->messageData.field.prevOwnId) {
                context_->control = CallControl::Control::NONE;
                context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
                context_->createRtp(context_->messageData.field.prevOwnId, context_->Duplex_type);
                switchLed(context_->assignedData.key, true, 0,0,0, GPIO::GREEN);
                context_->isAnsweredCall = true;
            }
        }
    }
    break;
    default:
        break;
    }
}
