#include "main.h"

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


int
main(void) {

    init();

    xTaskCreate(task1, "LED1", 100, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(task2, "LED2", 100, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(task3, "LED3", 100, NULL, configMAX_PRIORITIES-1, NULL);
	vTaskStartScheduler();

	for (;;);

	return 0;
}

