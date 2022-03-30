#include "main.h"
#include "delay.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#define     __IO    volatile
//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

//для I2C
//GPIO_InitTypeDef i2c_gpio;
//I2C_InitTypeDef i2c;

void init_I2C1(void)
{

    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
              GPIO6 | GPIO7);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C1);

    /* APB1 is running at 36MHz. */
    i2c_set_clock_frequency(I2C1, 36);

    /* 400KHz - I2C Fast Mode */
    i2c_set_fast_mode(I2C2);
//    i2c_set_standard_mode(I2C1);

    /*
     * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
     * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
     * Datasheet suggests 0x1e.
     */
    i2c_set_ccr(I2C1, 0x1e);

    /*
     * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
     * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
     * Incremented by 1 -> 11.
     */
    i2c_set_trise(I2C1, 0x0b);

    /*
     * This is our slave address - needed only if we want to receive from
     * other masters.
     */
    i2c_set_own_7bit_slave_address(I2C1, 0x27);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C1);

//    // Включаем тактирование нужных модулей
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

//    // А вот и настройка I2C
//    i2c.I2C_ClockSpeed = 100000;
//    i2c.I2C_Mode = I2C_Mode_I2C;
//    i2c.I2C_DutyCycle = I2C_DutyCycle_2;
//    // Адрес я тут взял первый пришедший в голову
//    i2c.I2C_OwnAddress1 = 0x15;
//    i2c.I2C_Ack = I2C_Ack_Enable;
//    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//    I2C_Init(I2C1, &i2c);

//    // I2C использует две ноги микроконтроллера, их тоже нужно настроить
//    i2c_gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//    i2c_gpio.GPIO_Mode = GPIO_Mode_AF_OD;
//    i2c_gpio.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &i2c_gpio);

//    // Ну и включаем, собственно, модуль I2C1
//    I2C_Cmd(I2C1, ENABLE);
}


FlagStatus I2C_GetFlagStatus_(uint32_t i2c, uint32_t I2C_FLAG)
//FlagStatus I2C_GetFlagStatus_(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
    #define FLAG_Mask               ((uint32_t)0x00FFFFFF)
  FlagStatus bitstatus = RESET;
  __IO uint32_t i2creg = 0, i2cxbase = 0;

  /* Check the parameters */
//  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
//  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* Get the I2Cx peripheral base address */

  if(i2c == (uint32_t)I2C1) i2cxbase = I2C1_BASE;
  if(i2c == (uint32_t)I2C2) i2cxbase = I2C2_BASE;
  #ifdef I2C3_BASE
  if(i2c == (uint32_t)I2C3) i2cxbase = I2C3_BASE;
  #endif

//  i2cxbase = (uint32_t)I2Cx;

  /* Read flag register index */
  i2creg = I2C_FLAG >> 28;

  /* Get bit[23:0] of the flag */
  I2C_FLAG &= FLAG_Mask;

  if(i2creg != 0)
  {
    /* Get the I2Cx SR1 register address */
    i2cxbase += 0x14;
  }
  else
  {
    /* Flag in I2Cx SR2 Register */
    I2C_FLAG = (uint32_t)(I2C_FLAG >> 16);
    /* Get the I2Cx SR2 register address */
    i2cxbase += 0x18;
  }

  if(((*(__IO uint32_t *)i2cxbase) & I2C_FLAG) != (uint32_t)RESET)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }

  /* Return the I2C_FLAG status */
  return  bitstatus;
}


ErrorStatus I2C_CheckEvent_(uint32_t i2c, uint32_t I2C_EVENT)
{
  #define FLAG_Mask               ((uint32_t)0x00FFFFFF)
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters */
//  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
//  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register */
  flag1 = I2C_SR1(i2c);
  flag2 = I2C_SR2(i2c);
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }
  /* Return status */
  return status;
}



void I2C_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
//void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress)
{

//    i2c_send_start(I2C1);
//    i2c_send_7bit_address(I2C1, slaveAddress, transmissionDirection);

//    // На всякий слуыай ждем, пока шина осовободится
    while(I2C_GetFlagStatus_(i2c, I2C_FLAG_BUSY));
//    // Генерируем старт - тут все понятно )
//    I2C_GenerateSTART(I2Cx, ENABLE);

        i2c_send_start(I2C1);


    // Ждем пока взлетит нужный флаг
    while(!I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_MODE_SELECT));
    // Посылаем адрес подчиненному  //возможно тут нужен сдвиг влево  //судя по исходникам - да, нужен сдвиг влево
    //http://microtechnics.ru/stm32-ispolzovanie-i2c/#comment-8109
//    I2C_Send7bitAddress(I2Cx, slaveAddress<<1, transmissionDirection);

    i2c_send_7bit_address(I2C1, slaveAddress, transmissionDirection);

//    // А теперь у нас два варианта развития событий - в зависимости от выбранного направления обмена данными
    if(transmissionDirection== I2C_Direction_Transmitter)
    {
        while(!I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    if(transmissionDirection== I2C_Direction_Receiver)
    {
    while(!I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

/*******************************************************************/
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
    // Просто вызываем готоваую функцию из SPL и ждем, пока данные улетят
    I2C_SendData(I2Cx, data);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}



/*******************************************************************/
uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
{
    // Тут картина похожа, как только данные пришли быстренько считываем их и возвращаем
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    uint8_t data;
    data = I2C_ReceiveData(I2Cx);
    return data;
}

