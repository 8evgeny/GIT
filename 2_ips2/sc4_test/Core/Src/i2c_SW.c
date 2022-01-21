#include "i2c_SW.h"

void delay_us(uint32_t us) {
	int32_t us_count_tick =  us * (SystemCoreClock/1000000);
	//разрешаем использовать счётчик
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	//обнуляем значение счётного регистра
	DWT->CYCCNT  = 0;
	//запускаем счётчик
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; 
	while(DWT->CYCCNT < us_count_tick);
	//останавливаем счётчик
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}

#define WaitBit()  delay_us(8)     // 4us (I2C_FREQ~100khz), min=0.5us
#define WaitSCL()  delay_us(24)    // 12us, min=10us

void PulseSCL (void)
 {
  WaitBit();
  SCL_HIGH();
  WaitBit();
  SCL_LOW();
 }

//------------------------------
void I2C_Start (void)
 {
  SCL_HIGH();
  WaitBit();
  SDA_LOW();
  WaitBit();
  SCL_LOW();
  WaitBit();
 }

//------------------------------
void I2C_Stop (void)
 {
  SDA_LOW();
  WaitBit();
  SCL_HIGH();
  WaitBit();
  SDA_HIGH();
  WaitBit();
 }

//------------------------------
void I2C_Init ()
 {
  I2C_PORT_SCL->MODER &= ~(3<<(I2C_PIN_SCL<<1)); // input mode port
  I2C_PORT_SCL->OTYPER |= (1<<I2C_PIN_SCL); // open-drain for output
  I2C_PORT_SCL->OSPEEDR |= (3<<(I2C_PIN_SCL<<1)); // very fast speed
  I2C_PORT_SCL->PUPDR &= ~(3<<(I2C_PIN_SCL<<1)); // no pullup/pulldown

  I2C_PORT_SDA->MODER &= ~(3<<(I2C_PIN_SDA<<1)); // input mode port
  I2C_PORT_SDA->OTYPER |= (1<<I2C_PIN_SDA); // open-drain for output
  I2C_PORT_SDA->OSPEEDR |= (3<<(I2C_PIN_SDA<<1)); // very fast speed
  I2C_PORT_SDA->PUPDR &= ~(3<<(I2C_PIN_SDA<<1)); // no pullup/pulldown 
}

//------------------------------
uint8_t I2C_Put (uint8_t b)
 {
  uint8_t ack;
  uint8_t m = 128;

  do {
    if (b & m) SDA_HIGH() else SDA_LOW();
	  PulseSCL();
    m >>= 1;
  } while (m > 0);

  SDA_HIGH();
  delay_us(1);  // 1 us
  SCL_HIGH();
  WaitBit();
  ack = SDA_IN;
  SCL_LOW();
  WaitBit();

  return ack;
 }

//------------------------------
uint8_t I2C_Get (uint8_t ack)
 {
  uint8_t b = 0, i;
  uint8_t n = 200;

  SDA_HIGH();
  SCL_HIGH();
  delay_us(1);  // 1 us
  
  if (!SCL_IN) {
    while ((n-- > 0) && !SCL_IN) WaitSCL();
  }

  for (i = 8; i > 0; i--) {
    b <<= 1;
    SCL_HIGH();
	  WaitBit();
    b |= SDA_IN;
    SCL_LOW();
    WaitBit();
  }

  if (ack) SDA_HIGH() else SDA_LOW();
  PulseSCL();

  SDA_HIGH();
  WaitBit();

  return b;
 }

//------------------------------
uint8_t I2C_StartWrite8 (uint8_t IC, uint8_t addr)
{
  I2C_Start();
  if (I2C_Put(IC)==I2C_NACK) return I2C_NACK;
  return I2C_Put(addr);
}

void  I2C_StartWrite (uint8_t IC, uint16_t addr)
 {
  I2C_Start();
  I2C_Put(IC);
  I2C_Put(addr >> 8);
  I2C_Put(addr & 0xFF);
 }

//------------------------------
uint8_t I2C_StartRead8 (uint8_t IC, uint8_t addr)
{
  if (I2C_StartWrite8(IC,addr)==I2C_NACK) return I2C_NACK;;
  I2C_Restart();
  return I2C_Put(IC | 1);
}

void  I2C_StartRead (uint8_t IC, uint16_t addr)
 {
  I2C_StartWrite(IC,addr);
  I2C_Restart();
  I2C_Put(IC | 1);
 }

//------------------------------
uint8_t  I2C_ReadBytes8 (uint8_t IC, uint8_t addr, uint8_t *buf, uint16_t n)
 {
  uint16_t i;

  if (I2C_StartRead8(IC,addr)==I2C_NACK) return I2C_NACK;

  for (i = 0; i < n; i++)
   buf [i] = I2C_Get(I2C_ACK);

  I2C_Get(I2C_NACK);
  I2C_Stop();

  return I2C_ACK;
 }
 
void  I2C_ReadBytes (uint8_t IC, uint16_t addr, uint8_t *buf, uint16_t n)
 {
  uint16_t i;

  I2C_StartRead(IC,addr);

  for (i = 0; i < n; i++)
   buf [i] = I2C_Get(I2C_ACK);

  I2C_Get(I2C_NACK);
  I2C_Stop();
 }

//------------------------------
uint8_t I2C_WriteBytes8 (uint8_t IC, uint8_t addr, const uint8_t *buf, uint16_t n)
 {
  uint16_t i;

  if (I2C_StartWrite8(IC,addr)==I2C_NACK) return I2C_NACK;

  for (i = 0; i < n; i++)
	  if (I2C_Put(buf [i])==I2C_NACK) return I2C_NACK;

  I2C_Stop();

  WaitSCL();

  return I2C_ACK;
 }


void  I2C_WriteBytes (uint8_t IC, uint16_t addr, const uint8_t *buf, uint16_t n)
 {
  uint16_t i;

  I2C_StartWrite(IC,addr);

  for (i = 0; i < n; i++)
   I2C_Put(buf [i]);

  I2C_Stop();
  
  WaitSCL(); 
 }

//------------------------------
uint8_t I2C_ReadByte (uint8_t IC, uint16_t addr)
 {
  uint8_t b;

  I2C_StartRead(IC,addr);
  b = I2C_Get(I2C_NACK);
  I2C_Stop();

  return b;
 }

 //------------------------------
void  I2C_WriteByte (uint8_t IC, uint16_t addr, uint8_t b)
 {
  I2C_StartWrite(IC,addr);
  I2C_Put(b);
  I2C_Stop();

  WaitSCL(); 
 }

//------------------------------
uint16_t I2C_ReadWord (uint8_t IC, uint16_t addr)
 {
  uint8_t a, b;

  I2C_StartRead(IC,addr);
  a = I2C_Get(I2C_ACK);
  b = I2C_Get(I2C_NACK);
  I2C_Stop();

  return (a<<8) | b;
 }

//------------------------------
void  I2C_WriteWord (uint8_t IC, uint16_t addr, uint16_t d)
 {
  I2C_StartWrite(IC,addr);
  I2C_Put(d >> 8);
  I2C_Put(d & 0xFF);
  I2C_Stop();

  WaitSCL(); 
 }

