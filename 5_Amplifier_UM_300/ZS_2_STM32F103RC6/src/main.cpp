#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif


int
main(void) {

    init();

    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES-1, NULL);
	vTaskStartScheduler();

	for (;;);

	return 0;
}

