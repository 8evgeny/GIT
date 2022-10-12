#include "os_timers.h"
#include "rs232.h"
#include "../UDP_JSON/udp_multicast.h"
#include "callwaiting.h"
#include "conferencecall.h"
#include "simplexdirectcall.h"

extern uint16_t pressedKey; /*!< The variable stores a number of the pressed key */
extern uint8_t handleClick_count; /*!< The variable is the counter to handle clicks */

extern uint8_t keyMode, /*!< The variable stores a mode of the pressed key */
       func;    /*!< The variable stores a function of the pressed key */

//constexpr static uint16_t TIMEOUT {200};

osTimerDef (handleRelasedButtonTimer, handleRelasedButtonTimer_Callback);   /*!< handleRelasedButtonTimer definition */
osTimerDef (dialingTimer, dialingTimer_Callback);                           /*!< dialingTimer definition */
osTimerDef (autoAnswTimer, autoAnswTimer_Callback);                         /*!< autoAnswTimer definition */
osTimerDef (requestTimer, requestTimer_Callback);                           /*!< requestTimer definition */

OSTimer::OSTimer()
{
    if ((button_timerId = osTimerCreate (osTimer(handleRelasedButtonTimer), osTimerPeriodic, nullptr)) == nullptr) {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((telephone_timerId = osTimerCreate (osTimer(dialingTimer), osTimerPeriodic, nullptr)) == nullptr) {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((autoAnsw_timerId = osTimerCreate (osTimer(autoAnswTimer), osTimerPeriodic, nullptr)) == nullptr) {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((request_timerId = osTimerCreate (osTimer(requestTimer), osTimerPeriodic, nullptr)) == nullptr) {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

term("OSTimer::OSTimer()")
}

osStatus OSTimer::start(osTimerId id, osStatus& status, const uint16_t timerDelay)
{
//term1("OSTimer::start Delay") term((uint8_t)timerDelay)
    status = osTimerStart (id, timerDelay);     // timer starting
    if (status != osOK)  {
        RS232::getInstance().term << "Error OSTimer::start  " << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    return status;
}

osStatus OSTimer::stop(osTimerId id, osStatus& status)
{
    if (status == osOK) {
        status = osTimerStop (id);            // timer stopping
        if (status != osOK)  {
            RS232::getInstance().term << "osTimerStop Error " <<__FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
        status = osErrorOS;
    }
    return status;
}

#ifdef __cplusplus
extern "C" {
#endif

void handleRelasedButtonTimer_Callback(void const *arg)
{
    (void)arg;

    osMutexWait(MutexCallControl_, osWaitForever);
    if (!CallControl_->isIncomingCall || CallControl_->assignedData.key == CallControl::Asterisk)
    {
        if (CallControl_->subjectKey.key == CallControl_->assignedData.key)
        {
            if (keyMode == NotFixed)
            {
                switch (func) {
                case CallControl::Direct: {
                    if (!CallControl_->missedCall.isMissedKey)
                        CallControl_->handleNotFixedButton();
                }
                break;
                case CallControl::Group:
                    CallControl_->handleNotFixedButton();
                    break;
                case CallControl::Circular:
                    CallControl_->microphone.stop();
                    break;
                case CallControl::Conference:
                    CallControl_->microphone.stop();
                    break;
                case CallControl::Telephone:
                    CallControl_->microphone.stop();

                    break;
                default:
                    break;
                }

            }
        } else if (CallControl_->assignedData.key == CallControl::Asterisk)
        {
            if (CallControl_->subjectKey.key != CallControl::Asterisk
                    && CallControl_->subjectKey.key != CallControl::Hash)
                switchLed(CallControl_->subjectKey.key, false);
            stopDtmfTone();
        }

    }

    //Начальный набор номера на кейпаде
    if (CallControl_->simplexTelephoneCall)
    {
            stopDtmfTone();
    }

    CallControl_->osTimer.stop( CallControl_->osTimer.button_timerId,
            CallControl_->osTimer.button_timerStatus);

    osMutexRelease(MutexCallControl_);

    handleClick_count = 0;
    pressedKey = 0;
}

void dialingTimer_Callback(void const *arg)
{
    (void)arg;

    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    osMutexWait(MutexCallControl_, osWaitForever);

    uint8_t size = static_cast<uint8_t>(CallControl_->telephoneDynamicStorage.size());
    uint16_t distSubject = 0;
    char msg[100];
    //old telefone call
    if (size > 2 && CallControl_->ordinaryTelephoneCall)
    {
        CallControl_->ordinaryTelephoneCall = false;
        for (uint8_t i = 0; i < size; ++i)
        {//distSubject - абонент набранный на кейпаде
            distSubject += static_cast<uint16_t> (pow(10, (size-i-1)) * CallControl_->telephoneDynamicStorage[i]);
        }
        sprintf(msg,"abonent = %d",distSubject);
term2(msg)

        CallControl_->messageData.field.prevDistId = distSubject;

        if (1000 > distSubject && distSubject > 99)
        {
            doc["Own_Id"] = Json::getInstance()->thisStation.id;
            doc["Dist_Id"].add(distSubject);
            doc["Call_Type"] = 5;
            doc["Priority"] = CallControl_->assignedData.priority;
            doc["Link_Data"] = 0xFF;
//            CallControl_->sendJson(doc, capacity);

            CallControl_->requestCount = 0;
            std::fill(CallControl_->messageData.txBuff, CallControl_->messageData.txBuff + CallControl_->messageData.txBuffSize, 0);
            if (serializeJson(doc, CallControl_->messageData.txBuff, capacity) > 0) {
                sendUdpMulticast(CallControl_->messageData.txBuff, strlen(CallControl_->messageData.txBuff));
            }
            CallControl_->requestCount++;

//            CallControl_->copyRecvBuff(CallControl_->serviceData->recvBuffCopy,
//                    CallControl_->outputJsonBuff);
            CallControl_->osTimer.start(CallControl_->osTimer.request_timerId,
                    CallControl_->osTimer.request_timerStatus,
                    200);
        }

        CallControl_->telephoneDynamicStorage.clear();

    }
    //new telefone call
    else if (size > 2 && CallControl_->simplexTelephoneCall)
    {
        CallControl_->simplexTelephoneCall = false;
        for (uint8_t i = 0; i < size; ++i)
        {//distSubject - абонент набранный на кейпаде
            distSubject += static_cast<uint16_t> (pow(10, (size-i-1)) * CallControl_->telephoneDynamicStorage[i]);
        }
        sprintf(msg,"abonent simplex = %d",distSubject);
        term2(msg)
        CallControl_->telephoneDynamicStorage.clear();

        //Тут переход в симплекс и астерикс ловим уже там
        CallControl_->TransitionTo(new CallWaiting);
//        CallControl_->TransitionTo(new SimplexDirectCall);



    }

    else if (CallControl_->ordinaryTelephoneCall)
    {
        CallControl_->ordinaryTelephoneCall = false;
        switchLed(CallControl_->assignedData.key, false);
        switchLed(CallControl_->subjectKey.key, false);
        stopDtmfTone();
        CallControl_->resetData();
        CallControl_->telephoneDynamicStorage.clear();
//        CallControl_->osTimer.stop( CallControl_->osTimer.telephone_timerId,
//                CallControl_->osTimer.telephone_timerStatus);
        CallControl_->TransitionTo(new CallWaiting);
    }
    else if (CallControl_->simplexTelephoneCall)
    {
        CallControl_->simplexTelephoneCall = false;
        stopDtmfTone();
        CallControl_->resetData();
        CallControl_->telephoneDynamicStorage.clear();
        CallControl_->TransitionTo(new CallWaiting);
    }
    CallControl_->osTimer.stop(CallControl_->osTimer.telephone_timerId, CallControl_->osTimer.telephone_timerStatus);
    osMutexRelease(MutexCallControl_);
}

void autoAnswTimer_Callback(void const *arg)
{

    (void)arg;
    osMutexWait(MutexCallControl_, osWaitForever);
    CallControl_->answerUnknownCall();
    osMutexRelease(MutexCallControl_);

}

void requestTimer_Callback(void const *arg)
{
    (void)arg;
    osMutexWait(MutexCallControl_, osWaitForever);
    CallControl_->checkRequest();
    osMutexRelease(MutexCallControl_);
}

#ifdef __cplusplus
}
#endif
