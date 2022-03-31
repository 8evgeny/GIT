#include "main.h"
#include "I2C.h"
#include "delay.h"

void digitaPOT(void *args)
{
    (void)args;
    DelayMC(50000);
#if 0
void i2c_reset(uint32_t i2c);
void i2c_peripheral_enable(uint32_t i2c);
void i2c_peripheral_disable(uint32_t i2c);
void i2c_send_start(uint32_t i2c);
void i2c_send_stop(uint32_t i2c);
void i2c_clear_stop(uint32_t i2c);
void i2c_set_own_7bit_slave_address(uint32_t i2c, uint8_t slave);
void i2c_set_own_10bit_slave_address(uint32_t i2c, uint16_t slave);
void i2c_set_own_7bit_slave_address_two(uint32_t i2c, uint8_t slave);
void i2c_enable_dual_addressing_mode(uint32_t i2c);
void i2c_disable_dual_addressing_mode(uint32_t i2c);
void i2c_set_clock_frequency(uint32_t i2c, uint8_t freq);
void i2c_send_data(uint32_t i2c, uint8_t data);
void i2c_set_fast_mode(uint32_t i2c);
void i2c_set_standard_mode(uint32_t i2c);
void i2c_set_ccr(uint32_t i2c, uint16_t freq);
void i2c_set_trise(uint32_t i2c, uint16_t trise);
void i2c_send_7bit_address(uint32_t i2c, uint8_t slave, uint8_t readwrite);
uint8_t i2c_get_data(uint32_t i2c);
void i2c_enable_interrupt(uint32_t i2c, uint32_t interrupt);
void i2c_disable_interrupt(uint32_t i2c, uint32_t interrupt);
void i2c_enable_ack(uint32_t i2c);
void i2c_disable_ack(uint32_t i2c);
void i2c_nack_next(uint32_t i2c);
void i2c_nack_current(uint32_t i2c);
void i2c_set_dutycycle(uint32_t i2c, uint32_t dutycycle);
void i2c_enable_dma(uint32_t i2c);
void i2c_disable_dma(uint32_t i2c);
void i2c_set_dma_last_transfer(uint32_t i2c);
void i2c_clear_dma_last_transfer(uint32_t i2c);
void i2c_transfer7(uint32_t i2c, uint8_t addr, const uint8_t *w, size_t wn, uint8_t *r, size_t rn);
void i2c_set_speed(uint32_t i2c, enum i2c_speeds speed, uint32_t clock_megahz);

AR - только запись
57 - чтение
56 - запись
56 02 80 выбор WCR
56 02 00 выбор DR
The WCR is a volatile register and is written with the contents of the nonvolatile Data Register (DR) on power-up.

WCR WRITE OPERATION                 start 56 02 80 stop start 56 00 data stop

WCR INCREMENT/DECREMENT OPERATION - start 56 02 80 stop start 5E 00 ??

WCR READ OPERATION                  start 56 02 80 stop start 56 00 start 57 data stop

The WCR is also written during a write to DR

DR WRITE OPERATION                  start 56 02 00 stop start 56 00 data stop

DR READ OPERATION                   start 56 02 00 stop start 56 00 start 57 data stop
#endif


//    init_I2C1();

//start 56 02 00 stop start 56 00 data stop

//void i2c_send_start(uint32_t i2c)
//{
//	I2C_CR1(i2c) |= I2C_CR1_START; !!!!!!!!!!!!!!!!!!!!!!!!!!! тут все
//}


//    I2C_POD_StartTransm (I2C1, I2C_Direction_Transmitter, 0x56);
//    while(!I2C_CheckEvent_(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//    i2c_send_data(I2C1,0x02);







    for(;;)
    {
//        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
