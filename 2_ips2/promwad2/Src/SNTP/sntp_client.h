#ifndef SNTP_CLIENT_H
#define SNTP_CLIENT_H

#include <string.h>

#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/apps/sntp.h"
#include "rtc.h"

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * \brief The function creates SNTP Thread
 * \param none
 * \return none
 */
void SNTP_Start(void);

#ifdef __cplusplus
}
#endif

#endif /*SNTP_CLIENT_H*/
