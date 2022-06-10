#include "callwaiting.h"

#include "duplexdirectcall.h"
#include "simplexdirectcall.h"
#include "groupcall.h"
#include "circularcall.h"
#include "conferencecall.h"
#include "telephonecall.h"


void CallWaiting::handleButton()
{

    context_->assignedData.key = context_->subjectKey.key;
    context_->assignedData.priority = context_->subjectKey.priority;

    context_->messageData.field.prevOwnId = 0;
    context_->messageData.field.prevPriority = 0;


    switch (context_->subjectKey.function) {

    case CallControl::Direct: {

        context_->messageData.field.prevDistId = context_->subjectKey.assign;

        if (context_->subjectKey.directLinkMode == CallControl::Duplex)
            context_->TransitionTo(new DuplexDirectCall);
        else if (context_->subjectKey.directLinkMode == CallControl::Simplex)
            context_->TransitionTo(new SimplexDirectCall);

        context_->sendRequest(CallControl::Direct, CallControl::Request::LINK, TIMEOUT);
//        context_->sendMessage(CallControl::Request::LINK);

    }
    break;
    case CallControl::Group: {

        context_->assignedData.id = context_->subjectKey.assign;

        context_->sendRequest(CallControl::Group, CallControl::Request::LINK, TIMEOUT);


        context_->createRtp(Json::getInstance()->thisStation.id, context_->Simplex_send_type);

        context_->TransitionTo(new GroupCall);
    }
    break;
    case CallControl::Circular: {
        context_->messageData.field.prevOwnId = 0;
        context_->microphone.start();
        switchLed(context_->subjectKey.key, true);
//        context_->createRtp(Json::getInstance()->thisStation.id, context_->Simplex_send_type);
        context_->TransitionTo(new CircularCall);
    }
    break;
    case CallControl::Conference: {

        context_->messageData.field.prevOwnId = 0;
        context_->assignedData.conferenceKey = context_->subjectKey.key;
        context_->assignedData.confPriority = context_->subjectKey.priority;
        switchLed(context_->subjectKey.key, true);
        context_->microphone.start();
//        context_->createRtp(Json::getInstance()->thisStation.id, context_->Duplex_type);
        context_->TransitionTo(new ConferenceCall);
    }
    break;
#ifndef SC2BOARD
    case CallControl::Telephone: {
        if (context_->subjectKey.key == CallControl::Asterisk) {


            context_->assignedData.key = context_->subjectKey.key;
            context_->assignedData.priority = context_->subjectKey.priority;
//            context_->messageData.field.prevPriority = 4;
            switchLed(context_->subjectKey.key, true);
            context_->TransitionTo(new TelephoneCall);
        }
    }
    break;
#endif
    default:
        break;
    }
}

void CallWaiting::handleJsonMessage()
{
    if (Json::getInstance()->thisStation.id == context_->messageData.field.distId)
        context_->setCallType();
}

void CallWaiting::handleLostLink()
{

}

void CallWaiting::handleRepeatedRequestCallBack()
{
    context_->osTimer.stop(context_->osTimer.request_timerId, context_->osTimer.request_timerStatus);
}

void CallWaiting::handleUnknownIncomingCallBack()
{
    context_->osTimer.stop(context_->osTimer.autoAnsw_timerId, context_->osTimer.autoAnsw_timerStatus);
}

