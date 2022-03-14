#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "gpio.h"
#include "main.h"

PORT_InitTypeDef PORTB_Init; // Объявление структуры, с помощью которой будет происходить инициализация порта
PORT_InitTypeDef PORTC_Init;

void GPIO_init()
{
    // Включение тактования
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);

    // Объявляем номера ножек порта, которые настраиваются данной структурой
    PORTB_Init.PORT_Pin = LED1_ERROR | LED2_REC | BUZZER | RS | RW | EN;
    PORTB_Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
    PORTB_Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
    PORTB_Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
    PORT_Init(MDR_PORTB, &PORTB_Init);          // Инициализация порта B объявленной структурой

    PORTC_Init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 ;
    PORTC_Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORTC_Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
    PORTC_Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
    PORTC_Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
    PORT_Init(MDR_PORTC, &PORTC_Init);          // Инициализация порта C объявленной структурой

}
//---------------------------------
