/*!
 \file rtc.cpp

 \authors Androshchuk A.V.

 \code
    SNTP_Start();

    ...

    while (1) {

        sRTC::getInstance()->test();
    }

 \endcode

*/
#include "rtc.h"
#include "rtc_set_time.h"
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RTC handler declaration */
static RTC_HandleTypeDef hrtc;

/* Buffers used for displaying Time and Date */
//static uint8_t aShowTime[50] = {0};
//static uint8_t aShowDate[50] = {0};

/*!
  \brief RTC Initialization Function
  \param None
  \retval None
  */
void RTCInit()
{
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 0x7F;
    hrtc.Init.SynchPrediv = 0x00FF;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&hrtc) != HAL_OK) {
        /* Initialization Error */
        Debug::getInstance().dbg << "RTC Init Error!" << "\n";
    }
}

#ifdef __cplusplus
}
#endif

sRTC::sRTC()
{
    rtcHandle = &hrtc;
}

sRTC *sRTC::getInstance()
{
    if(!pInstance) pInstance = new sRTC();
    return pInstance;
}

sRTC *sRTC::pInstance = nullptr;

void sRTC::RTC_CalendarConfig()
{
    time_t tim = rawtime + 60UL*60*3;
    struct tm * timeinfo;

    timeinfo = localtime(&tim);

    /*##-1- Configure the Date #################################################*/
    sDateStruct.Year = (uint8_t)timeinfo->tm_year % 100;
    sDateStruct.Month = timeinfo->tm_mon + 1;
    sDateStruct.Date = timeinfo->tm_mday;
    sDateStruct.WeekDay = timeinfo->tm_wday;

    if(HAL_RTC_SetDate(rtcHandle, &sDateStruct, RTC_FORMAT_BIN) != HAL_OK) {
        /* Initialization Error */
        Debug::getInstance().dbg << "RTC Data Set Error!" << "\n";
    }

    /*##-2- Configure the Time #################################################*/
    sTimeStruct.Hours = timeinfo->tm_hour;
    sTimeStruct.Minutes = timeinfo->tm_min;
    sTimeStruct.Seconds = timeinfo->tm_sec;
    sTimeStruct.TimeFormat = RTC_HOURFORMAT12_AM;
    sTimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    sTimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(rtcHandle, &sTimeStruct, RTC_FORMAT_BIN) != HAL_OK) {
        /* Initialization Error */
        Debug::getInstance().dbg << "RTC Time Set Error!" << "\n";
    }

    /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
    HAL_RTCEx_BKUPWrite(rtcHandle, RTC_BKP_DR1, 0x32F2);
}

void sRTC::RTC_CalendarShow()
{
    RTC_DateTypeDef sDateStructGet;
    RTC_TimeTypeDef sTimeStructGet;

    /* Get the RTC current Time */
    HAL_RTC_GetTime(rtcHandle, &sTimeStructGet, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(rtcHandle, &sDateStructGet, RTC_FORMAT_BIN);
    /* Display time Format : hh:mm:ss */
    Debug::getInstance().dbg << sTimeStructGet.Hours << " " << sTimeStructGet.Minutes << " " << sTimeStructGet.Seconds << " " << "\n";
    /* Display date Format : mm-dd-yy */
    Debug::getInstance().dbg << sDateStructGet.Date << " " << sDateStructGet.Month << " " << 2000 + sDateStructGet.Year << " " << "\n";
}

void sRTC::test(void)
{
    if (HAL_RTCEx_BKUPRead(rtcHandle, RTC_BKP_DR1) != 0x32F2) {
        /* Configure RTC Calendar */
        RTC_CalendarConfig();
    }
    RTC_CalendarShow();
}
#ifdef __cplusplus
extern "C" {
#endif

void TimeSet(time_t rx_time)
{
    sRTC::getInstance()->rawtime = rx_time;
}

/**
  * \brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * \param hrtc RTC handle pointer
  * \retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();


    /*##-2- Configure LSE as RTC clock source ###################################*/
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
    // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;

    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        /* Initialization Error */
        Debug::getInstance().dbg << "LSE Init Error!" << "\n";
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        /* Initialization Error */
        Debug::getInstance().dbg << "RTC/LSE clock Init Error!" << "\n";
    }

    /*##-3- Enable RTC peripheral Clocks #######################################*/
    __HAL_RCC_RTC_ENABLE();    /* Enable RTC Clock */
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_RTC_DISABLE();

    /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
    HAL_PWR_DisableBkUpAccess();
    __HAL_RCC_PWR_CLK_DISABLE();

}

#ifdef __cplusplus
}
#endif
