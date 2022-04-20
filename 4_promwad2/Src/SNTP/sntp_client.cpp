/*!
 \file sntp_client.cpp

 \authors Androshchuk A.V.

 \code
    SNTP_Start();

    ...

    while (1) {

        sRTC::getInstance()->test();
    }

 \endcode

*/

#include "sntp_client.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
  \brief Create SNTP task function
  \param None
  \retval None
  */
static
void SNTP_Client(const void *arg)
{
    (void)arg;

    ip_addr_t addr;
    sntp_setoperatingmode(SNTP_OPMODE_POLL); // Setting operation mode SNTP
    inet_pton(AF_INET, "10.54.5.65", &addr);
    sntp_setserver(0, &addr); // Setting server address

    /* Init SNTP Client */
    sntp_init();

    sRTC *rtc = sRTC::getInstance();

    while(1) {

        // work_RTC->getInstance()->RTC_CalendarConfig();
        // work_RTC->getInstance()->RTC_CalendarShow();
        rtc->getInstance()->test();


    }

}

void SNTP_Start()
{
    /* Start SNTP Client task : Send recuest to SNTP server */
    osThreadDef(sntp, SNTP_Client, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 40);

    osThreadCreate(osThread(sntp), NULL);
}

#ifdef __cplusplus
}
#endif
