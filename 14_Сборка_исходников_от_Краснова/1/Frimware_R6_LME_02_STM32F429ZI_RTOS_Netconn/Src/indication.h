#ifndef INDICATION_H
#define INDICATION_H

typedef enum { IND_OFF = 0, IND_ON, IND_BLINK, IND_PREV} tIndMode;

#define BLINK_PERIOD        400u // мс

enum {LED_E1 = 0, LED_E2, LED_E3, LED_E4, LED_E5, LED_E6, LED_ON, LED_ERR}; 

#define NUMBER_OF_LEDS          8

void indicationInit(void);
void indicationSet(unsigned char led, tIndMode mode);
void doIndication(void);
void ledOn(unsigned char led);
void ledOff(unsigned char led);

#endif // INDICATION_H
