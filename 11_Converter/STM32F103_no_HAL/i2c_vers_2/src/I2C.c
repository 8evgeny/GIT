#include "delay.h"
#include "I2C.h"

Digital_POT_I2C_Def poti2c;
char buf[10];
void toUart(const char* data)
{
    stringTo_diagnostic_Usart1(data);
    sprintf(buf, "%04X", (uint16_t)I2C1_SR1);
    stringTo_diagnostic_Usart1("I2C1_SR1");
    stringTo_diagnostic_Usart1(buf);
    sprintf(buf, "%04X", (uint16_t)I2C1_SR2);
    stringTo_diagnostic_Usart1("I2C1_SR2");
    stringTo_diagnostic_Usart1(buf);
}
void toUart_(const char* data)
{
    stringTo_diagnostic_Usart1(data);
}
void toUart__(uint8_t data)
{
    sprintf(buf, "%d", data);
    stringTo_diagnostic_Usart1(buf);
}

void init_I2C2(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
              GPIO_I2C2_SCL | GPIO_I2C2_SDA);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C2);

    /* APB1 is running at 36MHz. */
    i2c_set_clock_frequency(I2C2, 36);

    /* 400KHz - I2C Fast Mode */
    i2c_set_fast_mode(I2C2);
//    i2c_set_standard_mode(I2C2);

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
    i2c_set_own_7bit_slave_address(I2C2, 0x11);

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

void I2C_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
{
      while((I2C_SR2(i2c) & I2C_SR2_BUSY) != 0x00000000){};

#ifdef useI2C1
        i2c_send_start(I2C1);
#endif
#ifdef useI2C2
        i2c_send_start(I2C2);
#endif

    while(// Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(i2c) & I2C_SR1_SB) == 0x00000000)  ||     //Start condition generated.
          ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000) ||     //MSL
          ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000)       //BUSY
          );

#ifdef useI2C1
    i2c_send_7bit_address(I2C1, slaveAddress, transmissionDirection);
#endif
#ifdef useI2C2
    i2c_send_7bit_address(I2C2, slaveAddress, transmissionDirection);
#endif

    if(transmissionDirection== I2C_Direction_Transmitter)
    {

        while(
              ((I2C_SR1(i2c) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
              ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0x00000000) ||  //ADDR
              ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
              ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
              ((I2C_SR2(i2c) & I2C_SR2_TRA) == 0x00000000)      //TRA
              );
    }
    if(transmissionDirection== I2C_Direction_Receiver)
    {

        while(
              ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0x00000000) ||  //ADDR
              ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
              ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)      //MSL
              );
    }
}

void I2C_WriteData(uint32_t i2c, uint8_t data)
{
    i2c_send_data(i2c, data);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(i2c) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE  Data register empty (transmitters) 0 - Data register not empty
          ((I2C_SR1(i2c) & I2C_SR1_BTF) == 0x00000000) ||   //BTF  Byte transfer finished 0 - Data byte transfer not done
          ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY 0: No communication on the bus
          ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL  0: Slave Mode
          ((I2C_SR2(i2c) & I2C_SR2_TRA) == 0x00000000)      //TRA  0: Data bytes received  1: Data bytes transmitted
          );
}

//uint8_t I2C_ReadData(I2C_TypeDef* I2Cx)
uint8_t I2C_ReadData(uint32_t  i2c)
{

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(i2c) & I2C_SR1_RxNE) == 0x00000000) ||  //RXNE
          ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)      //MSL
          );

    uint8_t data;
//    data = I2C_ReceiveData(I2Cx);
    data = i2c_get_data(i2c);
    return data;
}




// Аппаратная реализация i2c1 - пока не работает
#ifndef useProgI2C1

static const char *i2c_msg[] = {
    "OK",
    "Address timeout",
    "Address NAK",
    "Write timeout",
    "Read timeout"
};

jmp_buf i2c_exception;

static inline TickType_t
diff_ticks(TickType_t early,TickType_t later) {

    if ( later >= early )
        return later - early;
    return ~(TickType_t)0 - early + 1 + later;
}

const char *
i2c_error(I2C_Fails fcode) {
    int icode = (int)fcode;

    if ( icode < 0 || icode > (int)I2C_Read_Timeout )
        return "Bad I2C_Fails code";
    return i2c_msg[icode];
}

