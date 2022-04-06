#include "delay.h"
#include "I2C.h"

Digital_POT_I2C_Def poti2c;

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
          ((I2C_SR1(i2c) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
          ((I2C_SR1(i2c) & I2C_SR1_BTF) == 0x00000000) ||   //BTF
          ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
          ((I2C_SR2(i2c) & I2C_SR2_TRA) == 0x00000000)      //TRA
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


void I2C_POD_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
{
    char buf[10];

    while((I2C_SR2(i2c) & I2C_SR2_BUSY) != 0x00000000){};

    sprintf(buf, "%04X", (uint16_t)I2C1_SR1);
    stringTo_diagnostic_Usart1("I2C1_SR1");
    stringTo_diagnostic_Usart1(buf);
    sprintf(buf, "%04X", (uint16_t)I2C1_SR2);
    stringTo_diagnostic_Usart1("I2C1_SR2");
    stringTo_diagnostic_Usart1(buf);

    i2c_send_start(i2c);

    sprintf(buf, "%04X", (uint16_t)I2C1_SR1);
    stringTo_diagnostic_Usart1("I2C1_SR1");
    stringTo_diagnostic_Usart1(buf);
    sprintf(buf, "%04X", (uint16_t)I2C1_SR2);
    stringTo_diagnostic_Usart1("I2C1_SR2");
    stringTo_diagnostic_Usart1(buf);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(i2c) & I2C_SR1_TxE) == 0x00000000)  ||  //TxE: Data register empty (transmitters)
          ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0x00000000) ||  //ADDR: Address sent
          ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL Master/slave
          ((I2C_SR2(i2c) & I2C_SR2_TRA) == 0x00000000)      //TRA
          );

    i2c_send_7bit_address(i2c, slaveAddress, transmissionDirection);

    if(transmissionDirection== I2C_Direction_Transmitter)
    {
        while( // Выход из цикла когда все флаги упадут в 0
              ((I2C_SR1(i2c) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
              ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0x00000000) ||  //ADDR
              ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
              ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
              ((I2C_SR2(i2c) & I2C_SR2_TRA) == 0x00000000)      //TRA
              );
    }
    if(transmissionDirection== I2C_Direction_Receiver)
    {
        while( // Выход из цикла когда все флаги упадут в 0
              ((I2C_SR1(i2c) & I2C_SR1_ADDR) == 0x00000000) ||  //ADDR
              ((I2C_SR2(i2c) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
              ((I2C_SR2(i2c) & I2C_SR2_MSL) == 0x00000000)      //MSL
              );
    }
}



//#define SCL     TRISB4   // I2C bus
//#define SDA     TRISB1   //
//#define SCL_IN  RB4    //
//#define SDA_IN  RB1    //

//// initialize
//SDA = SCL = 1 ;
//SCL_IN = SDA_IN = 0 ;

//// make master wait
//void i2c_dly(void) {
//    // i2c is not quite fast, needs delay time
//}

//void i2c_start(void) {
//    /* I2C start condition is defined as
//     * a High to Low Transition on the SDA line
//     * as the SCL in a high level */

//    SDA = 1;
//    i2c_dly();
//    SCL = 1;             // SCL High
//    i2c_dly();
//    SDA = 0;             // SDA level change
//    i2c_dly();
//    SCL = 0;             // SCL low, prepare to generate pulse
//    i2c_dly();
//}


//void i2c_stop(void) {
//    /* I2C stop condition is defined as
//     * a Low to High Transition on the SDA line
//     * as the SCL in a high level */

//    SDA = 0;
//    i2c_dly();
//    SCL = 1;             // SCL on
//    i2c_dly();
//    SDA = 1;             // SDA level change
//    i2c_dly();           // there is no need to make SCL off
//}

//bit i2c_tx(unsigned char data)
//{
//    /* An I2C output data is usually send out from bit 7 to 0 (MSB to LSB).
//     * Implement this by shift one bit at each time,  set level as bit's value
//     * and them make one SCL pulse.
//     * Don't forget to add extra one pulse for slave's ACK */

//    char x;
//    static bit b ;

//    // write data to slave
//    for(x=8 ; x; x --) {
//        if(data & 0x80 ) SDA = 1 ;    // 0x80 = 1000 0000, read from MSB
//        else SDA = 0 ;
//        SCL = 1 ;                    // make SCL pulse
//        data <<= 1 ;                 // make bits shift left, keep SDA high/low
//        SCL = 0 ;                    // one bit send finish
//    }

//    // Read ACK from slave
//    SDA = 1;             // SDA need to be high, slave will pull SDA low
//    SCL = 1;             // the 9th pulse of SCL
//    i2c_dly();           // wait for slave
//    b = SDA_IN ;        // possible ACK bit, if b is 0, means successful data transport
//    SCL = 0;             // finish the 9th pulse
//    return b ;
//}

//unsigned char i2c_rx(char ack){
//    /* An I2C input data is usually send out from bit 7 to 0 (MSB to LSB) by slave.
//     * First we need to check if we can pull high SCL,
//     *      make sure not under clock stretching.
//     * Next put SDA value in the Low bit, and then shift left one bit at each time,
//     *      and finish this pulse
//     * In the end, send an ACK if there is another byte need to read */

//    char x, d =0;
//    SDA = 1;                     // initialize

//    // Read from slave
//    for(x=0 ; x< 8; x ++) {
//        d <<= 1 ;                // move to next bit
//        do {
//            SCL = 1 ;            // master's SCL needs to always pull high?
//        }
//        while(SCL_IN ==0);       // wait for any SCL clock stretching
//        i2c_dly ();             // slave ready, SCL_IN back to high
//        if(SDA_IN ) d |= 1 ;    // get data, and put into d
//        SCL = 0 ;                // finish this bit
//    }

//    // Send ACK back to slave
//    if(ack) SDA = 0 ;            // ACK value is 0
//    else SDA = 1 ;
//    SCL = 1;
//    i2c_dly();                   // master need to send (N)ACK bit

//    SCL = 0;                     // SCL and SDA get initialize
//    SDA = 1;
//    return d ;
//}


//void main(){
//    // the hole process is about how to get light sensor value, not sure yet

//    i2c_start();                 // send start sequence

//    // Write message about where is light sensor's address
//    i2c_tx(0xE0 );               // SRF08 I2C address with R/W bit clear(write)
//    i2c_tx(0x01 );               // SRF08 light sensor register address

//    // Read message
//    i2c_start();                 // send a restart sequence
//    i2c_tx(0xE1 );               // SRF08 I2C address with R/W bit set(read)
//    lightsensor = i2c_rx (1);     // get light sensor and send acknowledge. Internal register address will increment automatically.
//    rangehigh = i2c_rx (1);      // get the high byte of the range and send acknowledge.
//    rangelow = i2c_rx (0);       // get low byte of the range - note we don't acknowledge the last byte.
//    i2c_stop();                 // send stop sequence
//}


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
    i2c_send_data(I2C1,0x02);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(I2C1) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
          ((I2C_SR1(I2C1) & I2C_SR1_BTF) == 0x00000000)  ||  //BTF
          ((I2C_SR2(I2C1) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(I2C1) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
          ((I2C_SR2(I2C1) & I2C_SR2_TRA) == 0x00000000)      //TRA
          );

    i2c_send_data(I2C1,0x00);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(I2C1) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
          ((I2C_SR1(I2C1) & I2C_SR1_BTF) == 0x00000000)  ||  //BTF
          ((I2C_SR2(I2C1) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(I2C1) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
          ((I2C_SR2(I2C1) & I2C_SR2_TRA) == 0x00000000)      //TRA
          );

    i2c_send_stop(I2C1);

    I2C_POD_StartTransmission(I2C1, I2C_Direction_Transmitter, 0x56);

    i2c_send_data(I2C1,0x00);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(I2C1) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
          ((I2C_SR1(I2C1) & I2C_SR1_BTF) == 0x00000000)  ||  //BTF
          ((I2C_SR2(I2C1) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(I2C1) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
          ((I2C_SR2(I2C1) & I2C_SR2_TRA) == 0x00000000)      //TRA
          );

    i2c_send_data(I2C1,data);

    while( // Выход из цикла когда все флаги упадут в 0
          ((I2C_SR1(I2C1) & I2C_SR1_TxE) == 0x00000000)  ||  //TXE
          ((I2C_SR1(I2C1) & I2C_SR1_BTF) == 0x00000000)  ||  //BTF
          ((I2C_SR2(I2C1) & I2C_SR2_BUSY) == 0x00000000) ||  //BUSY
          ((I2C_SR2(I2C1) & I2C_SR2_MSL) == 0x00000000)  ||  //MSL
          ((I2C_SR2(I2C1) & I2C_SR2_TRA) == 0x00000000)      //TRA
          );

    i2c_send_stop(I2C1);
}
