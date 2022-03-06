#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define WORK_MODE_UDEF		0
#define WORK_MODE_HWFAIL	1
#define WORK_MODE_NOCFG		2
#define WORK_MODE_OK		3

uint8_t get_pdo_work_mode();

void Error_Handler(void);

#define DET_48V_Pin GPIO_PIN_13
#define DET_48V_GPIO_Port GPIOC

#if defined SC_4 || defined SL_1
#define UPR_PHONE_Pin GPIO_PIN_14
#endif
#ifdef SC_2
#define UPR_PHONE_Pin GPIO_PIN_6
#endif
#define UPR_PHONE_GPIO_Port GPIOC

#define UPR_MIC2_Pin GPIO_PIN_15 //Ошибка пин не задействован
#define UPR_MIC2_GPIO_Port GPIOC

#define UPR1_SP_Pin GPIO_PIN_3
#define UPR1_SP_GPIO_Port GPIOA

#define UPR2_FAN_Pin GPIO_PIN_4
#define UPR2_FAN_GPIO_Port GPIOA

#define MKVKL_UPR_Pin GPIO_PIN_5
#define MKVKL_UPR_GPIO_Port GPIOA

#define RELE_UPR_Pin GPIO_PIN_6
#define RELE_UPR_GPIO_Port GPIOA

#define POW_DOWN_Pin GPIO_PIN_2
#define POW_DOWN_GPIO_Port GPIOB

#define CFG_UI0_Pin GPIO_PIN_6
#define CFG_UI0_GPIO_Port GPIOC

#define CFG_UI1_Pin GPIO_PIN_7
#define CFG_UI1_GPIO_Port GPIOC

#define CFG_UI2_Pin GPIO_PIN_8
#define CFG_UI2_GPIO_Port GPIOC

#define VOL_UP_Pin GPIO_PIN_9
#define VOL_UP_GPIO_Port GPIOA

#define VOL_DOWN_Pin GPIO_PIN_10
#define VOL_DOWN_GPIO_Port GPIOA

#define SENS_UP_Pin GPIO_PIN_11
#define SENS_UP_GPIO_Port GPIOA

#define SENS_DOWN_Pin GPIO_PIN_12
#define SENS_DOWN_GPIO_Port GPIOA

#define INT_BUT_Pin GPIO_PIN_4
#define INT_BUT_GPIO_Port GPIOA

#define NORMA_UPR_Pin GPIO_PIN_6
#define NORMA_UPR_GPIO_Port GPIOD

#define TEST_BUT_Pin GPIO_PIN_5
#define TEST_BUT_GPIO_Port GPIOB

#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB

#ifdef SC_2
#define LED1 GPIO_PIN_10
#define LED2 GPIO_PIN_11
#define LED3 GPIO_PIN_12
#define LED4 GPIO_PIN_6
#define LED5 GPIO_PIN_7
#define LED6 GPIO_PIN_8
#define K_1 GPIO_PIN_9
#define K_2 GPIO_PIN_10
#define K_3 GPIO_PIN_11
#define K_4 GPIO_PIN_12
#define K_5 GPIO_PIN_13
#define K_6 GPIO_PIN_14
#endif

#ifdef SL_1
#define LedUPR_1 GPIO_PIN_7
#define LedUPR_2 GPIO_PIN_8
#define LedUPR_3 GPIO_PIN_9
#define LedUPR_4 GPIO_PIN_10
#define LedUPR_5 GPIO_PIN_11
#define LedUPR_6 GPIO_PIN_12
#define LedUPR_7 GPIO_PIN_13
#define LedUPR_8 GPIO_PIN_14
#define IN_1 GPIO_PIN_9
#define IN_2 GPIO_PIN_8
#define IN_3 GPIO_PIN_7
#define IN_4 GPIO_PIN_6
#define IN_5 GPIO_PIN_12
#define IN_6 GPIO_PIN_11
#define IN_7 GPIO_PIN_10
#define IN_8 GPIO_PIN_9
#endif

#define I2C3_INT GPIO_PIN_8
#define I2C3_INT_PORT GPIOC

#define TANG_UPR_Port GPIOA
#define TANG_UPR_Pin GPIO_PIN_9

#define PEDAL_UPR_Port GPIOA
#define PEDAL_UPR_Pin GPIO_PIN_10

#define RELE1_UPR_Port GPIOA
#define RELE1_UPR_Pin GPIO_PIN_11

#define AMP_UPR_Port GPIOA
#define AMP_UPR_Pin GPIO_PIN_12


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

