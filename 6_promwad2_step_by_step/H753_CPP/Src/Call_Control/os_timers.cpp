#include "os_timers.h"
#include "../Debug/debug.h"
#include "../UDP_JSON/udp_multicast.h"
#include "callwaiting.h"
#include "conferencecall.h"

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
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((telephone_timerId = osTimerCreate (osTimer(dialingTimer), osTimerPeriodic, nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((autoAnsw_timerId = osTimerCreate (osTimer(autoAnswTimer), osTimerPeriodic, nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

    if ((request_timerId = osTimerCreate (osTimer(requestTimer), osTimerPeriodic, nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

osStatus OSTimer::start(osTimerId id, osStatus& status, const uint16_t timerDelay)
{
    status = osTimerStart (id, timerDelay);     // timer starting
    if (status != osOK)  {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    return status;
}

osStatus OSTimer::stop(osTimerId id, osStatus& status)
{
    if (status == osOK) {
        status = osTimerStop (id);            // timer stopping
        if (status != osOK)  {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
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

    osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
    if (!UdpJsonExch::getInstance()->callControl->isIncomingCall ||
            UdpJsonExch::getInstance()->callControl->assignedData.key == CallControl::Asterisk) {
        if (UdpJsonExch::getInstance()->callControl->subjectKey.key == UdpJsonExch::getInstance()->callControl->assignedData.key) {


            if (keyMode == NotFixed) {
                switch (func) {
                case CallControl::Direct: {
                    if (!UdpJsonExch::getInstance()->callControl->missedCall.isMissedKey)
                        UdpJsonExch::getInstance()->callControl->handleNotFixedButton();
                }
                break;
                case CallControl::Group:
                    UdpJsonExch::getInstance()->callControl->handleNotFixedButton();
                    break;
                case CallControl::Circular:
                    UdpJsonExch::getInstance()->callControl->microphone.stop();
                    break;
                case CallControl::Conference:
                    UdpJsonExch::getInstance()->callControl->microphone.stop();
                    break;
                case CallControl::Telephone:
                    UdpJsonExch::getInstance()->callControl->microphone.stop();

                    break;
                default:
                    break;
                }

            }
        } else if (UdpJsonExch::getInstance()->callControl->assignedData.key == CallControl::Asterisk) {
            if (UdpJsonExch::getInstance()->callControl->subjectKey.key != CallControl::Asterisk
                    && UdpJsonExch::getInstance()->callControl->subjectKey.key != CallControl::Hash)
                switchLed(UdpJsonExch::getInstance()->callControl->subjectKey.key, false);
            stopDtmfTone();
        }

    }

    UdpJsonExch::getInstance()->callControl->osTimer.stop( UdpJsonExch::getInstance()->callControl->osTimer.button_timerId,
            UdpJsonExch::getInstance()->callControl->osTimer.button_timerStatus);

    osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);

    handleClick_count = 0;
    pressedKey = 0;
}



void dialingTimer_Callback(void const *arg)
{
    (void)arg;

    const int capacity = JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument doc (capacity);

    osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);

    uint8_t size = static_cast<uint8_t>(UdpJsonExch::getInstance()->callControl->telephoneDynamicStorage.size());
    uint16_t distSubject = 0;

    if (size > 2) {
        for (uint8_t i = 0; i < size; ++i) {
            distSubject += static_cast<uint16_t> (pow(10, (size-i-1))
                                                  * UdpJsonExch::getInstance()->callControl->telephoneDynamicStorage[i]);
        }

        UdpJsonExch::getInstance()->callControl->messageData.field.prevDistId = distSubject;

        if (65001 > distSubject && distSubject > 99) {
            doc["Own_Id"] = Json::getInstance()->thisStation.id;
            doc["Dist_Id"].add(distSubject);
            doc["Call_Type"] = 5;
            doc["Priority"] = UdpJsonExch::getInstance()->callControl->assignedData.priority;
            doc["Link_Data"] = 0xFF;
//            UdpJsonExch::getInstance()->callControl->sendJson(doc, capacity);

            UdpJsonExch::getInstance()->callControl->requestCount = 0;
            std::fill(UdpJsonExch::getInstance()->callControl->messageData.txBuff, UdpJsonExch::getInstance()->callControl->messageData.txBuff + UdpJsonExch::getInstance()->callControl->messageData.txBuffSize, 0);
            if (serializeJson(doc, UdpJsonExch::getInstance()->callControl->messageData.txBuff, capacity) > 0) {
                sendUdpMulticast(UdpJsonExch::getInstance()->callControl->messageData.txBuff, strlen(UdpJsonExch::getInstance()->callControl->messageData.txBuff));
            }
            UdpJsonExch::getInstance()->callControl->requestCount++;

//            UdpJsonExch::getInstance()->callControl->copyRecvBuff(UdpJsonExch::getInstance()->callControl->serviceData->recvBuffCopy,
//                    UdpJsonExch::getInstance()->callControl->outputJsonBuff);
            UdpJsonExch::getInstance()->callControl->osTimer.start(UdpJsonExch::getInstance()->callControl->osTimer.request_timerId,
                    UdpJsonExch::getInstance()->callControl->osTimer.request_timerStatus,
                    200);
        }

        UdpJsonExch::getInstance()->callControl->telephoneDynamicStorage.clear();

    } else {
        switchLed(UdpJsonExch::getInstance()->callControl->assignedData.key, false);
        switchLed(UdpJsonExch::getInstance()->callControl->subjectKey.key, false);
        stopDtmfTone();
        UdpJsonExch::getInstance()->callControl->resetData();
        UdpJsonExch::getInstance()->callControl->telephoneDynamicStorage.clear();
//        UdpJsonExch::getInstance()->callControl->osTimer.stop( UdpJsonExch::getInstance()->callControl->osTimer.telephone_timerId,
//                UdpJsonExch::getInstance()->callControl->osTimer.telephone_timerStatus);
        UdpJsonExch::getInstance()->callControl->TransitionTo(new CallWaiting);
    }
    UdpJsonExch::getInstance()->callControl->osTimer.stop(UdpJsonExch::getInstance()->callControl->osTimer.telephone_timerId,
            UdpJsonExch::getInstance()->callControl->osTimer.telephone_timerStatus);
    osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
}

void autoAnswTimer_Callback(void const *arg)
{

    (void)arg;
    osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
    UdpJsonExch::getInstance()->callControl->answerUnknownCall();
    osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);

}

void requestTimer_Callback(void const *arg)
{
    (void)arg;
    osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
    UdpJsonExch::getInstance()->callControl->checkRequest();
    osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
}

#ifdef __cplusplus
}
#endif
