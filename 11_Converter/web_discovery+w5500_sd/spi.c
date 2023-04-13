#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "spi.h"


uint8_t W5500_rxtx(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,data);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI1);
}



void spiW5500_init(void)
{

	// Initialize SPI and GPIO
	RCC_APBPeriphClockCmd_SPI_W5500(RCC_APBPeriph_SPI_W5500, ENABLE);
	RCC_APBPeriphClockCmd_CS_W5500(RCC_APB2Periph_GPIO_W5500_CS, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	// MOSI & CLK
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_SCK | GPIO_Pin_SPI_W5500_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

	// MISO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

	// CS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_CS_W5500;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_W5500_CS, &GPIO_InitStruct);


	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_W5500, &SPI_InitStruct);


	SPI_Cmd(SPI_W5500, ENABLE);

}
