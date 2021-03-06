#include <init.h>
#include <stdlib.h>
#include <MLDR187_lib.h>
#include <MLDR187_uart.h>
#include <MLDR187_gpio.h>
#include <MLDR187_bkp.h>
#include <MLDR187_it.h>
#include <stdbool.h>


struct {MDR_GPIO_TypeDef* port; uint16_t pin; bool state;}
		LED_PINS[] = {
		{MDR_PORTD, pin0, 0},
		{MDR_PORTD, pin1, 0}
};

void init_clock()
{
    ErrorStatus erCode;
    RST_CLK_CpuSpeed_InitTypeDef clkInit;

    clkInit.hseState = RST_CLK_HseOscillator;
    clkInit.hseSpeed = 8000000U;
    clkInit.cpuC1Src = RST_CLK_CpuC1SelHse;
    clkInit.pllState = RST_CLK_PllFromHse;
    clkInit.pllMult = 6;
    clkInit.cpuC2Src = RST_CLK_CpuC2SelPllCpu;
    clkInit.cpuC3Div = RST_CLK_Div1;
    clkInit.hclkSrc = RST_CLK_HclkSelCpuC3;
    erCode = RST_CLK_SetupCpuSpeed(&clkInit);
    if (erCode != SUCCESS) {
        exit(EXIT_FAILURE);
    }

    erCode = RST_CLK_SetupHsPeripheralClock(RST_CLK_Clk_PER1_C2, RST_CLK_ClkSrc_CPU_C1);
    if (erCode != SUCCESS) {
        exit(EXIT_FAILURE);
    }
}

void init_interrupts() {
    PLIC_SetMinPermPriority(0);
    PLIC_SetPriority(CLKMEAUSRE_IRQn, 1);
    PLIC_DisableIRQ(CLKMEAUSRE_IRQn);
    PLIC_ReleaseIRQ(CLKMEAUSRE_IRQn);
    PLIC_EnableIRQ(CLKMEAUSRE_IRQn);
    enable_irq_extm();
}

void init_leds() {
    for (int i = 0; i < (int)sizeof(LED_PINS) / (int)sizeof(LED_PINS[0]); i++) {
        PORT_InitTypeDef portInit = {
                .PORT_MODE = PORT_MODE_DIGITAL,
                .PORT_OE = PORT_OE_OUT,
                .PORT_FUNC = PORT_FUNC_PORT,
                .PORT_SPEED = PORT_SPEED_SLOW_4mA,
                .PORT_PULL_DOWN = PORT_PULL_DOWN_OFF
        };

        PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !LED_PINS[i].state);
        PORT_Init(LED_PINS[i].port, LED_PINS[i].pin, &portInit);
    }
}

void toggle_led(int i) {
    LED_PINS[i].state = !LED_PINS[i].state;
    PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !LED_PINS[i].state);
}

void set_led(int i, BitStatus state) {
    LED_PINS[i].state = state;
    PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !state);
}
