#include <algorithm>
#include <cstring>
#include <array>
#include <cstdint>
#include "../Debug/debug.h"
#include "sai.h"
#include "ringBack.h"

#include "i2c.h"
#include "tlv320aic3254.h"
#include "rtp.h"
#include "net_sockets.h"
#include <cstdlib>

#include <cmath>
#include "arm_math.h"

#define PI 3.141592653589793

osTimerDef (ringToneTimer, ringToneTimer_Callback);

static uint8_t saveCurrentTypeOfRtp = 0;
static uint8_t timerCount = 0;
static osTimerId ringToneTimer_id;
static RingToneType toneType = RingToneType::RING_TONE;

#ifdef __cplusplus
extern "C" {
#endif

_Noreturn void timerForMixAudio(void const *arg);                     // prototypes for timer callback function

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyCRYP[4] __ALIGN_END = {
    0x12345678, 0x12345678, 0x12345678, 0x12345678
};

DMA_HandleTypeDef hdmaCrypIn;
DMA_HandleTypeDef hdmaCrypOut;

SAI_HandleTypeDef audioTxSai;
SAI_HandleTypeDef audioRxSai;

static DMA_HandleTypeDef hdmaSaiRx;
static DMA_HandleTypeDef hdmaSaiTx;

static osSemaphoreId semaphoreRxFullId;
static osSemaphoreId semaphoreRxHalfId;

static osSemaphoreId semaphoreTxFullId;
static osSemaphoreId semaphoreTxHalfId;

osMutexId mutexRtpRxId;
osMutexId mutexMixRtpRxId;
osMutexId mutexRtpTxId;

osMutexId mutexRtpThreadCreatingId;

osMutexId mutexCryptTxId;
osMutexId mutexCryptRxId;

static osSemaphoreId semaphoreRtpCreateId;

osThreadId sendThreadHalfId;
osThreadId sendThreadFullId;
osThreadId sendThreadMixAudioId;
extern osThreadId recvThreadId;

extern osThreadId lostPackThreadId;

static uint16_t rxBuf[BUFFER_AUDIO_SIZE_RTP] = {0};
static uint16_t txBuf[BUFFER_AUDIO_SIZE_RTP] = {0};

/**
  * @brief CRYP Initialization Function
  * @param None
  * @retval None
  */
static void crypInit(uint32_t *key)
{
    /* DMA2_Stream5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
    /* DMA2_Stream6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

    hcryp.Instance = CRYP;
    hcryp.Init.DataType = CRYP_DATATYPE_32B;
    hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
    hcryp.Init.pKey = key;
    hcryp.Init.Algorithm = CRYP_AES_ECB;
    hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
    if (HAL_CRYP_Init(&hcryp) != HAL_OK) {
        Debug::getInstance().dbg << "crypInit -> ERROR\n";
    }
}

#ifdef __cplusplus
}
#endif

SAI::SAI()
{
    hOutSai = &audioTxSai;
    hInSai = &audioRxSai;

    if ((ringToneTimer_id = osTimerCreate (osTimer(ringToneTimer), osTimerPeriodic, nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

SAI *SAI::getInstance()
{
    if (!pInstance) pInstance = new SAI();
    return pInstance;
}

SAI *SAI::pInstance = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

extern void sendHalfThread(void const *arg);
extern void sendFullThread(void const *arg);

extern uint8_t rtpDataTxHalf[BUFFER_AUDIO_SIZE_RTP];
extern uint8_t rtpDataTxFull[BUFFER_AUDIO_SIZE_RTP];

uint8_t rtpDataTxHalfCrypt[BUFFER_AUDIO_SIZE_RTP];
uint8_t rtpDataTxFullCrypt[BUFFER_AUDIO_SIZE_RTP];

uint8_t rtpDataRxHalfCrypt[BUFFER_AUDIO_SIZE_RTP];
uint8_t rtpDataRxFullCrypt[BUFFER_AUDIO_SIZE_RTP];

/*!
  \brief SAI Initialization Function
  \param None
  \retval None
  */
void saiInitAudio(void)
{
    /* Enable DMA controller clock for SAI */

    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    /* DMA2_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

    audioTxSai.Instance = SAI1_Block_A;
    audioTxSai.Init.AudioMode = SAI_MODEMASTER_TX;
    audioTxSai.Init.Synchro = SAI_ASYNCHRONOUS;
    audioTxSai.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
    audioTxSai.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
    audioTxSai.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
    audioTxSai.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_16K;
    audioTxSai.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
    audioTxSai.Init.MonoStereoMode = SAI_STEREOMODE;
    audioTxSai.Init.CompandingMode = SAI_NOCOMPANDING;
    audioTxSai.Init.TriState = SAI_OUTPUT_NOTRELEASED;
    if (HAL_SAI_InitProtocol(&audioTxSai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK) {
        Debug::getInstance().dbg << "HAL_SAI_InitProtocol(&haudioOutSai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) -> ERROR\n";
    }

    audioRxSai.Instance = SAI1_Block_B;
    audioRxSai.Init.AudioMode = SAI_MODESLAVE_RX;
    audioRxSai.Init.Synchro = SAI_SYNCHRONOUS;
    audioRxSai.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
    audioRxSai.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
    audioRxSai.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
    audioRxSai.Init.MonoStereoMode = SAI_STEREOMODE;
    audioRxSai.Init.CompandingMode = SAI_NOCOMPANDING;
    audioRxSai.Init.TriState = SAI_OUTPUT_NOTRELEASED;
    if (HAL_SAI_InitProtocol(&audioRxSai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK) {
        Debug::getInstance().dbg << "HAL_SAI_InitProtocol(&haudioInSai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) -> ERROR\n";
    }
}

/**
  * @brief This function handles SAI1 global interrupt.
  */
void SAI1_IRQHandler(void)
{
    HAL_SAI_IRQHandler(&audioRxSai);
    HAL_SAI_IRQHandler(&audioTxSai);
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdmaSaiTx);
}

/**
  * @brief This function handles DMA2 stream4 global interrupt.
  */
void DMA2_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdmaSaiRx);
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hOutSai)
{
    if (hOutSai->Instance == SAI1_Block_A) {
        osSemaphoreRelease(semaphoreTxFullId);
    }
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hOutSai)
{
    if (hOutSai->Instance == SAI1_Block_A) {
        osSemaphoreRelease(semaphoreTxHalfId);
    }
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hInSai)
{
    if (hInSai->Instance == SAI1_Block_B) {
        osSemaphoreRelease(semaphoreRxFullId);
    }
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hInSai)
{
    if (hInSai->Instance == SAI1_Block_B) {
        osSemaphoreRelease(semaphoreRxHalfId);
    }
}

