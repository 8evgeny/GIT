#include "usb.h"
#include "main.h"
#include <string>

USB_Clock_TypeDef USB_Clock_InitStruct;
USB_DeviceBUSParam_TypeDef USB_DeviceBUSParam;

#ifdef USB_VCOM_SYNC
    volatile uint32_t PendingDataLength = 0;
#endif /* USB_VCOM_SYNC */

/* USB protocol debugging */
#ifdef USB_DEBUG_PROTO
    #define USB_DEBUG_NUM_PACKETS   100
    typedef struct {
        USB_SetupPacket_TypeDef packet;
        uint32_t address;
    } TDebugInfo;
    static TDebugInfo SetupPackets[USB_DEBUG_NUM_PACKETS];
    static uint32_t SPIndex;
#endif /* USB_DEBUG_PROTO */

void Setup_CPU_Clock(void)
{
    /* Enable HSE */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);

    while(RST_CLK_HSEstatus() != SUCCESS)
    {
        delay_ms(100);
        LCD_write_string((char*)"Error CPU_Clock");
    }

    /* CPU_C1_SEL = HSE */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    RST_CLK_CPU_PLLcmd(ENABLE);

    while(RST_CLK_CPU_PLLstatus() != SUCCESS)
    {
        delay_ms(100);
        LCD_write_string((char*)"Error CPU_Clock");
    }

    /* CPU_C3_SEL = CPU_C2_SEL */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    /* CPU_C2_SEL = PLL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* HCLK_SEL = CPU_C3_SEL */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}

void Setup_USB(void)
{
    /* Включение тактирования */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_USB, ENABLE);

    /* Выбор источника тактирования USB */
    USB_Clock_InitStruct.USB_USBC1_Source = USB_C1HSEdiv2;

    /* Выбор коэффициента умножения схемы PLL для USB */
    USB_Clock_InitStruct.USB_PLLUSBMUL    = USB_PLLUSBMUL12;

    /* Выбор режима USB FULL Speed */
    USB_DeviceBUSParam.MODE  = USB_SC_SCFSP_Full;

    /* Выбор скорости 12мБит в сек */
    USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_12Mb;

    /* Подтягивание линии DP к питанию */
    USB_DeviceBUSParam.PULL  = USB_HSCR_DP_PULLUP_Set;

    /* Инициализация USB с заданными параметрами */
    USB_DeviceInit(&USB_Clock_InitStruct, &USB_DeviceBUSParam);

    /* Разрешение всех видов прерываний от USB */
    USB_SetSIM(USB_SIS_Msk);

    /* Включение питания USB и разрешение передачи и приема данных */
    USB_DevicePowerOn();

    /* Включение прерываний USB */
#ifdef USB_INT_HANDLE_REQUIRED
    NVIC_EnableIRQ(USB_IRQn);
#endif /* USB_INT_HANDLE_REQUIRED */

    USB_DEVICE_HANDLE_RESET;
}

/* Данная процедура автоматически вызывается при приеме данных по USB */
USB_Result USB_CDC_RecieveData(uint8_t* Buffer, uint32_t Length)
{
    USB_Result result;

#ifdef USB_DEBUG_PROTO
    ReceivedByteCount += Length;
#endif /* USB_DEBUG_PROTO */

    /* Передача (одного байта?) назад на устройство */
//    result = USB_CDC_SendData(Buffer, Length);

#ifdef USB_DEBUG_PROTO
    if (result == USB_SUCCESS)
    {
        SentByteCount += Length;
    }
#ifndef USB_VCOM_SYNC
    else
    {
        SkippedByteCount += Length;
    }
#endif /* !USB_VCOM_SYNC */
#endif /* USB_DEBUG_PROTO */

#ifdef USB_VCOM_SYNC
    if (result != USB_SUCCESS)
    {
        /* If data cannot be sent now, it will await nearest possibility
        * (see USB_CDC_DataSent) */
        PendingDataLength = Length;
    }
    return result;
#else
    return USB_SUCCESS;
#endif /* USB_VCOM_SYNC */
}

