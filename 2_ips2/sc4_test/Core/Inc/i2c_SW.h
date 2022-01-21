#ifndef __I2C__
#define __I2C__

#include "stdint.h"
#include "stm32h7xx_hal.h"

#define I2C_ACK  0
#define I2C_NACK 1

#define I2C_PORT_SCL GPIOH
#define I2C_PORT_SDA GPIOC
#define I2C_PIN_SCL 7
#define I2C_PIN_SDA 9

#define SCL_HIGH()   { I2C_PORT_SCL->MODER &= ~(3<<(I2C_PIN_SCL<<1));             }  // SCL ���� (��������� ��� 1)
#define SCL_LOW()    { I2C_PORT_SCL->MODER |= (1<<(I2C_PIN_SCL<<1)); I2C_PORT_SCL->ODR &= ~(1<<I2C_PIN_SCL); }  // SCL output 0

#define SDA_HIGH()   { I2C_PORT_SDA->MODER &= ~(3<<(I2C_PIN_SDA<<1));             }  // SDA ���� (��������� ��� 1)
#define SDA_LOW()    { I2C_PORT_SDA->MODER |= (1<<(I2C_PIN_SDA<<1)); I2C_PORT_SDA->ODR &= ~(1<<I2C_PIN_SDA); }  // SDA output 0

#define SCL_IN       ((I2C_PORT_SCL->IDR>>I2C_PIN_SCL)&1)   // read SCL
#define SDA_IN       ((I2C_PORT_SDA->IDR>>I2C_PIN_SDA)&1)   // read SDA

#define I2C_Restart() { I2C_Start();  }

void  I2C_Init (void);
void  I2C_Start (void);
void  I2C_Stop (void);
uint8_t I2C_Put (uint8_t b);
uint8_t I2C_Get (uint8_t ack);
void  I2C_StartWrite (uint8_t IC, uint16_t addr);
void  I2C_StartRead (uint8_t IC, uint16_t addr);
void  I2C_ReadBytes (uint8_t IC, uint16_t addr, uint8_t *buf, uint16_t n);
void  I2C_WriteBytes (uint8_t IC, uint16_t addr, const uint8_t *buf, uint16_t n);
uint8_t I2C_ReadByte (uint8_t IC, uint16_t addr);
void  I2C_WriteByte (uint8_t IC, uint16_t addr, uint8_t b);
uint16_t I2C_ReadWord (uint8_t IC, uint16_t addr);
void  I2C_WriteWord (uint8_t IC, uint16_t addr, uint16_t d);

uint8_t I2C_ReadBytes8 (uint8_t IC, uint8_t addr, uint8_t *buf, uint16_t n);
uint8_t I2C_WriteBytes8 (uint8_t IC, uint8_t addr, const uint8_t *buf, uint16_t n);


#endif
