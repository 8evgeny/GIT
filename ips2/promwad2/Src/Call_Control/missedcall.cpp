#include "missedcall.h"
#include "../UDP_JSON/udp_multicast.h"

osThreadDef(missedCallThread, missedCallThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);

MissedCall::MissedCall()
{
    missedCalls.reserve(32);

    if (osThreadCreate(osThread(missedCallThread), nullptr) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

void MissedCall::add(const uint8_t key)
{
    if (key != 0) {
        if (!seek(key)) {
            MissedCallData missedCallData;
            missedCallData.key = key;
            missedCalls.push_back(missedCallData);
        }
        switchLed(key, true, 100, 900);
    }
}

bool MissedCall::remove(const uint8_t key)
{
    if (seek(key)) {
        switchLed(key, false);
        missedCalls.erase(i);
        i = missedCalls.begin();
        return true;
    } else {
        return false;
    }
}

bool MissedCall::seek(const uint8_t key)
{
    if (!missedCalls.empty()) {
        i = missedCalls.begin();
        for (uint8_t j = 0; j < missedCalls.size(); ++j) {
            if ((i += j)->key == key) {
                return true;
            }
        }
    }
    return false;
}

void MissedCall::handleAutoReset()
{
    if (!missedCalls.empty())
        for (auto& n : missedCalls) {
            if (n.conter < MAX_COUNT) {
                n.conter++;
            } else {
                remove(n.key);
            }
        }
}

#ifdef __cplusplus
extern "C" {
#endif


[[noreturn]]
void missedCallThread (void const *arg)
{
    (void)arg;
    while(true) {
        osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
        UdpJsonExch::getInstance()->callControl->missedCall.handleAutoReset();
        osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
        osDelay(1000);
    }

}

#ifdef __cplusplus
}
#endif