void
i2c_configure(I2C_Control *dev,uint32_t i2c,uint32_t ticks) {

    dev->device = i2c;
    dev->timeout = ticks;

    i2c_peripheral_disable(dev->device);
    i2c_reset(dev->device);
    I2C_CR1(dev->device) &= ~I2C_CR1_STOP;	// Clear stop
    i2c_set_standard_mode(dev->device);	// 100 kHz mode
//	i2c_set_clock_frequency(dev->device,I2C_CR2_FREQ_36MHZ); // APB Freq

    i2c_set_clock_frequency(dev->device, 36); // APB Freq
    i2c_set_trise(dev->device,36);		// 1000 ns
    i2c_set_dutycycle(dev->device,I2C_CCR_DUTY_DIV2);
    i2c_set_ccr(dev->device,180);		// 100 kHz <= 180 * 1 /36M

//    i2c_set_own_7bit_slave_address(dev->device, 0x23); // Necessary?
    i2c_set_own_7bit_slave_address(dev->device, 0x27); // Necessary?

    i2c_peripheral_enable(dev->device);
}

void
i2c_wait_busy(I2C_Control *dev) {

    while ( I2C_SR2(dev->device) & I2C_SR2_BUSY )
        taskYIELD();			// I2C Busy

}

void
i2c_start_addr(I2C_Control *dev,uint8_t addr,enum I2C_RW rw) {
    TickType_t t0 = systicks();

    i2c_wait_busy(dev);			// Block until not busy
    I2C_SR1(dev->device) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
    i2c_clear_stop(dev->device);		// Do not generate a Stop
    if ( rw == Read )
        i2c_enable_ack(dev->device);
    i2c_send_start(dev->device);		// Generate a Start/Restart

    // Loop until ready:
        while ( !((I2C_SR1(dev->device) & I2C_SR1_SB)
      && (I2C_SR2(dev->device) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Addr_Timeout);
        taskYIELD();
    }

    // Send Address & R/W flag:
    i2c_send_7bit_address(dev->device,addr,
        rw == Read ? I2C_READ : I2C_WRITE);

    // Wait until completion, NAK or timeout
    t0 = systicks();
    while ( !(I2C_SR1(dev->device) & I2C_SR1_ADDR) ) {
        if ( I2C_SR1(dev->device) & I2C_SR1_AF ) {
            i2c_send_stop(dev->device);
            (void)I2C_SR1(dev->device);
            (void)I2C_SR2(dev->device); 	// Clear flags
            // NAK Received (no ADDR flag will be set here)
            longjmp(i2c_exception,I2C_Addr_NAK);
        }
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Addr_Timeout);
        taskYIELD();
    }

    (void)I2C_SR2(dev->device);		// Clear flags
}

void
i2c_write(I2C_Control *dev,uint8_t byte) {
    TickType_t t0 = systicks();

    i2c_send_data(dev->device,byte);
    while ( !(I2C_SR1(dev->device) & (I2C_SR1_BTF)) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Write_Timeout);
        taskYIELD();
    }
}

uint8_t
i2c_read(I2C_Control *dev,bool lastf) {
    TickType_t t0 = systicks();

    if ( lastf )
        i2c_disable_ack(dev->device);	// Reading last/only byte

    while ( !(I2C_SR1(dev->device) & I2C_SR1_RxNE) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Read_Timeout);
        taskYIELD();
    }

    return i2c_get_data(dev->device);
}