#ifdef USB_VCOM_SYNC

/**
  * @brief  USB_CDC_HANDLE_DATA_SENT implementation - sending of pending data
  * @param  None
  * @retval @ref USB_Result.
  */
USB_Result USB_CDC_DataSent(void)
{
    USB_Result result = USB_SUCCESS;

    if (PendingDataLength)
    {
        result = USB_CDC_SendData(Buffer, PendingDataLength);
#ifdef USB_DEBUG_PROTO
        if (result == USB_SUCCESS)
        {
            SentByteCount += PendingDataLength;
        }
        else
        {
            SkippedByteCount += PendingDataLength;
        }
#endif /* USB_DEBUG_PROTO */
        PendingDataLength = 0;
        USB_CDC_ReceiveStart();
    }
    return USB_SUCCESS;
}

#endif /* USB_VCOM_SYNC */

#ifdef USB_CDC_LINE_CODING_SUPPORTED

/**
  * @brief  USB_CDC_HANDLE_GET_LINE_CODING implementation example
  * @param  wINDEX: Request value 2nd word (wIndex)
  * @param  DATA: Pointer to the USB_CDC Line Coding Structure
  * @retval @ref USB_Result.
  */

USB_Result USB_CDC_GetLineCoding(uint16_t wINDEX, USB_CDC_LineCoding_TypeDef* DATA)
{
    assert_param(DATA);
    if (wINDEX != 0)
    {
        /* Invalid interface */
        return USB_ERR_INV_REQ;
    }

    /* Just store received settings */
    *DATA = LineCoding;
    return USB_SUCCESS;
}

/**
  * @brief  USB_CDC_HANDLE_SET_LINE_CODING implementation example
  * @param  wINDEX: Request value 2nd word (wIndex)
  * @param  DATA: Pointer to the USB_CDC Line Coding Structure
  * @retval @ref USB_Result.
  */
USB_Result USB_CDC_SetLineCoding(uint16_t wINDEX, const USB_CDC_LineCoding_TypeDef* DATA)
{
    assert_param(DATA);
    if (wINDEX != 0)
    {
        /* Invalid interface */
        return USB_ERR_INV_REQ;
    }

    /* Just send back settings stored earlier */
    LineCoding = *DATA;
    return USB_SUCCESS;
}

#endif /* USB_CDC_LINE_CODING_SUPPORTED */

#ifdef USB_DEBUG_PROTO

/**
  * @brief  Overwritten USB_DEVICE_HANDLE_SETUP default handler - to dump received setup packets
  * @param  EPx: USB Control EndPoint (EP0) number
  * @param  USB_SetupPacket: Pointer to a USB_SetupPacket_TypeDef structure
  *         that contains received setup packet contents (on success)
  * @retval @ref USB_Result.
  */
USB_Result USB_DeviceSetupPacket_Debug(USB_EP_TypeDef EPx, const USB_SetupPacket_TypeDef* USB_SetupPacket)
{

#ifdef USB_DEBUG_PROTO
    SetupPackets[SPIndex].packet = *USB_SetupPacket;
    SetupPackets[SPIndex].address = USB_GetSA();
    SPIndex = (SPIndex < USB_DEBUG_NUM_PACKETS ? SPIndex + 1 : 0);
#endif /* USB_DEBUG_PROTO */

    return USB_DeviceSetupPacket(EPx, USB_SetupPacket);
}

#endif /* USB_DEBUG_PROTO */

/**
  * @brief  Reports the source file ID, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file_id: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
    /* User can add his own implementation to report the source file ID and line number.
       Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr);
{
    /* User can add his own implementation to report the source file ID, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group USB_Virtual_COM_Port_Echo_92 */

/** @} */ /* End of group __MDR1986VE92_EVAL */

/** @} */ /* End of group __MDR32Fx_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr ********************************
*/
