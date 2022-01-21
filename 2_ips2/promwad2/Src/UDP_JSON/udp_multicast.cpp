#include "../UDP_JSON/udp_multicast.h"
#include "callwaiting.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <strings.h>
#include "lwip/sockets.h"
#include "net_sockets.h"


#define PORTNUM 65215//65001//99
#define UDP_RECV_TIMEOUT (1)

osMutexDef (mutexCallControl);
osMutexDef (mutexSock);
//osMutexDef (mutexS);

/**
  * @brief  RTP Status definition
  */
typedef enum {
    UDP_STATE_IDLE         = 0x00U,
    UDP_STATE_READY        = 0x01U,
    UDP_STATE_START        = 0x02U,
    UDP_STATE_SEND         = 0x03U,
    UDP_STATE_ERROR        = 0x04U,
    UDP_STATE_STOPPED      = 0x05U,
    UDP_STATE_UNKNOWN      = 0x06U

}
UDP_StatusTypeDef;

/**
  * @brief  RTP Structure definition
  */
typedef struct alignas (4)
{
    __IO UDP_StatusTypeDef       State;         /*!< UDP process current state   */

    struct sockaddr_in local;                   /*!< A local member in the network */

    struct sockaddr_in net_dest;                /*!< A destination member in the network */

//    alignas (4) char udp_send_packet[2048] = {0};                  /*!< Packet to send */

    int sock_id;                                /*!< socket ID */

}
UDP_HandleTypeDef;

static UDP_HandleTypeDef udpStructSend; /* UDP structure */

//static int sock = -1;
static int                sockUdpRecv;
static struct ip_mreq     ipmreqUdpRecv;
//static struct sockaddr_in to;

#ifdef __cplusplus
}
#endif

extern void parsingFirmwareFromJson(JsonDocument &doc);
extern void rebootMcuByJson(JsonDocument &doc);
extern void deleteConfigMcuByJson(JsonDocument &doc);
extern void writeConfigMcuByJson(JsonDocument &doc);

static CallControl callControl_(new CallWaiting);

