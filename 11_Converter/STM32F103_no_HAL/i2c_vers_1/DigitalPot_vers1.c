#include "i2c.h"
#include "main.h"

static const char *i2c_msg[] = {
    "OK",
    "Address timeout",
    "Address NAK",
    "Write timeout",
    "Read timeout"
};

jmp_buf i2c_exception;

void i2c_configure(I2C_Control *dev,uint32_t i2c,uint32_t ticks) {

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

void i2c_wait_busy(I2C_Control *dev) {
    while ( I2C_SR2(dev->device) & I2C_SR2_BUSY )
        taskYIELD();			// I2C Busy
}

static inline TickType_t diff_ticks(TickType_t early,TickType_t later)
{
    if ( later >= early )
        return later - early;
    return ~(TickType_t)0 - early + 1 + later;
}

const char * i2c_error(I2C_Fails fcode)
{
    int icode = (int)fcode;

    if ( icode < 0 || icode > (int)I2C_Read_Timeout )
        return "Bad I2C_Fails code";
    return i2c_msg[icode];
}

void i2c_start_addr(I2C_Control *dev,uint8_t addr,enum I2C_RW rw)
{
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

void i2c_write(I2C_Control *dev,uint8_t byte) {
    TickType_t t0 = systicks();

    i2c_send_data(dev->device,byte);
    while ( !(I2C_SR1(dev->device) & (I2C_SR1_BTF)) ) {
        if ( diff_ticks(t0,systicks()) > dev->timeout )
            longjmp(i2c_exception,I2C_Write_Timeout);
        taskYIELD();
    }
}

uint8_t i2c_read(I2C_Control *dev,bool lastf)
{
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

void i2c_write_restart(I2C_Control *dev,uint8_t byte,uint8_t addr)
{
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

void send_to_POT_vers1(uint8_t data)
{
//DR WRITE OPERATION  start 56 02 00 stop start 56 00 data stop

//    I2C_POD_StartTransmission(I2C1, 0, 0x56); //Запись
    toUart("i2c_start_addr");
    i2c_start_addr(&i2c,0x56,Write);

    toUart("i2c_write 02");
    i2c_write(&i2c,0x02);

    i2c_write(&i2c,0x00);
    i2c_stop(&i2c);
    i2c_start_addr(&i2c,0x56,Write);
    i2c_write(&i2c,0x00);
    i2c_write(&i2c,data);
    i2c_stop(&i2c);
}




void init_I2C1_vers1(void)
{
    i2c_configure(&i2c,I2C1,1000);

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

//    i2c_configure(&i2c,I2C1,1000);
}


//void I2C_POD_StartTransmission(uint32_t i2c, uint8_t transmissionDirection,  uint8_t slaveAddress)
//{
//    uint32_t reg32 __attribute__((unused));
//    i2c_send_start(i2c);
//    toUart("i2c_send_start");
//    /* Waiting for START is send and switched to master mode. */
//    while (!((I2C_SR1(i2c) & I2C_SR1_SB) & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
//    i2c_send_7bit_address(i2c, slaveAddress, transmissionDirection);
//    toUart("i2c_send_7bit_address");
//    /* Waiting for address is transferred. */
//    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
//    /* Cleaning ADDR condition sequence. */
//    reg32 = I2C_SR2(i2c);
//}



