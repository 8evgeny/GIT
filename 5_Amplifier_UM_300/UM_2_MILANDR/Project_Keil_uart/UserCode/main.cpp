#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"

#include "main.h"

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
//#include "MDR32F9Qx_eeprom.h"

static bool btnImpedance = 0;
static bool btnCalibrovka = 0;
static bool btnReset = 0;
static bool signalPowerOn = 0;
static bool signalImpedance = 0;
static bool signalTranslate = 0;
static bool input_IMP_UPR = 0;
static bool input_VOLT_UPR = 0;
static bool input_CUR_UPR1 = 0;
static bool input_CUR_UPR2 = 0;
static bool signalVnesh = 0;
static bool signalMic = 0;

#define fromStm_BtnImpedance_On     'Q'
#define fromStm_BtnImpedance_Off    'W'
#define fromStm_BtnCalibr_On        'E'
#define fromStm_BtnCalibr_Off       'R'
#define fromStm_BtnReset_On         'T'
#define fromStm_BtnReset_Off        'Y'
#define fromStm_SignalPowerOn_ON    'U'
#define fromStm_SignalPowerOn_OFF   'I'
#define fromStm_SignalImpedanse_ON  'O'
#define fromStm_SignalImpedanse_OFF 'P'
#define fromStm_SignalTranslate_ON  'A'
#define fromStm_SignalTranslate_OFF 'S'
#define fromStm_SignalFromOut_ON    'D'
#define fromStm_SignalFromOut_OFF   'F'
#define fromStm_SignalMic_ON        'G'
#define fromStm_SignalMic_OFF       'H'

#define toStm_BtnImpedance_On_OK     'q'
#define toStm_BtnImpedance_Off_OK    'w'
#define toStm_BtnCalibr_On_OK        'e'
#define toStm_BtnCalibr_Off_OK       'r'
#define toStm_BtnReset_On_OK         't'
#define toStm_BtnReset_Off_OK        'y'
#define toStm_SignalPowerOn_ON_OK    'u'
#define toStm_SignalPowerOn_OFF_OK   'i'
#define toStm_SignalImpedanse_ON_OK  'o'
#define toStm_SignalImpedanse_OFF_OK 'p'
#define toStm_SignalTranslate_ON_OK  'a'
#define toStm_SignalTranslate_OFF_OK 's'
#define toStm_SignalFromOut_ON_OK    'd'
#define toStm_SignalFromOut_OFF_OK   'f'
#define toStm_SignalMic_ON_OK        'g'
#define toStm_SignalMic_OFF_OK       'h'





#define UART_1
//#define UART_2

#ifdef UART_1
static PORT_InitTypeDef PortInitUART1;
static UART_InitTypeDef UART_InitStructure1;
#endif
#ifdef UART_2
static PORT_InitTypeDef PortInitUART2;
static UART_InitTypeDef UART_InitStructure2;
#endif
int main (void)
{
   RST_CLK_HSEconfig(RST_CLK_HSE_ON);
   while(RST_CLK_HSEstatus() != SUCCESS);

   /* Configures the CPU_PLL clock source */
   RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);

   /* Enables the CPU_PLL */
   RST_CLK_CPU_PLLcmd(ENABLE);
   if (RST_CLK_CPU_PLLstatus() == ERROR) {
      while (1);
   }

   /* Enables the RST_CLK_PCLK_EEPROM */
//   RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
   /* Sets the code latency value */

//   EEPROM_SetLatency(EEPROM_Latency_3);

   /* Select the CPU_PLL output as input for CPU_C3_SEL */
   RST_CLK_CPU_PLLuse(ENABLE);
   /* Set CPUClk Prescaler */
   RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

  /* Select the CPU clock source */
  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

  /* Enables the HSE clock on PORTB */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);

  /* Enables the HSE clock on PORTF */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF,ENABLE);

#ifdef UART_1
  /* Fill PortInit UART1 structure*/
  PortInitUART1.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInitUART1.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInitUART1.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInitUART1.PORT_PD = PORT_PD_DRIVER;
  PortInitUART1.PORT_GFEN = PORT_GFEN_OFF;
  PortInitUART1.PORT_FUNC = PORT_FUNC_ALTER;
  PortInitUART1.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInitUART1.PORT_MODE = PORT_MODE_DIGITAL;

  /* Configure PORTB pins 5 (UART1_TX) as output  */
  PortInitUART1.PORT_OE = PORT_OE_OUT;
  PortInitUART1.PORT_Pin = PORT_Pin_5;
  PORT_Init(MDR_PORTB, &PortInitUART1);
  /* Configure PORTB pins 6 (UART1_RX) as input  */
  PortInitUART1.PORT_OE = PORT_OE_IN;
  PortInitUART1.PORT_Pin = PORT_Pin_6;
  PORT_Init(MDR_PORTB, &PortInitUART1);
#endif

#ifdef UART_2
  /* Fill PortInit UART2 structure*/
  PortInitUART2.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInitUART2.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInitUART2.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInitUART2.PORT_PD = PORT_PD_DRIVER;
  PortInitUART2.PORT_GFEN = PORT_GFEN_OFF;
  PortInitUART2.PORT_FUNC = PORT_FUNC_OVERRID;
  PortInitUART2.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInitUART2.PORT_MODE = PORT_MODE_DIGITAL;

  //UART2 использую для прошивки в Linux
  /* Configure PORTF pins 1 (UART2_TX) as output */
  PortInitUART2.PORT_OE = PORT_OE_OUT;
  PortInitUART2.PORT_Pin = PORT_Pin_1;
  PORT_Init(MDR_PORTF, &PortInitUART2);
  /* Configure PORTF pins 0 (UART2_RX) as input */
  PortInitUART2.PORT_OE = PORT_OE_IN;
  PortInitUART2.PORT_Pin = PORT_Pin_0;
  PORT_Init(MDR_PORTF, &PortInitUART2);
