#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

extern void vApplicationStackOverflowHook(
	xTaskHandle *pxTask,
	signed portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(
  xTaskHandle *pxTask __attribute((unused)),
  signed portCHAR *pcTaskName __attribute((unused))
) {
	for(;;);	// Loop forever here..
}

static void
task1(void *args __attribute((unused)))
{

	for (;;) {
        gpio_toggle(GPIOC, GPIO6);
        vTaskDelay(pdMS_TO_TICKS(200));
	}
}

static void
task2(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOB, GPIO14);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int
main(void) {

//    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO6);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);

    xTaskCreate(task1, "LED1", 100, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(task2, "LED2", 100, NULL, configMAX_PRIORITIES-1, NULL);
	vTaskStartScheduler();

	for (;;);

	return 0;
}

// End
