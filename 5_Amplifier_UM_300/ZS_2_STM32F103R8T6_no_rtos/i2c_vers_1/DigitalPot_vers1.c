#include "i2c.h"
#include "main.h"



static I2C_Control i2c;			// I2C Control struct
static volatile bool readf = false; 	// ISR flag
static volatile int isr_count = 0;	// ISR count


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


void init_I2C1_vers1(void)
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

    i2c_send_start(i2c);
    toUart("i2c_send_start");

    /* Waiting for START is send and switched to master mode. */
    while (!((I2C_SR1(i2c) & I2C_SR1_SB) & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    i2c_send_7bit_address(i2c, slaveAddress, transmissionDirection);
    toUart("i2c_send_7bit_address");

    /* Waiting for address is transferred. */
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));

    /* Cleaning ADDR condition sequence. */
    reg32 = I2C_SR2(i2c);
}


void send_to_POT_vers1(uint8_t data)
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

    I2C_POD_StartTransmission(I2C1, 0, 0x56); //Запись

    i2c_send_data(I2C1,0x02);
    toUart("i2c_send_data 02");
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    i2c_send_data(I2C1,0x00);
    toUart("i2c_send_data 00");
    /* After the last byte we have to wait for TxE too. */
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

    /* Send STOP condition. */
    i2c_send_stop(I2C1);
    toUart("i2c_send_stop");

    I2C_POD_StartTransmission(I2C1, 0, 0x56);

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
