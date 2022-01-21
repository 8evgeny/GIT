#include "../AUDIO/sai.h"
#include "../Debug/debug.h"
#include <cstring>
#include <algorithm>
#include "../UDP_JSON/udp_multicast.h"


//TODO need to use namespace for signals

#ifdef __cplusplus
extern "C" {
#endif

#include "rtp.h"
#include "arm_math.h"


#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(a,b) (((a)>(b))?(a):(b))

osThreadId recvThreadId;              /* Thread ID */
osThreadId sendThreadId;              /* Thread ID */

osThreadId threadAudioTxFullId;       /* Thread ID */
osThreadId threadAudioTxHalfId;       /* Thread ID */

osThreadId lostPackThreadId;

extern osMutexId mutexRtpRxId;        /* Mutex ID */
extern osMutexId mutexMixRtpRxId;     /* Mutex ID */
extern osMutexId mutexRtpTxId;        /* Mutex ID */

extern osSemaphoreId semaphoreRtpTxId;        /* Mutex ID */

extern osMutexId mutexCryptTxId;      /* Mutex ID */
extern osMutexId mutexCryptRxId;      /* Mutex ID */
extern CRYP_HandleTypeDef hcryp;

extern osThreadId sendThreadHalfId;
extern osThreadId sendThreadFullId;
extern osThreadId sendThreadMixAudioId;

int                sockRtpRecv;
struct ip_mreq     ipmreqRtpRecv;

uint8_t rtpDataTxHalf[BUFFER_AUDIO_SIZE_RTP];
uint8_t rtpDataTxFull[BUFFER_AUDIO_SIZE_RTP];

RTP_HandleTypeDef rtpStructSend; /* RTP structure */

static uint16_t lostPackCounter = 0;
constexpr static uint16_t MAX_NUMBER_LOST_PACK = 1200;//50;

static u8_t rtpRecvPacket[PORT_RTP_SIZE];
static uint16_t rtpDataRxMixCrypt[BUFFER_AUDIO_SIZE_RTP / 2];
static uint16_t rtpDataRxMixAudio[BUFFER_AUDIO_SIZE_RTP / 2];
static uint16_t rtpDataRxMixAudioDst[BUFFER_AUDIO_SIZE_RTP / 2];
static RtpPackages inMix;

_Noreturn void timerForMixAudio(void const *arg)
{
    UNUSED(arg);
    while (1) {
        using SsrcIndex = struct alignas (4) {
            uint16_t seqNum;
            uint16_t empty;
            int index;
        };

        std::list <uint32_t> ssrcTmp;
        const uint16_t DIFFERENCE_BETWEEN_PACKAGES = 30000;

        //поиск ssrc которые есть в кольцевом буфере
        osMutexWait(mutexMixRtpRxId, osWaitForever);
        for(int i = 0; i < COUNTER_RTP_MIX; i++) {
            if(SAI::getInstance()->outMixRingBuffer[i].header.ssrc != 0) {
                auto it = std::find(ssrcTmp.begin(), ssrcTmp.end(), SAI::getInstance()->outMixRingBuffer[i].header.ssrc);
                if (it == ssrcTmp.end()) {
                    ssrcTmp.push_back(SAI::getInstance()->outMixRingBuffer[i].header.ssrc);
                }
            }
        }

        //если хотябы один отличный элемент ssrc есть в кольцевом буфер только условие верно
        //нельзя удалять элементы с кольцевого буфера, кольцевой буфер должен быть всегда заполнен
        if((ssrcTmp.size() == SAI::getInstance()->ssrc.size()) && (ssrcTmp.size() != 0)) {
            //все ок
            SAI::getInstance()->ssrcCounterPackages.clear();

            for(const auto &ssrc : ssrcTmp) {
                //в этом списке хранятся номера всех пактов для одно устройство согласно ssrc
                std::list <SsrcIndex> indexRingBufferAndSeqNumber;
                std::list <uint16_t> seqNumbers;

                for(int i = 0; i < COUNTER_RTP_MIX; i++) {
                    if(SAI::getInstance()->outMixRingBuffer[i].header.ssrc == ssrc) {
                        SsrcIndex indexTmp;

                        indexTmp.seqNum = SAI::getInstance()->outMixRingBuffer[i].header.seqNum;
                        indexTmp.index = i;

                        indexRingBufferAndSeqNumber.push_back(indexTmp);
                        seqNumbers.push_back(indexTmp.seqNum);
                    }
                }


                seqNumbers.sort();

                uint16_t seqNumberMin = seqNumbers.front();
                uint16_t seqNumberMax = seqNumbers.back();
                //поиск головы и хвоста в произвольном массиве

                //необходимо найти первый элемент из данного списка для суммирования, после того как найден первый элемент
                //в кольцевом буфере неоходимо его переместить в временный кольцевой буфер по индексу
                //после перемещения элемента, индекс основного кольцевого буфера необходимо занулить

                //переполнение по seqNumber не было
                if(abs(seqNumberMax - seqNumberMin) < DIFFERENCE_BETWEEN_PACKAGES) {
                    for(const auto & item : indexRingBufferAndSeqNumber) {
                        if(item.seqNum != seqNumberMin) {
                            continue;
                        }

                        SAI::getInstance()->tmpRingBuffer.push(SAI::getInstance()->outMixRingBuffer[item.index]);

                        RtpPackages erase;

                        erase.header.ssrc = 0;
                        erase.header.seqNum = 0;

                        //необходимо защитить мьютексом
                        SAI::getInstance()->outMixRingBuffer[item.index] = erase;
                        break;
                    }
                } else {
                    std::list<uint16_t>::iterator it;

                    uint16_t seqNumberMain = 0;
                    //поиск границы
                    for(uint32_t i = 1; i < seqNumbers.size(); i++) {

                        it  = seqNumbers.begin();
                        std::advance(it, i - 1);
                        uint16_t first = *it;

                        it = std::next(it);
                        uint16_t second = *it;

                        if(abs(second - first) < DIFFERENCE_BETWEEN_PACKAGES) {
                            continue;
                        }

                        seqNumberMain = second;
                        break;
                    }

                    for(const auto & item : indexRingBufferAndSeqNumber) {
                        if(item.seqNum != seqNumberMain) {
                            continue;
                        }

                        SAI::getInstance()->tmpRingBuffer.push(SAI::getInstance()->outMixRingBuffer[item.index]);
                        RtpPackages erase;
                        erase.header.ssrc = 0;
                        erase.header.seqNum = 0;

                        //необходимо защитить мьютексом
                        SAI::getInstance()->outMixRingBuffer[item.index] = erase;
                        break;
                    }
                }
            }
            //проверить сколько элементов в tmpRingBuffer
//            std::fill(rtpDataRxMixAudio, rtpDataRxMixAudio + BUFFER_AUDIO_SIZE_RTP / 2, 0);
            uint32_t sizeTmp =  SAI::getInstance()->tmpRingBuffer.size();
            if(sizeTmp >= ssrcTmp.size()) {
                arm_fill_q15(0, reinterpret_cast<q15_t *>(rtpDataRxMixAudio), BUFFER_AUDIO_SIZE_RTP / 2);
                for (uint32_t i = 0; i < sizeTmp; i++) {
                    inMix = SAI::getInstance()->tmpRingBuffer.shift();


                    osMutexWait(mutexCryptTxId, osWaitForever);
                    HAL_CRYP_Decrypt_DMA(&hcryp, reinterpret_cast<uint32_t *>(inMix.payload), BUFFER_AUDIO_SIZE_RTP / 4, reinterpret_cast<uint32_t *>(rtpDataRxMixCrypt));
                    while (!SAI::getInstance()->cryptTxComplete);
                    SAI::getInstance()->cryptTxComplete = false;
                    osMutexRelease(mutexCryptTxId);


                    arm_add_q15(reinterpret_cast<q15_t *>(rtpDataRxMixAudio), reinterpret_cast<q15_t *>(rtpDataRxMixCrypt), reinterpret_cast<q15_t *>(rtpDataRxMixAudioDst), BUFFER_AUDIO_SIZE_RTP / 2);
                    arm_copy_q15( reinterpret_cast<q15_t *>(rtpDataRxMixAudioDst), reinterpret_cast<q15_t *>(rtpDataRxMixAudio), BUFFER_AUDIO_SIZE_RTP / 2);
                }
                arm_copy_q15(reinterpret_cast<q15_t *>(rtpDataRxMixAudio), reinterpret_cast<q15_t *>(inMix.payload), BUFFER_AUDIO_SIZE_RTP / 2);

                osMutexWait(mutexRtpRxId, osWaitForever);
                SAI::getInstance()->outRingBuffer.push(inMix);
                osMutexRelease(mutexRtpRxId);
            }
        } else {

            std::vector<uint32_t> keys;

            //необходимо удалить элемент из списка основных ssrc если счетчик в мапе по ссрц привысел допустимое значение
            for(const auto& i : SAI::getInstance()->ssrcCounterPackages) {
                if(i.second >=  COUNTER_EMPTY_SSRC ) {
                    auto it = std::find(SAI::getInstance()->ssrc.begin(), SAI::getInstance()->ssrc.end(), i.first);
                    if (it != ssrcTmp.end()) {
                        SAI::getInstance()->ssrc.erase(it);
                        keys.push_back(i.first);
                    }
                }
            }

            //необходимо удалить элементы из мапы которые соотвествовали COUNTER_EMPTY_SSRC
            for(const auto& i : keys) {
                auto it = SAI::getInstance()->ssrcCounterPackages.find(i);
                if(it != SAI::getInstance()->ssrcCounterPackages.end()) {
                    SAI::getInstance()->ssrcCounterPackages.erase(it);
                }
            }


            //необходимо найти элементы которые не в списке
            for(const auto& i : SAI::getInstance()->ssrc) {
                auto it = std::find(ssrcTmp.begin(), ssrcTmp.end(), i);
                //если не найден элемент
                if (it == ssrcTmp.end()) {
                    //если элемента нету в мапе тогда необходимо добавить
                    //инача просто про инкрементировать счетчик существующего ssrc в мапе
                    auto iter = SAI::getInstance()->ssrcCounterPackages.find(i);
                    if(iter == SAI::getInstance()->ssrcCounterPackages.end()) {
                        SAI::getInstance()->ssrcCounterPackages.insert(std::pair<uint32_t, uint16_t>(i, 1));
                    } else {
                        iter->second++;
                    }
                }
            }
        }
        osMutexRelease(mutexMixRtpRxId);
        osDelay(1);
    }
}

/**
  * @brief  Initialize RTP thread
  * @param  None
  * @retval None
  */
void rtpSendInit(void const *arg)
{

    /* Reset the RTP structure */
    memset(&rtpStructSend, 0x00, sizeof(rtpStructSend));

    /* RTP state idle */
    rtpStructSend.State = RTP_STATE_IDLE;

    uint32_t port = reinterpret_cast<uint32_t>(arg);

    /* Create new socket */
    rtpStructSend.sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtpStructSend.sock_id >= 0) {

        /* Prepare local address */
        memset(&rtpStructSend.local, 0x00, sizeof(rtpStructSend.local));
        rtpStructSend.local.sin_family = AF_INET;
        rtpStructSend.local.sin_port = PP_HTONS(INADDR_ANY);
        rtpStructSend.local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

        /* Bind to local address */
        if (bind(rtpStructSend.sock_id, reinterpret_cast<struct sockaddr *>(&rtpStructSend.local), sizeof(rtpStructSend.local)) == 0) {

            /* prepare RTP stream address */
            memset(&rtpStructSend.net_dest, 0x00, sizeof(rtpStructSend.net_dest));
            rtpStructSend.net_dest.sin_family = AF_INET;
            rtpStructSend.net_dest.sin_port = PP_HTONS(port);
            rtpStructSend.net_dest.sin_addr.s_addr = RTP_STREAM_ADDRESS;

            /* Reset rtp packet */
            memset(rtpStructSend.rtp_send_packet, 0x00, sizeof(rtpStructSend.rtp_send_packet));

            /* Next state is RTP_STATE_START */
            rtpStructSend.State = RTP_STATE_START;
        } else {
            /* Binding local adress failed */
            rtpStructSend.State = RTP_STATE_ERROR;
        }
    }
}