void
i2c_write_restart(I2C_Control *dev,uint8_t byte,uint8_t addr) {
    TickType_t t0 = systicks();

    taskENTER_CRITICAL();
    i2c_send_data(dev->device,byte);
    // Must set start before byte has written out
    i2c_send_start(dev->device);
    taskEXIT_CRITICAL();

    // Wait for transmit to complete
    while ( !(I2C_SR1(dev->device) & (I2C_SR1_BTF)) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Write_Timeout);
        taskYIELD();
    }

    // Loop until restart ready:
    t0 = systicks();
        while ( !((I2C_SR1(dev->device) & I2C_SR1_SB)
      && (I2C_SR2(dev->device) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Addr_Timeout);
        taskYIELD();
    }

    // Send Address & Read command bit
    i2c_send_7bit_address(dev->device,addr,I2C_READ);

    // Wait until completion, NAK or timeout
    t0 = systicks();
    while ( !(I2C_SR1(dev->device) & I2C_SR1_ADDR) ) {
        if ( I2C_SR1(dev->device) & I2C_SR1_AF ) {
            i2c_send_stop(dev->device);
            (void)I2C_SR1(dev->device);
            (void)I2C_SR2(dev->device); 	// Clear flags
            // NAK Received (no ADDR flag will be set here)
            longjmp(i2c_exception,I2C_Addr_NAK);
        }
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Addr_Timeout);
        taskYIELD();
    }

    (void)I2C_SR2(dev->device);		// Clear flags
}


void init_I2C1(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
              GPIO_I2C1_SCL | GPIO_I2C1_SDA);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C1);

    /* APB1 is running at 36MHz. */
    i2c_set_clock_frequency(I2C1, 36);

    /* 400KHz - I2C Fast Mode */
    i2c_set_fast_mode(I2C1);
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
    i2c_set_own_7bit_slave_address(I2C1, 0x11);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C1);
}

void I2C_POD_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
{

    uint32_t reg32 __attribute__((unused));

//    i2c_send_start(i2c);
//    toUart("i2c_send_start");

    /* Waiting for START is send and switched to master mode. */
//    while (!((I2C_SR1(i2c) & I2C_SR1_SB) & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

//    i2c_send_7bit_address(i2c, slaveAddress, transmissionDirection);
//    toUart("i2c_send_7bit_address");
//    /* Waiting for address is transferred. */
//    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
    i2c_start_addr(&i2c_,0x56,Write);
    toUart("i2c_start_addr");


    /* Cleaning ADDR condition sequence. */
    reg32 = I2C_SR2(i2c);
}

void send_to_POT(uint8_t data)
{
    //AR - только запись
    //57 - чтение
    //56 - запись
    //56 02 80 выбор WCR
    //56 02 00 выбор DR
    //The WCR is a volatile register and is written with the contents of the nonvolatile Data Register (DR) on power-up.

    //WCR WRITE OPERATION                 start 56 02 80 stop start 56 00 data stop
    //WCR INCREMENT/DECREMENT OPERATION - start 56 02 80 stop start 5E 00 ??
    //WCR READ OPERATION                  start 56 02 80 stop start 56 00 start 57 data stop
    //The WCR is also written during a write to DR
    //DR WRITE OPERATION                  start 56 02 00 stop start 56 00 data stop
    //DR READ OPERATION                   start 56 02 00 stop start 56 00 start 57 data stop

    //start 56 02 00 stop start 56 00 data stop

    //void i2c_send_start(uint32_t i2c)
    //{
    //	I2C_CR1(i2c) |= I2C_CR1_START; !!!!!!!!!!!!!!!!!!!!!!!!!!! тут все
    //}

//DR WRITE OPERATION  start 56 02 00 stop start 56 00 data stop
    I2C_POD_StartTransmission(I2C1, I2C_Direction_Transmitter, 0x56);

    i2c_write(&i2c_,0x02&0x0FF);
//    i2c_send_data(I2C1,0x02);
    toUart("i2c_send_data 02");

//    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    i2c_send_data(I2C1,0x00);
    toUart("i2c_send_data 00");
    /* After the last byte we have to wait for TxE too. */
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

    /* Send STOP condition. */
    i2c_send_stop(I2C1);
    toUart("i2c_send_stop");

    I2C_POD_StartTransmission(I2C1, I2C_Direction_Transmitter, 0x56);

    i2c_send_data(I2C1,0x00);
    toUart("i2c_send_data 00");

    vTaskDelay(pdMS_TO_TICKS(1));

    i2c_send_data(I2C1,data);
    toUart("i2c_send_data data");

    vTaskDelay(pdMS_TO_TICKS(1));

    i2c_send_stop(I2C1);
    toUart("i2c_send_stop");

    vTaskDelay(pdMS_TO_TICKS(1));

}
#endif



#ifdef useProgI2C1
// Программная реализация i2c1