static uint32_t SAI1_client = 0;

void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* SAI1 */
    if (hsai->Instance == SAI1_Block_A) {
        /* Peripheral clock enable */
        if (SAI1_client == 0) {
            __HAL_RCC_SAI1_CLK_ENABLE();
        }

        /* Peripheral interrupt init*/
        HAL_NVIC_SetPriority(SAI1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SAI1_IRQn);

        SAI1_client ++;

        /**SAI1_A_Block_A GPIO Configuration
        PE2     ------> SAI1_MCLK_A
        PE4     ------> SAI1_FS_A
        PE5     ------> SAI1_SCK_A
        PE6     ------> SAI1_SD_A
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* Peripheral DMA init*/

        hdmaSaiTx.Instance = DMA2_Stream3;
        hdmaSaiTx.Init.Channel = DMA_CHANNEL_0;
        hdmaSaiTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdmaSaiTx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdmaSaiTx.Init.MemInc = DMA_MINC_ENABLE;
        hdmaSaiTx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdmaSaiTx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdmaSaiTx.Init.Mode = DMA_CIRCULAR;
        hdmaSaiTx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
        hdmaSaiTx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdmaSaiTx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdmaSaiTx.Init.MemBurst = DMA_MBURST_INC8;
        hdmaSaiTx.Init.PeriphBurst = DMA_PBURST_SINGLE;

        if (HAL_DMA_Init(&hdmaSaiTx) != HAL_OK) {
            Debug::getInstance().dbg << "HAL_DMA_Init(&hdmaSaiTx) -> ERROR\n";
        }

        /* Several peripheral DMA handle pointers point to the same DMA handle.
         Be aware that there is only one stream to perform all the requested DMAs. */
        __HAL_LINKDMA(hsai, hdmarx, hdmaSaiTx);

        __HAL_LINKDMA(hsai, hdmatx, hdmaSaiTx);

    }
    if (hsai->Instance == SAI1_Block_B) {
        /* Peripheral clock enable */
        if (SAI1_client == 0) {
            __HAL_RCC_SAI1_CLK_ENABLE();

            /* Peripheral interrupt init*/
            HAL_NVIC_SetPriority(SAI1_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(SAI1_IRQn);
        }
        SAI1_client ++;

        /**SAI1_B_Block_B GPIO Configuration
        PE3     ------> SAI1_SD_B
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* Peripheral DMA init*/
        hdmaSaiRx.Instance = DMA2_Stream4;
        hdmaSaiRx.Init.Channel = DMA_CHANNEL_1;
        hdmaSaiRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdmaSaiRx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdmaSaiRx.Init.MemInc = DMA_MINC_ENABLE;
        hdmaSaiRx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdmaSaiRx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdmaSaiRx.Init.Mode = DMA_CIRCULAR;
        hdmaSaiRx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
        hdmaSaiRx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdmaSaiRx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdmaSaiRx.Init.MemBurst = DMA_MBURST_INC8;
        hdmaSaiRx.Init.PeriphBurst = DMA_PBURST_SINGLE;

        if (HAL_DMA_Init(&hdmaSaiRx) != HAL_OK) {
            Debug::getInstance().dbg << "HAL_DMA_Init(&hdmaSaiRx) -> ERROR\n";
        }

        /* Several peripheral DMA handle pointers point to the same DMA handle.
         Be aware that there is only one stream to perform all the requested DMAs. */
        __HAL_LINKDMA(hsai, hdmarx, hdmaSaiRx);
        __HAL_LINKDMA(hsai, hdmatx, hdmaSaiRx);
    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef *hsai)
{

    Debug::getInstance().dbg << __FUNCTION__ <<  " " << __LINE__ << "\n";
    /* SAI1 */
    if (hsai->Instance == SAI1_Block_A) {
        SAI1_client --;
        if (SAI1_client == 0) {
            /* Peripheral clock disable */
            __HAL_RCC_SAI1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SAI1_IRQn);
        }

        /**SAI1_A_Block_A GPIO Configuration
        PE2     ------> SAI1_MCLK_A
        PE4     ------> SAI1_FS_A
        PE5     ------> SAI1_SCK_A
        PE6     ------> SAI1_SD_A
        */
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

        HAL_DMA_DeInit(hsai->hdmarx);
        HAL_DMA_DeInit(hsai->hdmatx);
    }

    if (hsai->Instance == SAI1_Block_B) {
        SAI1_client --;
        if (SAI1_client == 0) {
            /* Peripheral clock disable */
            __HAL_RCC_SAI1_CLK_DISABLE();
            HAL_NVIC_DisableIRQ(SAI1_IRQn);
        }

        /**SAI1_B_Block_B GPIO Configuration
        PE3     ------> SAI1_SD_B
        */
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3);

        HAL_DMA_DeInit(hsai->hdmarx);
        HAL_DMA_DeInit(hsai->hdmatx);
    }
}

