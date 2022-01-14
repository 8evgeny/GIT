#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
#include <MDR32F9Qx_uart.h>
//---------------------------------
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_usb_handlers.h"
//---------------------------------
#include "main.h"
//=================================
// Настройка порта В
//---------------------------------
#define LED1_ERROR	PORT_Pin_5
#define LED2_REC	PORT_Pin_6
#define BUZZER		PORT_Pin_7
#define RS			PORT_Pin_8
#define RW 			PORT_Pin_9
#define EN 			PORT_Pin_10

#define delay(T) for(i = T; i > 0; i--) // Определение функции задержки (см. примечение 1)
PORT_InitTypeDef PORTB_Init; // Объявление структуры, с помощью которой будет происходить инициализация порта
int i; // Глобальная переменная счетчика, которая используется в функции delay()

#define BUFFER_LENGTH 100
USB_Clock_TypeDef USB_Clock_InitStruct;
USB_DeviceBUSParam_TypeDef USB_DeviceBUSParam;

static uint8_t Buffer[BUFFER_LENGTH];                       // Буфер для USB
#ifdef USB_CDC_LINE_CODING_SUPPORTED                        // USB_CDC_LINE_CODING_SUPPORTED
static USB_CDC_LineCoding_TypeDef LineCoding;
#endif

static void Setup_CPU_Clock(void);
static void Setup_USB(void);
static void VCom_Configuration(void);


int main (int argc, char** argv) {
    // Включение тактования порта B
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
    // Объявляем номера ножек порта, которые настраиваются данной структурой
    PORTB_Init.PORT_Pin =
        LED1_ERROR |
        LED2_REC |
        BUZZER |
        RS |
        RW |
        EN;
    PORTB_Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
    PORTB_Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
    PORTB_Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
    PORT_Init(MDR_PORTB, &PORTB_Init);          // Инициализация порта D объявленной структурой


    VCom_Configuration();                                   // Конфигурация параметров виртуального COM порта
    /* CDC инициализация */
    USB_CDC_Init(Buffer, 1, SET);                           // инициализация Буфера длинной 1 байт
    Setup_CPU_Clock();                                      // Установка тактовой частоты микропроцессора
    Setup_USB();


    while(1){
 //       PORT_SetBits(MDR_PORTB, LED1_ERROR);   // Установка единицы
 //       delay(0xFFFF);                         // Задержка
 //       PORT_ResetBits(MDR_PORTB, LED1_ERROR); // Установка нуля

    }

}

/* Установка тактовой частоты процессора */
void Setup_CPU_Clock(void)
{
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);                      // Включение тактирования от внешнего кварцевого резонатора
    if (RST_CLK_HSEstatus() != SUCCESS)
    {
        while (1){}   //Если внешний источник тактовой частоты не запустился то переходим в бесконечный цикл
    }
    // Определение параметров схемы PLL, коэффициент деления внешнего источника тактовой частоты 1,
    //коэффициент умножения 10 (CPU_C1_SEL = HSE)
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    RST_CLK_CPU_PLLcmd(ENABLE);                              // Разрешение работы PLL
    if (RST_CLK_CPU_PLLstatus() != SUCCESS)
    {
        while (1){}   //Если внешний источник тактовой частоты не запустился то переходим в бесконечный цикл
    }
    //Выбираем источник тактовой частоты процессора см. Спецификация микроконтроллеров серии 1986ВЕ9х, К1986ВЕ9х,
    //К1986ВЕ92QI, К1986ВЕ92QC, К1986ВЕ91Н4 стр. 123 раздел Сигналы тактовой частоты
    /* CPU_C3_SEL = CPU_C2_SEL */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    /* CPU_C2_SEL = PLL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* HCLK_SEL = CPU_C3_SEL */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
}

void Setup_USB(void)
{
    RST_CLK_PCLKcmd(RST_CLK_PCLK_USB, ENABLE);                  // Включение тактирования схемы USB
    USB_Clock_InitStruct.USB_USBC1_Source = USB_C1HSEdiv2;      // Выбор источника тактирования USB
    USB_Clock_InitStruct.USB_PLLUSBMUL    = USB_PLLUSBMUL12;    // Выбор коэффициента умножения схемы PLL для USB
    USB_DeviceBUSParam.MODE = USB_SC_SCFSP_Full;                //Выбор режима USB FULL Speed
    USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_12Mb;               //Выбор скорости передачи данных 12мБайт в сек.
    USB_DeviceBUSParam.PULL = USB_HSCR_DP_PULLUP_Set;           // Подтягивание линии DP к питанию.
    USB_DeviceInit(&USB_Clock_InitStruct, &USB_DeviceBUSParam); // Инициализация USB с заданными параметрами
    USB_SetSIM(USB_SIS_Msk);                                    // Разрешение всех видов прерываний от USB
    USB_DevicePowerOn();                                        // Включение питания USB и разрешение передачи и приема данных
    #ifdef USB_INT_HANDLE_REQUIRED
    NVIC_EnableIRQ(USB_IRQn);                                   // Включение прерываний USB
    #endif /* USB_INT_HANDLE_REQUIRED */
    USB_DEVICE_HANDLE_RESET;
}

/* Задание конфигурации последовательной линии связи которую может прочитать хост*/
static void VCom_Configuration(void)
{
    #ifdef USB_CDC_LINE_CODING_SUPPORTED
    LineCoding.dwDTERate = 115200;
    LineCoding.bCharFormat = 0;
    LineCoding.bParityType = 0;
    LineCoding.bDataBits = 8;
    #endif /* USB_CDC_LINE_CODING_SUPPORTED */
}
/*USB_CDC_HANDLE_DATA_RECEIVE implementation - data echoing*/

// Данная процедура автоматически вызывается при приеме данных по USB.
USB_Result USB_CDC_RecieveData(uint8_t* Buffer, uint32_t Length)
{
    USB_Result result;
    /* Передача одного байта назад на устройство */
    result = USB_CDC_SendData(Buffer, Length);
    
//  PORT_SetBits(MDR_PORTB, BUZZER);
//  delay(0xFFFF);
//  PORT_ResetBits(MDR_PORTB, BUZZER);
    
PORT_SetBits(MDR_PORTB, LED2_REC);
delay(0xFFFF);
PORT_ResetBits(MDR_PORTB, LED2_REC);
    
    return USB_SUCCESS;
}
#ifdef USB_CDC_LINE_CODING_SUPPORTED
/* Эти два запроса отправляются хостом, чтобы изменить или прочитать конфигурацию последовательной линии связи, которая включает в себя:
• Baudrate (скорость передачи данных)
• Number of stop bits (количество стоп-битов)
• Parity check (наличие бита контроля четности)
• Number of data bits (количество бит данных)
Когда программа терминала (ПО хоста наподобие HyperTerminal) меняет установки COM-порта, то
отправляется запрос SetLineCoding с новыми параметрами. Хост может также запросить текущие
настройки запросом GetLineCoding, и не менять их, если настройки корректны. */

/* USB_CDC_HANDLE_GET_LINE_CODING implementation example */
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

/* USB_CDC_HANDLE_SET_LINE_CODING implementation example */
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
