#include <mldr124.h>
#include <mldr124_port.h>

#define BRD_V3

#ifdef BRD_V3
    #define LedGreen PORT_Pin_17
    #define LedBlue  PORT_Pin_19
    #define LedRed   PORT_Pin_21

#elif defined (BRD_V2)
    #define LedGreen PORT_Pin_16
    #define LedBlue  PORT_Pin_18
    #define LedRed   PORT_Pin_20
#endif

#define LED_PORT PORTC
#define LED_DELAY       500000

void Delay(uint32_t waitTicks)
{
    uint32_t i;
    for (i = 0; i < waitTicks; i++);
}

void LED_Show(uint32_t ledSel)
{
    PORT_SetBits(LED_PORT, ledSel);
    Delay(LED_DELAY);
    PORT_ResetBits(LED_PORT, ledSel);
}

int main(void)
{
    PORT_InitTypeDef LedPort;

    CLK_CNTR->KEY = 0x8555AAA1;
    CLK_CNTR->PER0_CLK |=  1<<15; // PORTC EN

    LED_PORT->KEY = 0x8555AAA1;
    PORT_DeInit(LED_PORT);

    PORT_StructInit( &LedPort );

    LedPort.PORT_Pin     = LedGreen | LedBlue | LedRed;
    LedPort.PORT_SOE     = PORT_SOE_OUT;
    LedPort.PORT_SFUNC   = PORT_SFUNC_PORT;
    LedPort.PORT_SANALOG = PORT_SANALOG_DIGITAL;
    LedPort.PORT_SPWR    = PORT_SPWR_300;

    PORT_Init( LED_PORT, &LedPort );

    while(1)
    {
        LED_Show(LedGreen);
        LED_Show(LedBlue);
        LED_Show(LedRed);
    }
}