/**
 * RTP send packets
 */
static void rtpSendPacketsHalf(int sock_id, struct sockaddr_in *net_dest)
{
    struct rtp_hdr *rtphdr;            /* RTP header */
    uint8_t *rtp_payload;              /* RTP payload */
    int rtp_payload_size = 0;          /* RTP payload size in the current packet */

    /* prepare RTP packet */
    rtphdr = reinterpret_cast<struct rtp_hdr *>(rtpStructSend.rtp_send_packet);
    rtphdr->version = RTP_VERSION;
    rtphdr->payloadtype = 0;
    rtphdr->ssrc = PP_HTONL(inet_addr(Json::getInstance()->thisStation.ip));
    rtphdr->timestamp = htonl(ntohl(rtphdr->timestamp) + RTP_TIMESTAMP);

    /* send RTP stream packets */
    rtpStructSend.rtp_data = reinterpret_cast<char *>(rtpDataTxHalf);

    /* Set a payload pointer */
    rtp_payload = rtpStructSend.rtp_send_packet + sizeof(struct rtp_hdr);

    /* Set a payload size */
    rtp_payload_size = static_cast<int>(sizeof(rtpDataTxHalf));

    /* Set a payload */
//    memcpy(rtp_payload, rtpStructSend.rtp_data, static_cast<size_t>(rtp_payload_size));
    arm_copy_q15(reinterpret_cast<q15_t *>(rtpStructSend.rtp_data), reinterpret_cast<q15_t *>(rtp_payload), static_cast<size_t>(rtp_payload_size) / 2);

    /* Send RTP stream packet */
    if(sendto(sock_id, rtpStructSend.rtp_send_packet, RTP_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr *>(net_dest), sizeof(struct sockaddr)) > 0) {
        rtphdr->seqNum = htons(ntohs(rtphdr->seqNum) + 1);
    }
}