volatile uint8_t i2c_frame_error=0;

void i2c_init (void) // функция инициализации шины
{
//    delay_setup();

    i2c_stop_cond();   // стоп шины
    i2c_stop_cond();   // стоп шины
}

void SCL_in (void) //функция отпускания SCL в 1, порт на вход
{
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SCL);
}

void SCL_out_DOWN (void) //функция притягивания SCL в 0
{
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO_I2C1_SCL);
    SCL_O;
}

void SDA_in (void) //функция отпускания SDA в 1, порт на вход
{
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_I2C1_SDA);
}

void SDA_out_DOWN (void) //функция притягивания SDA в 0
{
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO_I2C1_SDA);
    SDA_O;
}

#include <stdint.h>
void i2c_stop_cond (void)  // функция генерации условия стоп
{
    uint16_t SCL, SDA;
    SCL_out_DOWN(); // притянуть SCL (лог.0)

    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SDA_out_DOWN(); // притянуть SDA (лог.0)

    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);

    SCL_in(); // отпустить SCL (лог.1)

    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);

    SDA_in(); // отпустить SDA (лог.1)
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);

    // проверка фрейм-ошибки
    i2c_frame_error=0;		// сброс счётчика фрейм-ошибок
    SCL_in();
    SCL=SCL_I;
    SDA_in();
    SDA=SDA_I;
    if (SCL == 0) i2c_frame_error++;   // проверяем, чтобы на ноге SDA была лог.1, иначе выдаём ошибку фрейма
    if (SDA == 0) i2c_frame_error++;   // проверяем, чтобы на ноге SCL была лог.1, иначе выдаём ошибку фрейма
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(40);
}

void i2c_start_cond (void)  // функция генерации условия старт
{
    SDA_out_DOWN(); // притянуть SDA (лог.0)
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SCL_out_DOWN(); // притянуть SCL (лог.0)
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
}

void i2c_restart_cond (void)   // функция генерации условия рестарт
{
    SDA_in(); // отпустить SDA (лог.1)
    delay_us(10);
    SCL_in(); // отпустить SCL (лог.1)
    delay_us(10);
    SDA_out_DOWN(); // притянуть SDA (лог.0)
    delay_us(10);
    SCL_out_DOWN(); // притянуть SCL (лог.0)
    delay_us(10);
}

uint8_t i2c_send_byte (uint8_t data)  // функция  отправки байта
{
    uint8_t i;
    uint8_t ack = 1;           //АСК, если АСК=1 – произошла ошибка
    uint16_t SDA;
    for (i=0; i < 8; i++)
    {
        if (data & 0x80)
        {
            SDA_in(); // лог.1
        }
        else
        {
            SDA_out_DOWN(); // Выставить бит на SDA (лог.0
        }

        vTaskDelay(pdMS_TO_TICKS(1));
//        delay_us(10);
        SCL_in();      // Записать его импульсом на SCL       // отпустить SCL (лог.1)

        vTaskDelay(pdMS_TO_TICKS(1));
//        delay_us(10);
        SCL_out_DOWN();     // притянуть SCL (лог.0)
        data<<=1;      // сдвигаем на 1 бит влево
    }
    SDA_in();          // отпустить SDA (лог.1), чтобы ведомое устройство смогло сгенерировать ACK

    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);

    SCL_in();          // отпустить SCL (лог.1), чтобы ведомое устройство передало ACK
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SDA=SDA_I;
    if (SDA == 0x00) ack=1;
    else ack=0;         // Считать ACK

    SCL_out_DOWN();         // притянуть SCL (лог.0)  // приём ACK завершён

    return ack;        // вернуть ACK (0) или NACK (1)
}

