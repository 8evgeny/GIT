#include "main.h"

void initGPIO()
{
    //При приеме данных моргаем светодиодом
    PORT_InitTypeDef PORTC_Init;
    // Включение тактования порта C
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
    // Объявляем номера ножек порта, которые настраиваются данной структурой
    PORTC_Init.PORT_Pin =   PORT_Pin_0 | PORT_Pin_1 ;
    PORTC_Init.PORT_OE =    PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORTC_Init.PORT_FUNC =  PORT_FUNC_PORT;        // Работа а режиме порта ввода-вывода
    PORTC_Init.PORT_MODE =  PORT_MODE_DIGITAL;     // Цифровой режим
    PORTC_Init.PORT_SPEED = PORT_SPEED_SLOW;       // Низкая частота тактования порта
    PORT_Init(MDR_PORTC, &PORTC_Init);             // Инициализация порта объявленной структурой

}
