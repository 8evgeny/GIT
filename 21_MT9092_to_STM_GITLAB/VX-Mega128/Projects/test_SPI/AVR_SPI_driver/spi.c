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
#include "spi.h"


/*инициализация SPI*/
void SPI_Init(void)
{
  /*настройка портов ввода-вывода
  все выводы, кроме MISO выходы*/
  SPI_DDRX = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
  SPI_PORTX = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
   
  /*разрешение spi,старший бит вперед,мастер, режим 0*/
  SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
  SPSR = (0<<SPI2X);
}

/*отослать байт данных по SPI*/
void SPI_WriteByte(uint8_t data)
{
   SPI_PORTX &= ~(1<<SPI_SS); 
   SPDR = data;
   while(!(SPSR & (1<<SPIF)));
   SPI_PORTX |= (1<<SPI_SS); 
}


/*отослать и получить байт данных по SPI*/
uint8_t SPI_ReadByte(uint8_t data)
{
   uint8_t report;
   
   SPI_PORTX &= ~(1<<SPI_SS);
   SPDR = data;
   while(!(SPSR & (1<<SPIF)));
   report = SPDR;
   SPI_PORTX |= (1<<SPI_SS); 
  
   return report; 
}

/*отправить несколько байт данных по SPI*/
void SPI_WriteArray(uint8_t num, uint8_t *data)
{
   SPI_PORTX &= ~(1<<SPI_SS); 
   while(num--){
      SPDR = *data++;
      while(!(SPSR & (1<<SPIF)));
   }
   SPI_PORTX |= (1<<SPI_SS); 
}

/*отправить и получить несколько байт данных по SPI*/
void SPI_ReadArray(uint8_t num, uint8_t *data)
{
   SPI_PORTX &= ~(1<<SPI_SS); 
   while(num--){
      SPDR = *data;
      while(!(SPSR & (1<<SPIF)));
      *data++ = SPDR; 
   }
   SPI_PORTX |= (1<<SPI_SS); 
}