/**
 * RTP send packets
 */
static void rtpSendPacketsFull(int sock_id, struct sockaddr_in *net_dest)
{
    struct rtp_hdr *rtphdr;            /* RTP header */
    uint8_t *rtp_payload;              /* RTP payload */
    int rtp_payload_size = 0;          /* RTP payload size in the current packet */

    /* prepare RTP packet */
    rtphdr = reinterpret_cast<struct rtp_hdr *>(rtpStructSend.rtp_send_packet);
    rtphdr->version = RTP_VERSION;
    rtphdr->payloadtype = 0;
    rtphdr->ssrc = PP_HTONL(inet_addr(Json::getInstance()->thisStation.ip));
    rtphdr->timestamp = htonl(ntohl(rtphdr->timestamp) + RTP_TIMESTAMP);

    /* send RTP stream packets */
    rtpStructSend.rtp_data = reinterpret_cast<char *>(rtpDataTxFull);

    /* Set a payload pointer */
    rtp_payload = rtpStructSend.rtp_send_packet + sizeof(struct rtp_hdr);

    /* Set a payload size */
    rtp_payload_size = static_cast<int>(sizeof(rtpDataTxFull));

    /* Set a payload */
//    memcpy(rtp_payload, rtpStructSend.rtp_data, static_cast<size_t>(rtp_payload_size));
    arm_copy_q15(reinterpret_cast<q15_t *>(rtpStructSend.rtp_data), reinterpret_cast<q15_t *>(rtp_payload), static_cast<size_t>(rtp_payload_size) / 2);

    /* Send RTP stream packet */
    if(sendto(sock_id, rtpStructSend.rtp_send_packet, RTP_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr *>(net_dest), sizeof(struct sockaddr)) > 0) {
        rtphdr->seqNum = htons(ntohs(rtphdr->seqNum) + 1);
    }
}