#endif

  /* Select HSI/2 as CPU_CLK source*/
  //RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSIdiv2,0);
#ifdef UART_1
  /* Enables the CPU_CLK clock on UART1 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
  /* Set the HCLK division factor = 1 for UART1*/
  UART_BRGInit(MDR_UART1, UART_HCLKdiv1);

#endif
 #ifdef UART_2
  /* Enables the CPU_CLK clock on UART2 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);
  /* Set the HCLK division factor = 1 for UART2*/
  UART_BRGInit(MDR_UART2, UART_HCLKdiv1);

#endif

#ifdef UART_1
  /* Initialize UART_InitStructure1 */
  UART_InitStructure1.UART_BaudRate                = 19200;
  UART_InitStructure1.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure1.UART_StopBits                = UART_StopBits1;
  UART_InitStructure1.UART_Parity                  = UART_Parity_No;
  UART_InitStructure1.UART_FIFOMode                = UART_FIFO_ON;
  UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
  /* Configure UART1 parameters*/
  UART_Init (MDR_UART1,&UART_InitStructure1);

  /* Enables UART1 peripheral */
//  UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
//  UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи
  UART_Cmd(MDR_UART1,ENABLE);

#endif
#ifdef UART_2
  /* Initialize UART_InitStructure2 */
  UART_InitStructure2.UART_BaudRate                = 115000;
  UART_InitStructure2.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure2.UART_StopBits                = UART_StopBits1;
  UART_InitStructure2.UART_Parity                  = UART_Parity_No;
  UART_InitStructure2.UART_FIFOMode                = UART_FIFO_ON;
  UART_InitStructure2.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
  /* Configure UART2 parameters*/
  UART_Init (MDR_UART2,&UART_InitStructure2);

  /* Enables UART2 peripheral */
  UART_ITConfig (MDR_UART2, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
  UART_ITConfig (MDR_UART2, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи
  UART_Cmd(MDR_UART2,ENABLE);
#endif




//При приеме данных моргаем светодиодом
  PORT_InitTypeDef PORTC_Init;
int i; // Глобальная переменная счетчика, которая используется в функции delay()
#define delay(T) for(i = T; i > 0; i--)
  // Включение тактования порта C
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
  // Объявляем номера ножек порта, которые настраиваются данной структурой
  PORTC_Init.PORT_Pin =
  PORT_Pin_0 | PORT_Pin_1 ;
  PORTC_Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
  PORTC_Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
  PORTC_Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
  PORTC_Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
  PORT_Init(MDR_PORTC, &PORTC_Init);          // Инициализация порта C объявленной структурой

  uint8_t ReciveByte;

  while (1)
  {

    while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);//ждем пока не не установиться флаг по приему байта
    ReciveByte = UART_ReceiveData(MDR_UART1);                     //считываем принятый байт

    if ((char)ReciveByte == fromStm_BtnImpedance_On)
    {
        UART_SendData(MDR_UART1, toStm_BtnImpedance_On_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_BtnImpedance_Off)
    {
        UART_SendData(MDR_UART1, toStm_BtnImpedance_Off_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_BtnCalibr_On)
    {
        UART_SendData(MDR_UART1, toStm_BtnCalibr_On_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_BtnCalibr_Off)
    {
        UART_SendData(MDR_UART1, toStm_BtnCalibr_Off_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_BtnReset_On)
    {
        UART_SendData(MDR_UART1, toStm_BtnReset_On_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_BtnReset_Off)
    {
        UART_SendData(MDR_UART1, toStm_BtnReset_Off_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalPowerOn_ON)
    {
        UART_SendData(MDR_UART1, toStm_SignalPowerOn_ON_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalPowerOn_OFF)
    {
        UART_SendData(MDR_UART1, toStm_SignalPowerOn_OFF_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalImpedanse_ON)
    {
        UART_SendData(MDR_UART1, toStm_SignalImpedanse_ON_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalImpedanse_OFF)
    {
        UART_SendData(MDR_UART1, toStm_SignalImpedanse_OFF_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalTranslate_ON)
    {
        UART_SendData(MDR_UART1, toStm_SignalTranslate_ON_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalTranslate_OFF)
    {
        UART_SendData(MDR_UART1, toStm_SignalTranslate_OFF_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalFromOut_ON)
    {
        UART_SendData(MDR_UART1, toStm_SignalFromOut_ON_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalFromOut_OFF)
    {
        UART_SendData(MDR_UART1, toStm_SignalFromOut_OFF_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalMic_ON)
    {
        UART_SendData(MDR_UART1, toStm_SignalMic_ON_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
    if ((char)ReciveByte == fromStm_SignalMic_OFF)
    {
        UART_SendData(MDR_UART1, toStm_SignalMic_OFF_OK);
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
    }
 

  }
}


//    delay(0xFF);
//    PORT_ResetBits(MDR_PORTC, PORT_Pin_1);

//      while (uart1_IT_RX_flag != SET); //ждем пока не не установиться флаг по приему байта
//      uart1_IT_RX_flag = RESET; //очищаем флаг приема
//      ReciveByte = UART_ReceiveData (MDR_UART2); //считываем принятый байт
//      UART_SendData (MDR_UART2, ReciveByte); //отправляем принятый байт обратно
//      while (uart1_IT_TX_flag != SET); //ждем пока байт уйдет
//      uart1_IT_TX_flag = RESET; //очищаем флаг передачи
