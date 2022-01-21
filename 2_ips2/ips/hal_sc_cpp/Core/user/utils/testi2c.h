/*
 * testi2c.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Dmitry Donskih
 */

#ifndef INC_TESTI2C_H_
#define INC_TESTI2C_H_

#ifdef SC4BOPLA

#define IPS_LED_ON		0xFF
#define IPS_LED_OFF		0x00

#define IPS_LED_RED		0x01
#define IPS_LED_GREEN		0x00

void writeI2CBlocking(I2C_HandleTypeDef *hi2c,uint8_t ic_addr,uint8_t reg,uint8_t value);
int testi2c(I2C_HandleTypeDef *hi2c);

void ledSetupPWMMode(void);
void ledPWM(uint8_t led,uint8_t color,uint8_t value);
void ledON(uint8_t led,uint8_t color);
void ledOFF(uint8_t led,uint8_t color);
void ledAllON(void);
void ledAllOFF(void);
int i2cRegRead(uint8_t dev,uint8_t reg);
void sysKeyboardDiag(void);
/*
void kbON(void);
void kbOFF(void);
*/

#endif /* SC4BOPLA */
#endif /* INC_TESTI2C_H_ */
