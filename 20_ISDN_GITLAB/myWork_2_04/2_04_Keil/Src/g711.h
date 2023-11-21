
#ifndef G711_H
#define G711_H

typedef enum {DM_SOUND = 0, DM_TONE} tDacMode; // что выводит ЦАП: звук или рингтон

extern tDacMode DAC_mode; // выводим звук или генерируем рингтон (или гудки)
//Tone duration (warble frequency) = 500 / COEF
//where: Frequency is in Hz, and 0≤COEF <256
//Highest frequency possible: 500 Hz
//Lowest frequency possible: 2.0 Hz
extern uint8_t warble_rate; // частота чередования частот генератора рингтона, см. регистр MT9092 "TONE_RINGER_WARBLE_RATE_REGISTER"
extern uint8_t tone_coef_1; // первая частота генератора рингтона, см. регистр MT9092 "TONE_COEFFICIENT_REGISTER_1"
extern uint8_t tone_coef_2; // первая частота генератора рингтона, см. регистр MT9092 "TONE_COEFFICIENT_REGISTER_2"
extern float att; // ослабление/усиление звука

int16_t AudioSamleFromADC(void);
void AudioSamleToDAC(uint8_t s);
void AudioInit(void);

#endif /* G711_H */

