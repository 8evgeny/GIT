//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: Mega
//
//  Compiler....: 
//
//  Description.: ������� SPI
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

/*____________�������____________________*/

/*������������� SPI ������*/
void SPI_Init(void); 

/*��������� ���� ������ �� SPI*/
void SPI_WriteByte(uint8_t data); 

/*��������� � �������� ���� ������ �� SPI*/
uint8_t SPI_ReadByte(uint8_t data);

/*��������� ��������� ���� ������ �� SPI*/
void SPI_WriteArray(uint8_t num, uint8_t *data);

/*��������� � �������� ��������� ���� ������ �� SPI*/
void SPI_ReadArray(uint8_t num, uint8_t *data);

#endif //SPI_H