UdpJsonExch::UdpJsonExch()
{

    callControl = &callControl_;

    mutexSock_id = osMutexCreate(osMutex(mutexSock));
    if (mutexSock_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << "Failed to create [mutexSock]" << "\n";
    }

//    mutexSend_sock_id = osMutexCreate(osMutex(mutexS));
//    if (mutexSend_sock_id == nullptr) {
//        while(1)
//            Debug::getInstance().dbg << "Failed to create [mutexSock]" << "\n";
//    }

    mutexCallControlId = osMutexCreate(osMutex(mutexCallControl));
    if (mutexCallControlId == nullptr) {
        while(1)
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

UdpJsonExch *UdpJsonExch::getInstance()
{
    if (!pInstance) pInstance = new UdpJsonExch();
    return pInstance;
}

UdpJsonExch *UdpJsonExch::pInstance = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

void udpSendInit(void)
{
    /* Reset the RTP structure */
    memset(&udpStructSend, 0, sizeof(udpStructSend));

    /* RTP state idle */
    udpStructSend.State = UDP_STATE_IDLE;

//    uint32_t port = reinterpret_cast<uint32_t>(arg);

    /* Create new socket */
    udpStructSend.sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpStructSend.sock_id >= 0) {

        /* Prepare local address */
        memset(&udpStructSend.local, 0x00, sizeof(udpStructSend.local));
        udpStructSend.local.sin_family = AF_INET;
        udpStructSend.local.sin_port = PP_HTONS(INADDR_ANY);
        udpStructSend.local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

        /* Bind to local address */
        if (bind(udpStructSend.sock_id, reinterpret_cast<struct sockaddr *>(&udpStructSend.local), sizeof(udpStructSend.local)) == 0) {

            /* prepare UDP address */
            memset(&udpStructSend.net_dest, 0x00, sizeof(udpStructSend.net_dest));
            udpStructSend.net_dest.sin_family = AF_INET;
            udpStructSend.net_dest.sin_port = PP_HTONS(PORTNUM);
            udpStructSend.net_dest.sin_addr.s_addr = inet_addr("232.0.0.0");

            /* Reset UDP packet */
//            memset(udpStructSend.udp_send_packet, 0x00, sizeof(udpStructSend.udp_send_packet));

            /* Next state is UDP_STATE_START */
            udpStructSend.State = UDP_STATE_START;
        } else {
            /* Binding local adress failed */
            udpStructSend.State = UDP_STATE_ERROR;
        }
    }

}

void recvUdpThread(const void *arg)
{
    (void)arg;
    udpSendInit();

    const int capacity = JSON_OBJECT_SIZE(6)  + JSON_ARRAY_SIZE(100);
    DynamicJsonDocument recvDoc (capacity);

    struct sockaddr_in local;
    struct sockaddr_in from;
    int                fromlen;
    int                timeout;
    int                result;

    /* create new socket */
    sockUdpRecv = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockUdpRecv >= 0) {
        /* prepare local address */
        memset(&local, 0, sizeof(local));
        local.sin_family      = AF_INET;
        local.sin_port        = PP_HTONS(PORTNUM);
        local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

        /* bind to local address */
        if (bind(sockUdpRecv, reinterpret_cast<struct sockaddr *>(&local), sizeof(local)) == 0) {

            /* set recv timeout */
            timeout = UDP_RECV_TIMEOUT;
            setsockopt(sockUdpRecv, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&timeout), sizeof(timeout));

            /* prepare multicast "ip_mreq" struct */
            ipmreqUdpRecv.imr_multiaddr.s_addr = inet_addr("232.0.0.0");
            ipmreqUdpRecv.imr_interface.s_addr = PP_HTONL(INADDR_ANY);

            /* join multicast group */
            if (setsockopt(sockUdpRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmreqUdpRecv, sizeof(ipmreqUdpRecv)) == 0) {
                /* receive RTP packets */
                while (1) {
                    fromlen = sizeof(from);
                    osMutexWait(UdpJsonExch::getInstance()->mutexSock_id, osWaitForever);
                    result = recvfrom(sockUdpRecv, &UdpJsonExch::getInstance()->recvBuff, sizeof(UdpJsonExch::getInstance()->recvBuff), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
                    osMutexRelease(UdpJsonExch::getInstance()->mutexSock_id);

                    if (result != 0) {

                        if (Json::getInstance()->deserialize(recvDoc, (void *)UdpJsonExch::getInstance()->recvBuff, strlen(UdpJsonExch::getInstance()->recvBuff))) {

                            osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
                            if (!UdpJsonExch::getInstance()->callControl->sendInfoAboutStation(recvDoc)) {
                                UdpJsonExch::getInstance()->callControl->jsonMessage(recvDoc);
                                parsingFirmwareFromJson(recvDoc);
                                rebootMcuByJson(recvDoc);
                                deleteConfigMcuByJson(recvDoc);
                                writeConfigMcuByJson(recvDoc);
                            }
                            osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
                        }
                    }
                    osDelay(1);
                }
            }
            /* leave multicast group */
            setsockopt(sockUdpRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ipmreqUdpRecv, sizeof(ipmreqUdpRecv));
        }
    }
    /* close the socket */
    closesocket(sockUdpRecv);
}


void sendUdpMulticast(char *sendBuff, size_t size)
{
    if (udpStructSend.State == UDP_STATE_START) {
//        memcpy(udpStructSend.udp_send_packet, sendBuff, size);
//        osMutexWait(UdpJsonExch::getInstance()->mutexSend_sock_id, osWaitForever);
        sendto(udpStructSend.sock_id, sendBuff, size, 0, reinterpret_cast<struct sockaddr *>(&udpStructSend.net_dest), sizeof(struct sockaddr));
//        osMutexRelease(UdpJsonExch::getInstance()->mutexSend_sock_id);
    }
}
#ifdef __cplusplus
}
#endif
