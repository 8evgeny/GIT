/*!
 \file rtc.h

*/

#ifndef RTC_H
#define RTC_H

#include <cstdint>
#include <ctime>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rtc.h"

/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class RTC rtc.h "rtc.h"
*/

class sRTC
{
private:
    static sRTC * pInstance;

    /*!
     \brief Base constructor. A default constructor.

     \fn sRTC
    */
    sRTC();

    /*!
     \brief Copy constructor.

     \fn sRTC
     \param
    */
    sRTC(const sRTC&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return sRTC &operator
    */
    sRTC& operator=(sRTC&);

public:

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return RTC. The public static operation can be accessed easily by using the class name and operation name.
    */
    static sRTC * getInstance();

    /*!
     * \brief Configures the current time and date.
     * \param none
     * \return none
     */
    void RTC_CalendarConfig();

    /*!
     * \brief Displays the current time and date.
     * \param none
     * \return none
     */
    void RTC_CalendarShow();


    RTC_HandleTypeDef *rtcHandle; /*!< The variable is pointer to the RTC Handle Type */
    RTC_DateTypeDef sDateStruct; /*!< The object of the RTC_DateTypeDef class */
    RTC_TimeTypeDef sTimeStruct; /*!< The object of the RTC_TimeTypeDef class */
    time_t rawtime; /*!< The variable stores the received time in seconds */

    /*!
      \brief RTC Test Function
      \param None
      \retval None
      */
    void test(void);
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief RTCInit Initialization
 */
void RTCInit(void);

#ifdef __cplusplus
}
#endif

#endif // RTC_H