void testImpuls(void *args)
{
    (void)args;

    for(;;)
    {
        SCL_out_DOWN();
        SDA_out_DOWN();
        vTaskDelay(pdMS_TO_TICKS(10));
        SCL_in();
        SDA_in();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

uint8_t i2c_get_byte (uint8_t last_byte) // функция принятия байта
{
    uint8_t i, res=0;
    uint16_t SDA;
    SDA_in();                // отпустить SDA (лог.1)
    for (i=0;i<8;i++)
    {
        res<<=1;

        SCL_in();            // отпустить SCL (лог.1)      //Импульс на SCL
        vTaskDelay(pdMS_TO_TICKS(1));
//        delay_us(10);

        SDA_in();
        SDA=SDA_I;
        if (SDA == 1) res=res|0x01;    // Чтение SDA в переменную  Если SDA=1 то записываем 1
        SCL_out_DOWN();                     // притянуть SCL (лог.0)

        vTaskDelay(pdMS_TO_TICKS(1));
//        delay_us(10);
    }

    if (last_byte == 0)
    {
        SDA_out_DOWN();  // притянуть SDA (лог.0) Подтверждение, ACK, будем считывать ещё один байт
    }
    else
    {
        SDA_in();   // отпустить SDA (лог.1) Без подтверждения, NACK, это последний считанный байт
    }
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SCL_in(); // отпустить SCL (лог.1)
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SCL_out_DOWN(); // притянуть SCL (лог.0)
    vTaskDelay(pdMS_TO_TICKS(1));
//    delay_us(10);
    SDA_in(); // отпустить SDA (лог.1)

    return res; // вернуть считанное значение
}

void send_Programm_to_POT(uint8_t data)
{
    //AR - только запись
    //57 - чтение
    //56 - запись
    //56 02 80 выбор WCR
    //56 02 00 выбор DR
    //The WCR is a volatile register and is written with the contents of the nonvolatile Data Register (DR) on power-up.

    //WCR WRITE OPERATION                 start 56 02 80 stop start 56 00 data stop
    //WCR INCREMENT/DECREMENT OPERATION - start 56 02 80 stop start 5E 00 ??
    //WCR READ OPERATION                  start 56 02 80 stop start 56 00 start 57 data stop
    //The WCR is also written during a write to DR
    //DR WRITE OPERATION                  start 56 02 00 stop start 56 00 data stop
    //DR READ OPERATION                   start 56 02 00 stop start 56 00 start 57 data stop

    //start 56 02 00 stop start 56 00 data stop

//DR WRITE OPERATION  start 56 02 00 stop start 56 00 data stop
    sprintf(buf, "%d", data);
    toUart_(buf);

    i2c_start_cond();
    toUart_("i2c_start_cond");

    toUart__(i2c_send_byte (0x56));

    toUart__(i2c_send_byte (0x02));

    toUart__(i2c_send_byte (0x00));

    i2c_stop_cond();
    toUart_("i2c_stop_cond");

    i2c_start_cond();
    toUart_("i2c_start_cond");

    toUart__(i2c_send_byte (0x00));

    toUart__(i2c_send_byte (data));

    i2c_stop_cond();
    toUart_("i2c_stop_cond");
}

void send_Programm_to_POT1(uint8_t data)
{
    //WCR WRITE OPERATION                 start 56 02 80 stop start 56 00 data stop
    uint8_t tmp;
    char buf[10];

    i2c_start_cond();
    tmp = i2c_send_byte (0x56);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (0x56)");
    stringTo_diagnostic_Usart1(buf);

    tmp = i2c_send_byte (0x02);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (0x02)");
    stringTo_diagnostic_Usart1(buf);

    tmp = i2c_send_byte (0x80);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (0x80)");
    stringTo_diagnostic_Usart1(buf);

    i2c_stop_cond();

    i2c_start_cond();

    tmp = i2c_send_byte (0x56);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (0x56)");
    stringTo_diagnostic_Usart1(buf);

    tmp = i2c_send_byte (0x00);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (0x00)");
    stringTo_diagnostic_Usart1(buf);

    tmp = i2c_send_byte (data);
//    tmp = i2c_send_byte (0xF0);
    sprintf(buf, "%X", tmp);
    stringTo_diagnostic_Usart1("i2c_send_byte (data)");
    stringTo_diagnostic_Usart1(buf);

    sprintf(buf, "%d", data);
    stringTo_diagnostic_Usart1("i2c_value");
    stringTo_diagnostic_Usart1(buf);

    i2c_stop_cond();
    sprintf(buf, "%d", i2c_frame_error);
    stringTo_diagnostic_Usart1("i2c_frame_error");
    stringTo_diagnostic_Usart1(buf);
}

#endif

