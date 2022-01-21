#ifndef RTP_SENDER_H
#define RTP_SENDER_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "stm32f7xx_hal.h"

#define RTP_VERSION                 0x80
#define RTP_TIMESTAMP               20
#define RTP_MARKER_BIT              0x80
#define RTP_PACKET_SIZE             ((640 * 2) + sizeof (rtp_hdr_t))
#define RTP_PAYLOAD_SIZE_MAX        1400
#define RTP_PAYLOAD_TYPE            0x100


#define RTP_RECV_TIMEOUT (1)
#define PORT_RTP_SIZE (RTP_PACKET_SIZE)

PACK_STRUCT_BEGIN
struct rtp_hdr_t {
    PACK_STRUCT_FIELD(u8_t  version);
    PACK_STRUCT_FIELD(u8_t  payloadtype);
    PACK_STRUCT_FIELD(u16_t seqNum);
    PACK_STRUCT_FIELD(u32_t timestamp);
    PACK_STRUCT_FIELD(u32_t ssrc);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

/** RTP stats display period - in received packets */
#ifndef RTP_RECV_STATS
#define RTP_RECV_STATS  (50)
#endif

/** RTP stream multicast address as IPv4 address in "u32_t" format */
#ifndef RTP_STREAM_ADDRESS
#define RTP_STREAM_ADDRESS          inet_addr("232.0.0.0")
#endif

/*!
 \brief Thread receiving rtp packets

 \fn rtpRecvThread
 \param arg RTP port
*/
void rtpRecvThread(void const *arg);

/*!
 \brief Thread initialization rtp sending

 \fn rtpSendInit
 \param arg RTP port
*/
void rtpSendInit(void const *arg);

/*!
 \brief RTP sending stream of the first half of audio data

 \fn sendHalfThread
 \param arg Doesn't use
*/
_Noreturn void sendHalfThread(void const *arg);

/*!
 \brief RTP sending stream of the second half of audio data

 \fn sendFullThread
 \param arg Doesn't use
*/
_Noreturn void sendFullThread(void const *arg);

/**
  * @brief  Sending thread
  * @param  None
  * @retval None
  */
_Noreturn void lostPackThread(void const *arg);

/**
  * @brief  Connection Status definition
  */
typedef enum {
    CONNECT_OK         = 0x00U,
    CONNECT_CLOSED     = 0x01U,
    CONNECT_ERROR      = 0x02U

} Connection_StatusTypeDef;

/**
  * @brief  RTP Status definition
  */
typedef enum {
    RTP_STATE_IDLE         = 0x00U,
    RTP_STATE_READY        = 0x01U,
    RTP_STATE_START        = 0x02U,
    RTP_STATE_SEND         = 0x03U,
    RTP_STATE_ERROR        = 0x04U,
    RTP_STATE_STOPPED      = 0x05U,
    RTP_STATE_UNKNOWN      = 0x06U

} RTP_StatusTypeDef;

/**
  * @brief  RTP Structure definition
  */
typedef struct alignas(4)
{
    __IO RTP_StatusTypeDef       State;         /*!< RTSP process current state   */

    struct sockaddr_in local;                   /*!< A local member in the network */

    struct sockaddr_in net_dest;                /*!< A destination member in the network */

    char *rtp_data;                             /*!< Pointer to RTP/RAW packet */

    uint32_t Offset;                            /*!< RTP/JPEG Offset */

    uint8_t rtp_send_packet[RTP_PACKET_SIZE];   /*!< Packet to send */

    int sock_id;                                /*!< socket ID */

}
RTP_HandleTypeDef;


#ifdef __cplusplus
}
#endif


#endif // RTP_SENDER_H
