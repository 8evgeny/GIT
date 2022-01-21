/*!
 \file os_timers.h

*/

#ifndef TIMER_CALLBACK_FUNCTIONS_H
#define TIMER_CALLBACK_FUNCTIONS_H

#include "cmsis_os.h"
#include <math.h>

/*!
 \brief This class is used to work with OS timers

 \class OSTimer os_timers.h "os_timers.h"
*/
class OSTimer
{

public:
    /*!
     \brief Base constructor.

     \fn OSTimer
    */
    OSTimer();
    /*!
     \brief Copy constructor. A default constructor.

     \fn OSTimer
     \param
    */
    OSTimer(const OSTimer&) = default;
    /*!
     \brief Copy assignment operator.

     \fn operator =
     \param
     \return OSTimer &operator
    */
    OSTimer& operator=(OSTimer&) = default;


    /*!
    \brief The function that is used to start a software timer

    \fn startTimer
    \param id is a timer ID
    \param status is reference to the status timer
    \param timerDelay is the time of the delay
    \return osStatus is status code values returned by CMSIS-RTOS functions.
    */
    osStatus start(osTimerId id, osStatus& status, uint16_t timerDelay);


    /*!
    \brief The function that is used to stop a software timer

    \fn stopTimer
    \param id is a timer ID
    \param status is reference to the status timer
    \return osStatus is status code values returned by CMSIS-RTOS functions.
    */
    osStatus stop(osTimerId id, osStatus& status);

    osTimerId button_timerId,
              autoAnsw_timerId, /*!< TODO: describe */
              request_timerId, /*!< TODO: describe */
              telephone_timerId; /*!< TODO: describe */

    osStatus button_timerStatus = osErrorOS, /*!< TODO: describe */
             autoAnsw_timerStatus = osErrorOS, /*!< TODO: describe */
             request_timerStatus = osErrorOS, /*!< TODO: describe */
             telephone_timerStatus = osErrorOS; /*!< TODO: describe */

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief This callback function is called to handle a released button

\fn handleRelasedButtonTimer_Callback
 \param arg is not used
        */
void handleRelasedButtonTimer_Callback(void const *arg);
/*!
 \brief This callback function is called to handle pressed buttons during telephone call

\fn dialingTimer_Callback
 \param arg is not used
        */
void dialingTimer_Callback(void const *arg);
/*!
 \brief This callback function is called to answer to the unknown incoming call

\fn autoAnswTimer_Callback
 \param arg is not used
        */
void autoAnswTimer_Callback(void const *arg);
/*!
 \brief This callback function is called to send repeated request if no response

\fn requestTimer_Callback
 \param arg is not used
    */
void requestTimer_Callback(void const *arg);


#ifdef __cplusplus
}
#endif

#endif // TIMER_CALLBACK_FUNCTIONS_H
