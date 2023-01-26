#pragma once
#include "main.h"
#include <stdbool.h>
#include <setjmp.h>
#include <libopencm3/stm32/i2c.h>

#define systicks	xTaskGetTickCount

#define     __IO    volatile
#define  I2C_FLAG_BUSY                                     ((uint32_t)0x00020000)
#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */
#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)

typedef enum {
    I2C_Ok = 0,
    I2C_Addr_Timeout,
    I2C_Addr_NAK,
    I2C_Write_Timeout,
    I2C_Read_Timeout
} I2C_Fails;

enum I2C_RW {
    Read = 1,
    Write = 0
};

typedef struct {
    uint32_t	device;		// I2C device
    uint32_t	timeout;	// Ticks
} I2C_Control;

extern jmp_buf i2c_exception;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

const char *i2c_error(I2C_Fails fcode);

void i2c_configure(I2C_Control *dev,uint32_t i2c,uint32_t ticks);
void i2c_wait_busy(I2C_Control *dev);
void i2c_start_addr(I2C_Control *dev,uint8_t addr,enum I2C_RW rw);
void i2c_write(I2C_Control *dev,uint8_t byte);
void i2c_write_restart(I2C_Control *dev,uint8_t byte,uint8_t addr);
uint8_t i2c_read(I2C_Control *dev,bool lastf);

inline void i2c_stop(I2C_Control *dev) { i2c_send_stop(dev->device); }

static I2C_Control i2c_;			// I2C Control struct
static volatile bool readf = false; 	// ISR flag
static volatile int isr_count = 0;	// ISR count

typedef struct
{
    uint8_t Addr;
    uint8_t Access_Control_Register_byte1;
    uint8_t Access_Control_Register_byte2;
    uint8_t Access_Control_Register_byte3;
    uint8_t MEMORY_REGISTER_DR_7_bit;
    uint8_t WIPER_CONTROL_REGISTER_WCR_7_bit;
} Digital_POT_I2C_Def ;

void init_I2C1(void);
void init_I2C2(void);
bool I2C_GetFlagStatus_(uint32_t i2c, uint32_t I2C_FLAG);
ErrorStatus I2C_CheckEvent_(uint32_t i2c, uint32_t I2C_EVENT);
void I2C_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(uint32_t i2c, uint8_t data);
uint8_t I2C_ReadData(uint32_t  i2c);

void I2C_POD_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress);
void digitalPOT_send_data(uint8_t byte1, uint8_t byte2, uint8_t byte3 );

void SCL_in (void);
void SCL_out_DOWN (void);
void SDA_in (void);
void SDA_out_DOWN (void);

