//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: Mega
//
//  Compiler....: 
//
//  Description.: Драйвер SPI
//
//  Data........: 2.10.12
//
//***************************************************************************
#ifndef SPI_H
#define SPI_H

#include "compilers.h"

#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   6 
#define SPI_MOSI   5
#define SPI_SCK    7
#define SPI_SS     4

/*____________функции____________________*/

/*инициализация SPI модуля*/
void SPI_Init(void); 

/*отправить байт данных по SPI*/
void SPI_WriteByte(uint8_t data); 

/*отправить и получить байт данных по SPI*/
uint8_t SPI_ReadByte(uint8_t data);

/*отправить несколько байт данных по SPI*/
void SPI_WriteArray(uint8_t num, uint8_t *data);

/*отправить и получить несколько байт данных по SPI*/
void SPI_ReadArray(uint8_t num, uint8_t *data);

#endif //SPI_H