osThreadDef(recvThread, rtpRecvThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 40);
osThreadDef(audioTxHalfThread, threadAudioTxHalf, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
osThreadDef(audioTxFullThread, threadAudioTxFull, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
osThreadDef(handelMixAudio, timerForMixAudio, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 40);
osThreadDef(audioRxFullThread, threadAudioRxFull, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 20);
osThreadDef(audioRxHalfThread, threadAudioRxHalf, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 20);
osThreadDef(sendHalfRtp, sendHalfThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 5);
osThreadDef(sendFullRtp, sendFullThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 5);
osThreadDef(lostPackThread, lostPackThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);

void threadAudioInit(void const *arg)
{
    UNUSED(arg);

    constexpr uint32_t TIME_OUT = 10;

    crypInit((uint32_t *)pKeyCRYP);

    //Audio full initialization
    i2cInitAudio();
    saiInitAudio();

    TLV320AIC3254 codec;
    codec.codecInit();

    osMutexDef(mutexRtpRx);
    mutexRtpRxId = osMutexCreate(osMutex(mutexRtpRx));

    osMutexDef(mutexMixRtpRx);
    mutexMixRtpRxId = osMutexCreate(osMutex(mutexMixRtpRx));

    osMutexDef(mutexRtpTx);
    mutexRtpTxId = osMutexCreate(osMutex(mutexRtpTx));

    osMutexDef(mutexCryptTx);
    mutexCryptTxId = osMutexCreate(osMutex(mutexCryptTx));

    osMutexDef(mutexCryptRx);
    mutexCryptRxId = osMutexCreate(osMutex(mutexCryptRx));

    osSemaphoreDef(semaphoreRtpCreate);
    semaphoreRtpCreateId = osSemaphoreCreate(osSemaphore(semaphoreRtpCreate), 1);

//    osSemaphoreDef(semaphoreRtpTx);
//    semaphoreRtpTxId = osSemaphoreCreate(osSemaphore(semaphoreRtpTx), 1);

    osSemaphoreDef(semaphoreRxFull);
    semaphoreRxFullId = osSemaphoreCreate(osSemaphore(semaphoreRxFull), 1);

    osSemaphoreDef(semaphoreRxHalf);
    semaphoreRxHalfId = osSemaphoreCreate(osSemaphore(semaphoreRxHalf), 1);

    osSemaphoreDef(semaphoreTxFull);
    semaphoreTxFullId = osSemaphoreCreate(osSemaphore(semaphoreTxFull), 1);

    osSemaphoreDef(semaphoreTxHalf);
    semaphoreTxHalfId = osSemaphoreCreate(osSemaphore(semaphoreTxHalf), 1);

    do {
        sendThreadHalfId = osThreadCreate(osThread(sendHalfRtp), nullptr);
        if (sendThreadHalfId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (sendThreadHalfId == nullptr);

    do {
        sendThreadFullId = osThreadCreate(osThread(sendFullRtp), nullptr);
        if (sendThreadFullId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (sendThreadFullId == nullptr);



    do {
        SAI::getInstance()->threadAudioTxHalfId = osThreadCreate(osThread(audioTxHalfThread), nullptr);
        if (SAI::getInstance()->threadAudioTxHalfId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (SAI::getInstance()->threadAudioTxHalfId == nullptr);

    do {
        SAI::getInstance()->threadAudioTxFullId = osThreadCreate(osThread(audioTxFullThread), nullptr);
        if (SAI::getInstance()->threadAudioTxFullId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (SAI::getInstance()->threadAudioTxFullId == nullptr);

    do {
        sendThreadMixAudioId = osThreadCreate(osThread(handelMixAudio), nullptr);
        if (sendThreadMixAudioId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (sendThreadMixAudioId == nullptr);

    do {
        SAI::getInstance()->threadAudioRxFullId = osThreadCreate(osThread(audioRxFullThread), nullptr);
        if (SAI::getInstance()->threadAudioRxFullId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (SAI::getInstance()->threadAudioRxFullId == nullptr) ;

    do {
        SAI::getInstance()->threadAudioRxHalfId = osThreadCreate(osThread(audioRxHalfThread), nullptr);
        if (SAI::getInstance()->threadAudioRxHalfId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (SAI::getInstance()->threadAudioRxHalfId == nullptr);

    do {
        lostPackThreadId = osThreadCreate(osThread(lostPackThread), nullptr);
        if (lostPackThreadId == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
            Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
            osDelay(TIME_OUT);
        }
    } while (lostPackThreadId == nullptr);

//    rtpCreate(100, 2);

    while (1) {
        osThreadTerminate(SAI::getInstance()->threadAudioInitId);
    }
}

void threadAudioTxHalf(void const *arg)
{
    UNUSED(arg);
    RtpPackages in;
    while (1) {
        if (osSemaphoreWait(semaphoreTxHalfId, 0) == osOK) {
            osMutexWait(mutexRtpRxId, osWaitForever);
            if (!SAI::getInstance()->outRingBuffer.isEmpty()) {

                in = SAI::getInstance()->outRingBuffer.first();
                SAI::getInstance()->outRingBuffer.shift();
                osMutexRelease(mutexRtpRxId);

//                memcpy(reinterpret_cast<uint8_t *>(txBuf), reinterpret_cast<uint8_t *>(in.payload), BUFFER_AUDIO_SIZE_RTP);
                arm_copy_q15(reinterpret_cast<q15_t *>(in.payload), reinterpret_cast<q15_t *>(txBuf), BUFFER_AUDIO_SIZE_RTP / 2);
            } else if (SAI::getInstance()->tone.status == DTMF::Status::START) {
                SAI::getInstance()->tone.getData(DTMF::Control::FIRST_HALF);
                osMutexRelease(mutexRtpRxId);
            } else {
                osMutexRelease(mutexRtpRxId);
                osDelay(1);
            }
        } else {
            osDelay(1);
        }
    }
}

void threadAudioTxFull(void const *arg)
{
    UNUSED(arg);
    RtpPackages in;
    while (1) {
        if (osSemaphoreWait(semaphoreTxFullId, 0) == osOK) {
            osMutexWait(mutexRtpRxId, osWaitForever);
            if (!SAI::getInstance()->outRingBuffer.isEmpty()) {

                in = SAI::getInstance()->outRingBuffer.first();
                SAI::getInstance()->outRingBuffer.shift();
                osMutexRelease(mutexRtpRxId);

//                memcpy(reinterpret_cast<uint8_t *>(txBuf) + BUFFER_AUDIO_SIZE_RTP, reinterpret_cast<uint8_t *>(in.payload), BUFFER_AUDIO_SIZE_RTP);
                arm_copy_q15(reinterpret_cast<q15_t *>(in.payload), reinterpret_cast<q15_t *>(txBuf) + BUFFER_AUDIO_SIZE_RTP / 2, BUFFER_AUDIO_SIZE_RTP / 2);
            } else if (SAI::getInstance()->tone.status == DTMF::Status::START) {
                SAI::getInstance()->tone.getData(DTMF::Control::SECOND_HALF);
                osMutexRelease(mutexRtpRxId);
            } else {
                osMutexRelease(mutexRtpRxId);
                osDelay(1);
            }
        } else {
            osDelay(1);
        }
    }
}

void threadAudioRxFull(void const *arg)
{
    UNUSED(arg);
    while (1) {
        if (osSemaphoreWait(semaphoreRxFullId, 0) == osOK) {
//            memcpy(reinterpret_cast<uint8_t *>(rtpDataTxFullCrypt), reinterpret_cast<uint8_t *>(rxBuf) +  BUFFER_AUDIO_SIZE_RTP, BUFFER_AUDIO_SIZE_RTP);
//            memcpy(reinterpret_cast<uint8_t *>(rtpDataTxFull), reinterpret_cast<uint8_t *>(rxBuf) +  BUFFER_AUDIO_SIZE_RTP, BUFFER_AUDIO_SIZE_RTP);
            arm_copy_q15(reinterpret_cast<q15_t *>(rxBuf)  + BUFFER_AUDIO_SIZE_RTP / 2, reinterpret_cast<q15_t *>(rtpDataTxFullCrypt), BUFFER_AUDIO_SIZE_RTP / 2);


            osMutexWait(mutexCryptTxId, osWaitForever);
            HAL_CRYP_Encrypt_DMA(&hcryp, reinterpret_cast<uint32_t *>(rtpDataTxFullCrypt), BUFFER_AUDIO_SIZE_RTP / 4, reinterpret_cast<uint32_t *>(rtpDataTxFull));
            while (!SAI::getInstance()->cryptTxComplete);
            SAI::getInstance()->cryptTxComplete = false;
            osMutexRelease(mutexCryptTxId);

            osSignalSet(sendThreadFullId, 0x02);
        } else {
            osDelay(1);
        }
    }
}

void threadAudioRxHalf(void const *arg)
{
    UNUSED(arg);
    while (1) {
        if (osSemaphoreWait(semaphoreRxHalfId, 0) == osOK) {
//            memcpy(reinterpret_cast<uint8_t *>(rtpDataTxHalfCrypt), reinterpret_cast<uint8_t *>(rxBuf), BUFFER_AUDIO_SIZE_RTP);
//            memcpy(reinterpret_cast<uint8_t *>(rtpDataTxHalf), reinterpret_cast<uint8_t *>(rxBuf), BUFFER_AUDIO_SIZE_RTP);
            arm_copy_q15(reinterpret_cast<q15_t *>(rxBuf), reinterpret_cast<q15_t *>(rtpDataTxHalfCrypt), BUFFER_AUDIO_SIZE_RTP / 2);

            osMutexWait(mutexCryptTxId, osWaitForever);
            HAL_CRYP_Encrypt_DMA(&hcryp, reinterpret_cast<uint32_t *>(rtpDataTxHalfCrypt), BUFFER_AUDIO_SIZE_RTP / 4, reinterpret_cast<uint32_t *>(rtpDataTxHalf));
            while (!SAI::getInstance()->cryptTxComplete);
            SAI::getInstance()->cryptTxComplete = false;
            osMutexRelease(mutexCryptTxId);

            osSignalSet(sendThreadHalfId, 0x01);
        } else {
            osDelay(1);
        }
    }
}



//Simplex communication send, type = 0
//Simplex communication recv, type = 1
//Duplex communication, type = 2
//Simplex to send callback signal, type = 3
ErrorCode rtpCreate(uint32_t port, uint32_t type)
{
    constexpr uint32_t PORT_MIN = 100;
    constexpr uint32_t PORT_MAX = 65000;
    constexpr uint32_t TIME_OUT = 10;

    if ((port < PORT_MIN) || (port > PORT_MAX)) {
        return FAIL_RTP;
    }

    RtpPackages erase;

    //reset position
    SAI::getInstance()->outMixRingBufferPosition = 0;

    erase.header.ssrc = 0;
    erase.header.seqNum = 0;
//    erase.header.version = 0;
//    erase.header.timestamp = 0;
//    erase.header.payloadtype = 0;

//    std::fill(erase.payload, erase.payload + (BUFFER_AUDIO_SIZE_RTP / 2), 0);
//    //erase all buffers of the ring buffer for mixing data
    for(auto &i : SAI::getInstance()->outMixRingBuffer) {
        osMutexWait(mutexMixRtpRxId, osWaitForever);
        i = erase;
        SAI::getInstance()->outMixRingBufferPosition = (SAI::getInstance()->outMixRingBufferPosition + 1) % COUNTER_RTP_MIX;
        osMutexRelease(mutexMixRtpRxId);
    }

    //semaphore
    osSemaphoreWait(semaphoreRtpCreateId, osWaitForever);

    saveCurrentTypeOfRtp = static_cast<uint8_t>(type);

    //Send RTP, save SAI RX
    if ((saveCurrentTypeOfRtp == 0) || (saveCurrentTypeOfRtp == 2) /*|| (saveCurrentTypeOfRtp == 3)*/) {
        rtpSendInit(reinterpret_cast<void const *>(port));
    }

    if ((saveCurrentTypeOfRtp == 1) || (saveCurrentTypeOfRtp == 2)) {
        do {
            recvThreadId = osThreadCreate(osThread(recvThread), reinterpret_cast<void *>(port));
            if (recvThreadId == nullptr) {
                Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
                Debug::getInstance().dbg << "heap size: " << xPortGetFreeHeapSize()  << "\n";
                osDelay(TIME_OUT);
            }
        } while (recvThreadId == nullptr);
    }

//    if (saveCurrentTypeOfRtp != 3) {
    memset(txBuf, 0x00, sizeof (txBuf));
    memset(rxBuf, 0x00, sizeof (rxBuf));

    HAL_SAI_Transmit_DMA(&audioTxSai, reinterpret_cast<uint8_t *>(txBuf), BUFFER_AUDIO_SIZE_RTP);
    HAL_SAI_Receive_DMA(&audioRxSai, reinterpret_cast<uint8_t *>(rxBuf), BUFFER_AUDIO_SIZE_RTP);

//    }

    return OK_RTP;
}

extern int               sockRtpRecv;
extern struct ip_mreq    ipmreqRtpRecv;
extern RTP_HandleTypeDef rtpStructSend;         /* RTP structure */

ErrorCode rtpRemove()
{
//    if (saveCurrentTypeOfRtp != 3) {
    HAL_SAI_DMAStop(&audioTxSai);
    HAL_SAI_DMAStop(&audioRxSai);
//    }

    if ((saveCurrentTypeOfRtp == 0) || (saveCurrentTypeOfRtp == 2)) {

        osMutexWait(mutexRtpTxId, osWaitForever);
        rtpStructSend.State = RTP_STATE_IDLE;
        close(rtpStructSend.sock_id);

        SAI::getInstance()->ssrcCounterPackages.clear();
        SAI::getInstance()->ssrc.clear();


        osMutexRelease(mutexRtpTxId);
    }

    if ((saveCurrentTypeOfRtp == 1) || (saveCurrentTypeOfRtp == 2)) {

        osMutexWait(mutexMixRtpRxId, osWaitForever);
        osThreadTerminate(recvThreadId);

        /* leave multicast group */
        setsockopt(sockRtpRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, &ipmreqRtpRecv, sizeof(ipmreqRtpRecv));
        /* close the socket */
        closesocket(sockRtpRecv);

        SAI::getInstance()->ssrcCounterPackages.clear();
        SAI::getInstance()->ssrc.clear();

        osMutexRelease(mutexMixRtpRxId);
    }

    SAI::getInstance()->outRingBuffer.clear();

    osDelay(100);
    osSemaphoreRelease(semaphoreRtpCreateId);
    return OK_RTP;
}

void startDtmfTone(uint8_t keyNum)
{
    if (keyNum < 10) {
        if (SAI::getInstance()->tone.getFrequency(keyNum) == DTMF::Status::READY)
            HAL_SAI_Transmit_DMA(&audioTxSai, reinterpret_cast<uint8_t *> (SAI::getInstance()->tone.getData(DTMF::Control::FULL)), SAI::getInstance()->tone.TABLESIZE*2);
    }
}

void stopDtmfTone()
{
    if (SAI::getInstance()->tone.status == DTMF::Status::START) {
        HAL_SAI_DMAStop(&audioTxSai);
        SAI::getInstance()->tone.status = DTMF::Status::IDLE;
    }
}

void startRingTone(const RingToneType type)
{
    toneType = type;
    uint16_t delay = 0;
    uint8_t* buff = nullptr;
    uint16_t buffSize = 0;
    switch (type) {
    case RingToneType::RING_TONE:
        delay = 1000;
        buff = zvon3_raw;
        buffSize = zvon3_raw_len;
        break;
    case RingToneType::RING_BACK_TONE:
        delay = 1000;
        buff = ringBackToneArray_raw;
        buffSize = ringBackToneArray_raw_length;
        break;
    case RingToneType::RING_BACK_BUSY_TONE:
        delay = 350;
        buff = ringBackToneArray_raw;
        buffSize = ringBackToneArray_raw_length;
        break;
    case RingToneType::RING_UNKNOWN_TONE:
        startDtmfTone(1);
        break;
    default:
        break;
    }

    if (type != RingToneType::RING_UNKNOWN_TONE) {
        timerCount = 0;
        SAI::getInstance()->ringToneStatus = osTimerStart (ringToneTimer_id, delay); // timer starting
        HAL_SAI_Transmit_DMA(&audioTxSai, buff, buffSize/2);

        if (SAI::getInstance()->ringToneStatus != osOK)  {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
    }

}

void stopRingTone(void)
{
    if (toneType == RingToneType::RING_UNKNOWN_TONE) {
        stopDtmfTone();
    } else if (SAI::getInstance()->ringToneStatus == osOK) {
        osTimerStop(ringToneTimer_id);
        HAL_SAI_DMAStop(&audioTxSai);
        SAI::getInstance()->ringToneStatus = osErrorOS;
    }
}

void ringToneTimer_Callback(void const *arg)
{
    uint8_t control = *(uint8_t*)arg;
    (void)arg;
    switch (toneType) {
    case RingToneType::RING_TONE:
        if (!timerCount) {
            HAL_SAI_DMAStop(&audioTxSai);
            timerCount++;
        } else {
            timerCount = 0;
            HAL_SAI_Transmit_DMA(&audioTxSai, zvon3_raw, zvon3_raw_len/2);
        }
        break;
    case RingToneType::RING_BACK_TONE:
        timerCount++;
        if (timerCount < 2) {
            HAL_SAI_DMAStop(&audioTxSai);
        } else if (timerCount > 3) {
            timerCount = 0;
            HAL_SAI_Transmit_DMA(&audioTxSai, ringBackToneArray_raw, ringBackToneArray_raw_length/2);
        }
        break;
    case RingToneType::RING_BACK_BUSY_TONE:
        if (!timerCount) {
            HAL_SAI_DMAStop(&audioTxSai);
            timerCount++;
        } else {
            timerCount = 0;
            HAL_SAI_Transmit_DMA(&audioTxSai, ringBackToneArray_raw, ringBackToneArray_raw_length/2);
        }
        break;
    default:
        break;
    }
}

#ifdef __cplusplus
}
#endif