/**
 * RTP recv thread
 */
void rtpRecvThread(void const *arg)
{
    struct sockaddr_in local;
    struct sockaddr_in from;
    int                fromlen;

    u32_t              rtp_stream_address;
    int                result;
    RtpPackages in;

    /* initialize RTP stream address */
    rtp_stream_address = RTP_STREAM_ADDRESS;

    uint32_t port = reinterpret_cast<uint32_t>(arg);

    /* if we got a valid RTP stream address... */
    if (rtp_stream_address != 0) {
        /* create new socket */
        sockRtpRecv = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockRtpRecv >= 0) {

            int flags = fcntl(sockRtpRecv, F_GETFL, 0);
            fcntl(sockRtpRecv, F_SETFL, flags | O_NONBLOCK);

            /* prepare local address */
            memset(&local, 0, sizeof(local));
            local.sin_family      = AF_INET;
            local.sin_port        = PP_HTONS(port);
            local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

            /* bind to local address */
            if (bind(sockRtpRecv, reinterpret_cast<struct sockaddr *>(&local), sizeof(local)) == 0) {

                /* prepare multicast "ip_mreq" struct */
                ipmreqRtpRecv.imr_multiaddr.s_addr = rtp_stream_address;
                ipmreqRtpRecv.imr_interface.s_addr = PP_HTONL(INADDR_ANY);

                /* join multicast group */
                if (setsockopt(sockRtpRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmreqRtpRecv, sizeof(ipmreqRtpRecv)) == 0) {
                    /* receive RTP packets */
                    while (1) {
                        fromlen = sizeof(from);
                        result  = recvfrom(sockRtpRecv, rtpRecvPacket, sizeof(rtpRecvPacket), 0, reinterpret_cast<struct sockaddr *>(&from), reinterpret_cast<socklen_t *>(&fromlen));
                        if (result >= static_cast<int>(sizeof(struct rtp_hdr))) {
                            lostPackCounter = 0;
                            //copy header
                            arm_copy_q7(reinterpret_cast<q7_t *>(rtpRecvPacket), reinterpret_cast<q7_t *>(&in.header),  sizeof(rtp_hdr));
                            //copy payload
                            arm_copy_q7(reinterpret_cast<q7_t *>(rtpRecvPacket + sizeof(rtp_hdr)), reinterpret_cast<q7_t *>(in.payload), BUFFER_AUDIO_SIZE_RTP);

                            osMutexWait(mutexMixRtpRxId, osWaitForever);
                            auto it = std::find(SAI::getInstance()->ssrc.begin(), SAI::getInstance()->ssrc.end(), in.header.ssrc);
                            if (it == SAI::getInstance()->ssrc.end()) {
                                SAI::getInstance()->ssrc.push_back(in.header.ssrc);
                            }
                            SAI::getInstance()->outMixRingBuffer[SAI::getInstance()->outMixRingBufferPosition] = in;
                            SAI::getInstance()->outMixRingBufferPosition = (SAI::getInstance()->outMixRingBufferPosition + 1) % COUNTER_RTP_MIX;
                            osMutexRelease(mutexMixRtpRxId);
                        } else {
                            lostPackCounter++;
                            if (lostPackCounter > MAX_NUMBER_LOST_PACK) {
                                lostPackCounter = 0;
                                osSignalSet(lostPackThreadId, 0xFB);
                            }
                            osDelay(1);
                        }
                    }
                    /* leave multicast group */
                    setsockopt(sockRtpRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ipmreqRtpRecv, sizeof(ipmreqRtpRecv));
                }
            }
            /* close the socket */
            closesocket(sockRtpRecv);
        }
    }
}

