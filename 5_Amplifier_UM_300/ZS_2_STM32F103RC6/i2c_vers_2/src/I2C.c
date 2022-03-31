#include "delay.h"
#include "I2C.h"

Digital_POT_I2C_Def poti2c;

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
//    i2c_set_fast_mode(I2C1);
    i2c_set_standard_mode(I2C1);

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
    i2c_set_own_7bit_slave_address(I2C1, 0x11);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C1);
}

void init_I2C2(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
              GPIO10 | GPIO11);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C2);

    /* APB1 is running at 36MHz. */
    i2c_set_clock_frequency(I2C2, 36);

    /* 400KHz - I2C Fast Mode */
//    i2c_set_fast_mode(I2C2);
    i2c_set_standard_mode(I2C2);

    /*
     * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
     * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
     * Datasheet suggests 0x1e.
     */
    i2c_set_ccr(I2C2, 0x1e);

    /*
     * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
     * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
     * Incremented by 1 -> 11.
     */
    i2c_set_trise(I2C2, 0x0b);

    /*
     * This is our slave address - needed only if we want to receive from
     * other masters.
     */
    i2c_set_own_7bit_slave_address(I2C2, 0x27);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C2);
}

bool I2C_GetFlagStatus_(uint32_t i2c, uint32_t I2C_FLAG)
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
      while(I2C_GetFlagStatus_(i2c, I2C_FLAG_BUSY))
      {};
//    // Генерируем старт - тут все понятно )
//    I2C_GenerateSTART(I2Cx, ENABLE);
#ifdef useI2C1
        i2c_send_start(I2C1);
#endif
#ifdef useI2C2
        i2c_send_start(I2C2);
#endif
    // Ждем пока взлетит нужный флаг
    while(!I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_MODE_SELECT));

    // Посылаем адрес подчиненному  //возможно тут нужен сдвиг влево  //судя по исходникам - да, нужен сдвиг влево
    //http://microtechnics.ru/stm32-ispolzovanie-i2c/#comment-8109
//    I2C_Send7bitAddress(I2Cx, slaveAddress<<1, transmissionDirection);
#ifdef useI2C1
    i2c_send_7bit_address(I2C1, slaveAddress, transmissionDirection);
#endif
#ifdef useI2C2
    i2c_send_7bit_address(I2C2, slaveAddress, transmissionDirection);
#endif
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

//void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
void I2C_WriteData(uint32_t i2c, uint8_t data)
{
    // Просто вызываем готоваую функцию из SPL и ждем, пока данные улетят

//    I2C_SendData(I2Cx, data);
    i2c_send_data(i2c, data);
    while(!I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}


//uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
uint8_t I2C_ReadData(uint32_t  i2c)
{
    // Тут картина похожа, как только данные пришли быстренько считываем их и возвращаем
    while( !I2C_CheckEvent_(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    uint8_t data;
//    data = I2C_ReceiveData(I2Cx);
    data = i2c_get_data(i2c);
    return data;
}


void I2C_POD_StartTransm (uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
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


void digitalPOT_I2C_init(uint8_t pot_Addr)
{
    poti2c.Addr = pot_Addr;
    init_I2C1(); // Wire.begin();
}

void digitalPOT_send_data(uint8_t byte1, uint8_t byte2, uint8_t byte3 )
{
    I2C_POD_StartTransm (I2C1, I2C_Direction_Transmitter, poti2c.Addr);
    //            I2C_WriteDat(I2C1,(int)byte1);
    //            I2C_WriteDat(I2C1,(int)byte2);
    //            I2C_WriteDat(I2C1,(int)byte3);
    //            I2C_GenerateSTOP(I2C1, ENABLE);
}

