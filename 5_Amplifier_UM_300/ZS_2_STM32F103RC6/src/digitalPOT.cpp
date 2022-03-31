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



#endif
//                    typedef struct
//                    {
//                        uint8_t Addr;
//                        uint8_t Access_Control_Register_byte1;
//                        uint8_t Access_Control_Register_byte2;
//                        uint8_t Access_Control_Register_byte3;
//                        uint8_t MEMORY_REGISTER_DR_7_bit;
//                        uint8_t WIPER_CONTROL_REGISTER_WCR_7_bit;
//                    } Digital_POT_I2C_Def ;

//    digitalPOT_I2C_init(0x11);
//    digitalPOT_send_data(0x11,0x22,0x44);







    for(;;)
    {
//        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
