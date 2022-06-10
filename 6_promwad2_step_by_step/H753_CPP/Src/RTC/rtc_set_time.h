#ifndef RTC_SET_TIME_H
#define RTC_SET_TIME_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
  \brief The function sets the rawtime variable
  \param rx_time time in seconds
  \retval None
  */
void TimeSet(time_t rx_time);

#ifdef __cplusplus
}
#endif

#endif /*RTC_SET_TIME_H*/
