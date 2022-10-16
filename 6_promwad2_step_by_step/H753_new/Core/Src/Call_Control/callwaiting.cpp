#include "callwaiting.h"
#include "../UDP_JSON/udp_multicast.h"
#include "duplexdirectcall.h"
#include "simplexdirectcall.h"
#include "groupcall.h"
#include "circularcall.h"
#include "conferencecall.h"
#include "telephonecall.h"
#include "rs232.h"
extern SAI_HandleTypeDef audioTxSai;
extern uint16_t lastDirectSubject;
extern int volatile asteriskPressed;
bool asteriksRecall = false;

void CallWaiting::handleButton()
{
term2("CallWaiting::handleButton")
    context_->assignedData.key = context_->subjectKey.key;
    context_->assignedData.priority = context_->subjectKey.priority;

    context_->messageData.field.prevOwnId = 0;
    context_->messageData.field.prevPriority = 0;


    switch (context_->subjectKey.function)
    {

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


        context_->createRtp(ThisStation_.id, context_->Simplex_send_type);

        context_->TransitionTo(new GroupCall);
    }
    break;
    case CallControl::Circular: {
        context_->messageData.field.prevOwnId = 0;
        context_->microphone.start();
        switchLed(context_->subjectKey.key, true, 0,0,0, GPIO::GREEN);
//        context_->createRtp(ThisStation_.id, context_->Simplex_send_type);
        context_->TransitionTo(new CircularCall);
    }
    break;
    case CallControl::Conference: {

        context_->messageData.field.prevOwnId = 0;
        context_->assignedData.conferenceKey = context_->subjectKey.key;
        context_->assignedData.confPriority = context_->subjectKey.priority;
        switchLed(context_->subjectKey.key, true, 0,0,0, GPIO::GREEN);
        context_->microphone.start();
//        context_->createRtp(ThisStation_.id, context_->Duplex_type);
        context_->TransitionTo(new ConferenceCall);
    }
    break;

    case CallControl::Telephone:
    {
            //Костыль
            if (context_->subjectKey.key == CallControl::Hash)
            {
                if (SAI::getInstance()->tone.status == DTMF::Status::START)
                {
                    HAL_SAI_DMAStop(&audioTxSai);
                    SAI::getInstance()->tone.status = DTMF::Status::IDLE;
                }
            }

        if ((context_->subjectKey.key == CallControl::Asterisk)
            && (context_->telephoneDynamicStorage.size() == 0))
        {//Когда первым следует Астериск то режим обычного телефонного вызова
         //Или повторного вызова - тут нужно отслеживать длительность удержания *

osDelay(700);

            if (asteriskPressed < 18)
            {
term2("ordinaryTelephoneCall")
                context_->ordinaryTelephoneCall = true;
                context_->assignedData.key = context_->subjectKey.key;
                context_->assignedData.priority = context_->subjectKey.priority;
                //            context_->messageData.field.prevPriority = 4;
                switchLed(context_->subjectKey.key, true, 0,0,0, GPIO::GREEN );
                context_->TransitionTo(new TelephoneCall);

            }
            else
            {
 term2("recall")
                asteriskPressed = 0;
                asteriksRecall = true;
                context_->TransitionTo(new SimplexDirectCall);
                int recallSubject = 103;
//Здесь код прямого вызова Simplex
//                const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
//                DynamicJsonDocument doc (capacity);
//                doc["Own_Id"] = ThisStation_.id;
//                doc["Dist_Id"].add(recallSubject);
//                doc["Call_Type"] = CallControl_->Direct;
//                doc["Priority"] = CallControl_->assignedData.priority;
//                doc["Link_Data"] = 0xFF;
//                doc["Direct_Link_Mode"] = 1;
//                //            CallControl_->sendJson(doc, capacity);

//                context_->requestCount = 0;
//                std::fill(context_->messageData.txBuff, context_->messageData.txBuff + context_->messageData.txBuffSize, 0);
//                if (serializeJson(doc, context_->messageData.txBuff, capacity) > 0) {
//                    sendUdpMulticast(context_->messageData.txBuff, strlen(context_->messageData.txBuff));
//                }
////                context_->requestCount++;

//                //            CallControl_->copyRecvBuff(CallControl_->serviceData->recvBuffCopy,
//                //                    CallControl_->outputJsonBuff);
//                context_->osTimer.start(context_->osTimer.request_timerId,
//                                            context_->osTimer.request_timerStatus,
//                                            200);
//                //Находим есть ли в конфиге абонент subjectDirectTelephoneCall
//                uint8_t key = context_->getKey(recallSubject);
//                switchLed(key, true, 0, 0, 0, GPIO::GREEN);
//                context_->TransitionTo(new SimplexDirectCall);
            }


        }

//Здесь код обработки клавиш Контекст переключаю сразу но взвожу другой флаг
        if ((context_->subjectKey.key != CallControl::Asterisk)
            && (context_->subjectKey.key != CallControl::Hash)
            && (context_->telephoneDynamicStorage.size() == 0))
        {//Нажата одна из цифровых клавиш
term2("Keypad pressed")
            context_->simplexTelephoneCall = true;
            context_->TransitionTo(new TelephoneCall);
            if ((context_->rtpStatus != OK_RTP) && context_->simplexTelephoneCall)
                for (auto& var : context_->keypadStructArray)
                    if (context_->subjectKey.key == var.n)
                    {
                        if (context_->telephoneDynamicStorage.size() < 3)
                        {
                            context_->telephoneDynamicStorage.push_back(var.i);
                            context_->osTimer.start(context_->osTimer.telephone_timerId, context_->osTimer.telephone_timerStatus, TelephoneCall::DIALING_TIMEOUT);
                            startDtmfTone(var.i);
                            break;
                        }
                    }
        }


    }// end case CallControl::Telephone
    break;

    default:
        break;
    }
}

void CallWaiting::handleJsonMessage()
{
    if (ThisStation_.id == context_->messageData.field.distId)
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

