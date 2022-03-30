#pragma once
#include "main.h"

void init_I2C1(void);
bool I2C_GetFlagStatus_(uint32_t i2c, uint32_t I2C_FLAG);
ErrorStatus I2C_CheckEvent_(uint32_t i2c, uint32_t I2C_EVENT);
void I2C_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(uint32_t i2c, uint8_t data);
uint8_t I2C_ReadData(uint32_t  i2c);
