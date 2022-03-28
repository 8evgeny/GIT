#include "main.h"

void initGPIO()
{
    //Порт A
    PORT_InitTypeDef PORTA_Init;
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
    PORTA_Init.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORTA_Init.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PORTA_Init.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORTA_Init.PORT_PD = PORT_PD_DRIVER;
    PORTA_Init.PORT_GFEN = PORT_GFEN_OFF;
    PORTA_Init.PORT_FUNC =  PORT_FUNC_PORT;
    PORTA_Init.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORTA_Init.PORT_MODE = PORT_MODE_DIGITAL;
    //Выходы
    PORTA_Init.PORT_OE =    PORT_OE_OUT;
    PORTA_Init.PORT_Pin =   PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_7 ;
    PORT_Init(MDR_PORTA, &PORTA_Init);
    //Входы
    PORTA_Init.PORT_OE =    PORT_OE_IN;
    PORTA_Init.PORT_Pin =   PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6;
    PORT_Init(MDR_PORTA, &PORTA_Init);

    //Порт B инициализируется с UART
}
