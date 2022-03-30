#include "main.h"
#include "digitalPOT.h"


//void LCDI2C_expanderWrite(uint8_t _data){
//	I2C_StartTransmission (I2C1, I2C_Direction_Transmitter, lcdi2c.Addr); //Wire.beginTransmission(_Addr);
//	I2C_WriteData(I2C1, (int)(_data) | lcdi2c.backlightval);  //printIIC((int)(_data) | _backlightval);
//	I2C_GenerateSTOP(I2C1, ENABLE); //Wire.endTransmission();
//}


//I2C_StartTransmission (I2C1, I2C_Direction_Transmitter, lcdi2c.Addr);
//I2C_GenerateSTOP(I2C1, ENABLE);
//I2C_GenerateSTART(I2C1, ENABLE)

Digital_POT_I2C_Def poti2c;

void digitalPOT_I2C_init(uint8_t pot_Addr)
{
  poti2c.Addr = pot_Addr;
  init_I2C1(); // Wire.begin();

}

void digitalPOT_send_data(uint8_t byte1, uint8_t byte2, uint8_t byte3 )
{
    I2C_StartTransmission (I2C1, I2C_Direction_Transmitter, poti2c.Addr);
    I2C_WriteData(I2C1,(int)byte1);
    I2C_WriteData(I2C1,(int)byte2);
    I2C_WriteData(I2C1,(int)byte3);
    I2C_GenerateSTOP(I2C1, ENABLE);
}




//uint8_t Addr;
//uint8_t Access_Control_Register_byte1;
//uint8_t Access_Control_Register_byte2;
//uint8_t Access_Control_Register_byte3;
//uint8_t MEMORY_REGISTER_DR_7_bit;
//uint8_t WIPER_CONTROL_REGISTER_WCR_7_bit;
