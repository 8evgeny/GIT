#ifndef __A_MAIN_H /* Define to prevent recursive inclusion --------------------*/
#define __A_MAIN_H
#ifdef __cplusplus
 extern "C" {
#endif

//#define USE_FULL_ASSERT	// закоментировать, если не нужна проверка

/* Main Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stddef.h>
#include <stdbool.h>

//  Дополнительно -------------------------------------------------------------
#define _BITSET(bits) ((uint16_t)((bits % 010) | (bits / 010 % 010) << 1 | (bits / 0100 % 010) << 2 | \
  (bits / 01000 % 010) << 3 | (bits / 010000 % 010) << 4 | (bits / 0100000 % 010) << 5 | (bits / 01000000 % 010) << 6 | \
  (bits / 010000000 % 010) << 7 | (bits / 0100000000 % 010) << 8 | (bits / 01000000000 % 010) << 9 | (bits / 010000000000 % 010) << 10 | \
  (bits / 0100000000000 % 010) << 11 | (bits / 01000000000000 % 010) << 12 | (bits / 010000000000000 % 010) << 13 | \
  (bits / 0100000000000000 % 010) << 14 | (bits / 01000000000000000 % 010) << 15))
#define BIN(bits) _BITSET(0##bits)	// Пример: BIN(111001100110)

#define BIT(n) (1UL<<(n))	// Пример: BIT(2)|BIT(0) == 0b101 == 0x5

#define ABS(x)         (x<0)?(-x):x
#define MAX(a,b)       (a<b)?(b):a

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define FALSE	false
#define TRUE	true

// Остальная конфигурация -----------------------------------------------------
//#define EXT_QUARZ		// для внешнего кварца
//#define NODEBUG		// защита от считывания FLASH
//#define W_DOG			// сторожевая собака

// stm32f030k6
//#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* страница FLASH 1кБайт*/
//#define FLASH_USER_START_ADDR   ((uint32_t)0x08000400)   /* начало второй страницы */
//#define FLASH_USER_END_ADDR     ((uint32_t)0x08000800)   /* конец */

// для фукции задержки мкс
#define DWT_CYCCNT *(volatile uint32_t*)0xE0001004
#define DWT_CONTROL *(volatile uint32_t*)0xE0001000
#define SCB_DEMCR *(volatile uint32_t*)0xE000EDFC

// Предопределения ------------------------------------------------------------
#ifndef PI
#define	PI					(3.14159265)
#endif

#define	SYSTEMCORECLOCK		32000000UL
#define	NUMBRSFREQ			6 // количество частот
#define	ADCBUFMAX			1000
#define	DACBUFMAX			1250
#define	ADC_CH				2

#define	CYCLDUMMY			6 // холостые циклы

#define	EELABELOK			38

#define	KDEBOUNCE			15

//#define	VREFINT_CAL 	*((uint16_t*)0x1FFFF7BA)
#define	VREFINT_CAL 		*((uint16_t*)0x1FF80078) // L151-A

#define AKT_GET()		(AKT_GPIO_Port->IDR & AKT_Pin)

#define LED0_ON()		(LED0_GPIO_Port->BSRR = LED0_Pin)
#define LED0_OFF()		(LED0_GPIO_Port->BSRR = LED0_Pin << 16)
#define LED0_TOG()		(LED0_GPIO_Port->ODR ^= LED0_Pin)

#define TX_485()		(DE_GPIO_Port->BSRR = DE_Pin)
#define RX_485()		(DE_GPIO_Port->BSRR = DE_Pin << 16)

/*
#define PIN_TEST1_GET()	(GPIOA->IDR & GPIO_Pin_8)
#define PIN_TEST1_ON()	(GPIOA->BSRR = GPIO_Pin_8)
#define PIN_TEST1_OFF()	(GPIOA->BRR = GPIO_Pin_8)
#define PIN_TEST1_TOG() (GPIOA->ODR ^= GPIO_Pin_8)
#define PIN_TEST1_OUT()	(GPIOA->MODER &= ~GPIO_MODER_MODER8_1)
#define PIN_TEST1_IN()	(GPIOA->MODER |= GPIO_MODER_MODER8)
*/

typedef struct complex_number {
	float Re;
	float Im;
} cplx_t;


// Декларации глобальных функций ----------------------------------------
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void User_init(void);
void User_main(void);
float measImpedance(uint8_t line, uint8_t n_freq);
uint16_t measLeakage(uint8_t line);
void User_int_DMA_ADC(void);
void setInputReleToAmp(uint8_t line);
void setInputReleToMeasCircuit(uint8_t line);

#ifdef __cplusplus
}
#endif
#endif /* __A_MAIN_H */