/**
  * @brief  Sending thread
  * @param  None
  * @retval None
  */
_Noreturn void sendHalfThread(void const *arg)
{
    UNUSED(arg);
    while (1) {
        /* Try to obtain the semaphore. */
        if (rtpStructSend.State == RTP_STATE_START) {
            osEvent evt = osSignalWait(0x01, osWaitForever);

            if (evt.status == osEventSignal)  {
                osMutexWait(mutexRtpTxId, osWaitForever);
                rtpSendPacketsHalf(rtpStructSend.sock_id, &rtpStructSend.net_dest);
                osMutexRelease(mutexRtpTxId);
                osDelay(1);
            }
        } else {
            osDelay(1);
        }
    }
}

/**
  * @brief  Sending thread
  * @param  None
  * @retval None
  */
_Noreturn void sendFullThread(void const *arg)
{
    UNUSED(arg);
    while (1) {
        /* Try to obtain the semaphore. */
        if (rtpStructSend.State == RTP_STATE_START) {
            osEvent evt = osSignalWait(0x02, osWaitForever);

            if (evt.status == osEventSignal) {
                osMutexWait(mutexRtpTxId, osWaitForever);
                rtpSendPacketsFull(rtpStructSend.sock_id, &rtpStructSend.net_dest);
                osMutexRelease(mutexRtpTxId);
                osDelay(1);
            }
        } else {
            osDelay(1);
        }
    }
}

/**
  * @brief  Sending thread
  * @param  None
  * @retval None
  */
_Noreturn void lostPackThread(void const *arg)
{
    UNUSED(arg);

    while (1) {
        osEvent evt = osSignalWait(0xFB, osWaitForever);
        if (evt.status == osEventSignal) {
            osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
            UdpJsonExch::getInstance()->callControl->hungUp();
            osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);

        }
        osDelay(1);
    }
}

#ifdef __cplusplus
}
#endif
