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

void I2C_StartTransm(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress)
{
    // На всякий слуыай ждем, пока шина осовободится
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    // Генерируем старт - тут все понятно )
    I2C_GenerateSTART(I2Cx, ENABLE);
    // Ждем пока взлетит нужный флаг
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    // Посылаем адрес подчиненному  //возможно тут нужен сдвиг влево  //судя по исходникам - да, нужен сдвиг влево
    //http://microtechnics.ru/stm32-ispolzovanie-i2c/#comment-8109
    I2C_Send7bitAddress(I2Cx, slaveAddress<<1, transmissionDirection);
    // А теперь у нас два варианта развития событий - в зависимости от выбранного направления обмена данными
    if(transmissionDirection== I2C_Direction_Transmitter)
    {
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    if(transmissionDirection== I2C_Direction_Receiver)
    {
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

void I2C_WriteDat(I2C_TypeDef* I2Cx, uint8_t data)
{
    // Просто вызываем готоваую функцию из SPL и ждем, пока данные улетят
    I2C_SendData(I2Cx, data);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_ReadDat(I2C_TypeDef* I2Cx)
{
    // Тут картина похожа, как только данные пришли быстренько считываем их и возвращаем
    while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    uint8_t data;
    data = I2C_ReceiveData(I2Cx);
    return data;
}



Digital_POT_I2C_Def poti2c;

void digitalPOT_I2C_init(uint8_t pot_Addr)
{
  poti2c.Addr = pot_Addr;
  init_I2C1(); // Wire.begin();

}

void digitalPOT_send_data(uint8_t byte1, uint8_t byte2, uint8_t byte3 )
{
    I2C_StartTransm (I2C1, I2C_Direction_Transmitter, poti2c.Addr);
    I2C_WriteDat(I2C1,(int)byte1);
    I2C_WriteDat(I2C1,(int)byte2);
    I2C_WriteDat(I2C1,(int)byte3);
    I2C_GenerateSTOP(I2C1, ENABLE);
}




//uint8_t Addr;
//uint8_t Access_Control_Register_byte1;
//uint8_t Access_Control_Register_byte2;
//uint8_t Access_Control_Register_byte3;
//uint8_t MEMORY_REGISTER_DR_7_bit;
//uint8_t WIPER_CONTROL_REGISTER_WCR_7_bit;
