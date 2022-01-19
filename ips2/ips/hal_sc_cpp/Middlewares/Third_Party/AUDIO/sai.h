#ifndef SAI_H
#define SAI_H

#include <cstdint>
#include <list>
#include <vector>
#include <map>

#include <cstdlib>
#include <array>
#include "CircularBuffer.h"
#include "dtmf_generator.h"

//#define BUFFER_AUDIO_SIZE_RTP (320 * 2)
#define BUFFER_AUDIO_SIZE_RTP (640 * 2)
#define COUNTER_RTP_MIX 40
#define COUNTER_EMPTY_SSRC 100 //100мс нету данных от устройства

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "cmsis_os.h"


#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

PACK_STRUCT_BEGIN
struct rtp_hdr {
    PACK_STRUCT_FIELD(uint8_t  version);
    PACK_STRUCT_FIELD(uint8_t  payloadtype);
    PACK_STRUCT_FIELD(uint16_t seqNum);
    PACK_STRUCT_FIELD(uint32_t timestamp);
    PACK_STRUCT_FIELD(uint32_t ssrc);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

typedef  struct {
    rtp_hdr header;
    uint16_t payload[BUFFER_AUDIO_SIZE_RTP / 2 ];
} RtpPackages;

/*!
 \brief Error codes

 \enum ERROR_CODE
*/
enum ErrorCode {
    FAIL_RTP = -1,
    OK_RTP = 0
};

enum class RingToneType {
    RING_TONE,
    RING_BACK_TONE,
    RING_BACK_BUSY_TONE,
    RING_UNKNOWN_TONE
};

/*!
 \brief This function creates an rtp stream for sending and receiving data.

 \fn rtpCreate
 \param port The port on which the rtp stream is created
 \param type Type of connection created
 \return ERROR_CODE Error code
*/
ErrorCode rtpCreate(uint32_t port, uint32_t type);

/*!
 \brief Delete current rtp stream

 \fn rtpRemove
 \return ERROR_CODE Error code
*/
ErrorCode rtpRemove();

#ifdef __cplusplus
}
#endif

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class SAI SAI.h "SAI.h"
*/

class alignas(4) SAI
{
private:
    static SAI *pInstance;

    /*!
     \brief Base constructor. A default constructor.

     \fn SAI
    */
    SAI();

    /*!
     \brief Copy constructor.

     \fn SAI
     \param
    */
    SAI(const SAI &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return SAI &operator
    */
    SAI &operator=(SAI &);


public:



    static SAI *getInstance();

    //const for audio size

    //(16000 * 32bit / 8) * 0.02 (20ms) / 2 (uint16_t)
    //64kB -> 1s
    //1280B -> 20ms
    //640 uin16_t -> 20ms
    //320 uin16_t -> 10ms

    osThreadId threadAudioRxFullId; /* Thread ID */
    osThreadId threadAudioRxHalfId; /* Thread ID */

    osThreadId threadAudioTxId; /* Thread ID */
    osThreadId threadAudioTxFullId; /* Thread ID */
    osThreadId threadAudioTxHalfId; /* Thread ID */
    osThreadId threadAudioInitId; /* Thread ID */


    CircularBuffer <RtpPackages, 5> outRingBuffer;
    CircularBuffer <RtpPackages, 10> tmpRingBuffer;
//    CircularBuffer <RtpPackages, COUNTER_RTP_MIX> outMixRingBuffer;
    std::array <RtpPackages, COUNTER_RTP_MIX> outMixRingBuffer;
    uint32_t outMixRingBufferPosition {0};
    std::list <uint32_t> ssrc;
    //ssrc, counter
    std::map <uint32_t, uint16_t> ssrcCounterPackages;


    SAI_HandleTypeDef *hOutSai;
    SAI_HandleTypeDef *hInSai;

    volatile bool cryptTxComplete = false; /*! Flag crypto tx complete */
    volatile bool cryptRxComplete = false; /*! Flag crypto rx complete */

    constexpr static uint16_t ringBackToneArray_len = 32000;

    osStatus ringToneStatus = osErrorOS; /*!< The variable that stores a status of the ring back tone*/

    DTMF tone; /*!< The object of the DTMF class to generate a DTMF tone for DTMF typing*/






};



#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief saiInit Initialization for SAI bus
 */
void saiInitAudio(void);

/*!
 \brief Audio Initialization Stream

 \fn threadAudioInit
 \param arg Doesn't use
*/
void threadAudioInit(void const *arg);

/*!
 \brief This thread takes data from the ring buffer and writes it to the first half of the dma channel for audio, the data is already decrypted

 \fn threadAudioTxHalf
 \param arg Doesn't use
*/
void threadAudioTxHalf(void const *arg);

/*!
 \brief This thread takes data from the ring buffer and writes it to the second half of the dma channel for audio, the data is already decrypted

 \fn threadAudioTxFull
 \param arg Doesn't use
*/
void threadAudioTxFull(void const *arg);
//[[noreturn]] void threadAudioTx(void const *arg);

/*!
 \brief This stream reads from the second half of the audio data and sends a signal that the RTP stream can be transmitted.

 \fn threadAudioRxFull
 \param arg Doesn't use
*/
void threadAudioRxFull(void const *arg);
/*!
 \brief This stream reads from the first half of the audio data and sends a signal that the RTP stream can be transmitted.

\fn threadAudioRxFull
 \param arg Doesn't use
        */
void threadAudioRxHalf(void const *arg);

/*!
 \brief The function is used to start the ring tone and the ring back tone.

\fn startRingTone
\param type is used to send a type of the ring tone to the function
*/
void startRingTone(const RingToneType type);
/*!
 \brief The function is used to stop the ring back tone.

 \fn stopRingBackTone
*/
void stopRingTone(void);
/*!
 \brief The function is used to start a DTMF tone.

\fn startDtmfTone
\param keyNum is used to send a key number to the function which used to create a tone.
*/
void startDtmfTone(uint8_t keyNum);

/*!
 \brief The function is used to stop a DTMF tone.

\fn stopDtmfTone
*/
void stopDtmfTone();


/*!
 \brief This stream reads from the first half of the audio data and sends a signal that the RTP stream can be transmitted.

 \fn threadAudioRxHalf
 \param arg Doesn't use
*/
void threadAudioRxHalf(void const *arg);

void ringToneTimer_Callback(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // SAI_H
