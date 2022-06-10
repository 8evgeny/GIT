/*!
 \file udp_multicast.h

*/

#ifndef UDP_MULTICAST_H
#define UDP_MULTICAST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif

#include "callcontrol.h"

/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class UdpJsonExch udp_multicast.h "udp_multicast.h"
*/
class UdpJsonExch
{
private:
    static UdpJsonExch * pInstance; /**< The static pointer to the CAN class */

    /*!
     \brief Base constructor. A default constructor.

     \fn UdpJsonExch
    */
    UdpJsonExch();

    /*!
     \brief Copy constructor.

     \fn UdpJsonExch
     \param
    */
    UdpJsonExch(const UdpJsonExch&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return UdpJsonExch &operator
    */
    UdpJsonExch& operator=(UdpJsonExch&);

public:



    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return UdpJsonExch. The public static operation can be accessed easily by using the class name and operation name.
    */
    static UdpJsonExch * getInstance();

    osThreadId sendUdpThreadId,     /*!< The variable stores ID of the sendUdpThreadI */
               recvUdpThreadId;     /*!< The variable stores ID of the recvUdpThread */
    osMutexId  mutexSock_id,        /*!< The variable stores ID of the mutexSock */
               mutexSend_sock_id,   /*!< The variable stores ID of the mutexSend_sock */
               mutexCallControlId;

    CallControl *callControl;

    alignas(4) char recvBuff[2048] = {0};


};

/*!
 \brief

 \fn firmwareInitThread
*/
void firmwareInitThread();

#ifdef __cplusplus
extern "C" {
#endif
/*!
 \brief The function creates the socket to send UDP packets and binds address

 \fn udpSendInit
 \return int socket descriptor
*/
void udpSendInit(void);

/*!
 \brief The function creates the socket to receive multicast UDP packets and binds address

 \fn UdpMulticastInit
 \return int socket descriptor
*/
int UdpMulticastInit();

//[[ noreturn ]]
/*!
 \brief The OS thread function receives UDP data and puts it to the buffer
        for further processing

 \fn recvUdpThread
 \param arg not used
*/
void recvUdpThread(const void *arg);

//[[ noreturn ]]
///*!
// \brief The OS thread function sends JSON document through UDP

// \fn sendUdpThread
// \param arg not used
//*/
//void sendUdpThread(const void *arg);

/*!
 \brief The function sends UDP data

 \fn sendUdpMulticast
 \param sendBuff pointer to the send buffer
 \param size size of send buffer
*/
void sendUdpMulticast(char* sendBuff, size_t size);

#ifdef __cplusplus
}
#endif

#endif //UDP_MULTICAST_H
