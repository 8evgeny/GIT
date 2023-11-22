
#ifndef G711_H
#define G711_H


int16_t AudioSamleFromADC(void);
void AudioSamleToDAC(int16_t s);
void AudioInit(void);
void ReadProtect(void);


#endif /* G711_H */

