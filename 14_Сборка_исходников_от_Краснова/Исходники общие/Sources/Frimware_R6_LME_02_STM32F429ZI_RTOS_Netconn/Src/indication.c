#include "main.h"
#include "indication.h"

static unsigned char led_ind_mode[NUMBER_OF_LEDS] = {0}; // режимы индикации для каждого светодиода
static unsigned char led_prev_ind_mode[NUMBER_OF_LEDS] = {0}; // предыдущие режимы индикации для каждого светодиода
static unsigned int blink_cnt = 0;

void ledOn(unsigned char led) {
	switch(led) {
		case LED_E1:
			LED_E1_ON();
			break;
		case LED_E2:
			LED_E2_ON();
			break;
		case LED_E3:
			LED_E3_ON();
			break;
		case LED_E4:
			LED_E4_ON();
			break;
		case LED_E5:
			LED_E5_ON();
			break;
		case LED_E6:
			LED_E6_ON();
			break;
    case LED_ON:
      LED_ON_ON();
      break;
    case LED_ERR:
      LED_ERR_ON();
      break;
	}
}
void ledOff(unsigned char led) {
	switch(led) {
		case LED_E1:
			LED_E1_OFF();
			break;
		case LED_E2:
			LED_E2_OFF();
			break;
		case LED_E3:
			LED_E3_OFF();
			break;
		case LED_E4:
			LED_E4_OFF();
			break;
		case LED_E5:
			LED_E5_OFF();
			break;
		case LED_E6:
			LED_E6_OFF();
			break;
    case LED_ON:
      LED_ON_OFF();
      break;
    case LED_ERR:
      LED_ERR_OFF();
      break;
	}
}

void indicationSet(unsigned char led, tIndMode mode) {

  if (led > NUMBER_OF_LEDS) return; // защита
  
  if (mode != IND_PREV) {
    led_prev_ind_mode[led] = led_ind_mode[led]; // запоминаем предыдущую индикация
    led_ind_mode[led] = mode;
  } else {
    led_ind_mode[led] = led_prev_ind_mode[led]; // восстанавливаем предыдущую индикацию
  }
  blink_cnt = 0;
}
void indicationInit(void){
//	TRIS_BUT_LED0_1 = 0; // переводим пины на выход
//	TRIS_BUT_LED0_2 = 0;
//	TRIS_BUT_LED1_1 = 0;
//	TRIS_BUT_LED1_2 = 0;
//	TRIS_BUT_LED2_1 = 0;
//	TRIS_BUT_LED2_2 = 0;
}

void doIndication(void) { // вызывается каждую 1 мс
  unsigned char i;

  blink_cnt++;
  if (blink_cnt >= BLINK_PERIOD) blink_cnt = 0;
  
  for (i = 0; i < NUMBER_OF_LEDS; i++) {
    switch (led_ind_mode[i]) {
      case IND_OFF:
        ledOff(i);
        break;
      case IND_ON:
        ledOn(i);
        break;
      case IND_BLINK:
        if (blink_cnt < BLINK_PERIOD/2) ledOff(i); else ledOn(i);
        break;
      default: ledOff(i);
    }
  }
}

