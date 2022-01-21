/*
 * testi2c.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Dmitry Donskih
 */

#include "main.h"
#include "testi2c.h"
#include <ips_globals.hpp>



#ifdef SC4BOPLA

#define I2C_IFLAG_OFF	0
#define I2C_IFLAG_ON	1


#define TLC59116_BASE_PWM	0x02

#define MCP23017_BASE_ADDRESS	0x20


extern I2C_HandleTypeDef hi2c3;

static I2C_HandleTypeDef *valid=NULL;

static const uint32_t _i2c_io_timeout=1500;

static const char* Color(uint8_t val);


static void scan4KBICs();

#ifdef I2C3_IN_NONBLOCKING
static int i2cbusy_flag=0;
static int i2cerror_flag=0;
#define I2C_BUSY		i2cbusy_flag=1
#define I2C_FREE		i2cbusy_flag=0
#else
#define I2C_BUSY
#define I2C_FREE
#endif

static int ips_errno=0;
static char output_suppress=1;

#define I2C_DBG_SUPPRESS output_suppress=0;
#define I2C_DBG_RELEASE output_suppress=1;

//static void i2cReset(void);
static void wait4i2c(I2C_HandleTypeDef *handle);
/*
void kbON(void){
	HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_SET);
}

void kbOFF(void){
	HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_RESET);
}
*/
void writeI2CBlocking(I2C_HandleTypeDef *hi2c,uint8_t ic_addr,uint8_t reg,uint8_t value){
	valid=hi2c;
	uint8_t data[2]={reg,value};
	while(HAL_I2C_GetState(hi2c)!=HAL_I2C_STATE_READY);
	I2C_BUSY;
	if (HAL_I2C_Master_Transmit(hi2c, ic_addr,&data[0], 2,_i2c_io_timeout)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		I2C_FREE;
		if(output_suppress)ipsSystem::debug->printfl("Failed to write as Master of I2C3 i.f. ERRNO=%d \n",ips_errno);
	}
	wait4i2c(hi2c);
}

int testi2c(I2C_HandleTypeDef *hi2c){
	//HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_SET);
	valid=hi2c;
	HAL_Delay(500);
	while(HAL_I2C_GetState(hi2c)!=HAL_I2C_STATE_READY);

	uint8_t init_leds[2],ldg1_on[2],readbuf[10];
	init_leds[0]=0x00;
	init_leds[1]=0x0F;
	ldg1_on[0]=0x14;
	ldg1_on[1]=0x55;
	readbuf[0]=0;
	readbuf[1]=0;
	uint8_t intena=0xFF;

	int ips_errno=0;
	I2C_BUSY;
	if (HAL_I2C_Master_Transmit(hi2c, 0xD0, &init_leds[0], 2, _i2c_io_timeout)!=HAL_OK){
		ips_errno=HAL_I2C_GetError(hi2c);
		I2C_FREE;
		ipsSystem::debug->printfl("Failed to init LED Controller @ 0xD0 as Master of I2C3 i.f. ERRNO=%d \n",ips_errno);
	}
	wait4i2c(hi2c);

	I2C_BUSY;
	if (HAL_I2C_Master_Transmit(hi2c, 0xC0, &ldg1_on[0], 2,_i2c_io_timeout)!=HAL_OK){
		ips_errno=HAL_I2C_GetError(hi2c);
		I2C_FREE;
		ipsSystem::debug->printfl("Failed to ldg1_on LED Controller @ 0xC0 as Master of I2C3 i.f. ERRNO=%d \n",ips_errno);
	}
	wait4i2c(hi2c);

	scan4KBICs();

//	I2C_BUSY;
//	if (HAL_I2C_Mem_Write(hi2c, 0x40,0x04,I2C_MEMADD_SIZE_8BIT, &intena, 1,_i2c_io_timeout)!=HAL_OK){
//		ips_errno=HAL_I2C_GetError(hi2c);
//		I2C_FREE;
//		ipsSystem::debug->printfl("Failed to enable interrupts on KB controller @ 0x40 as Master of I2C3 i.f. ERRNO=%d \n",ips_errno);
//	}
//	wait4i2c(hi2c);
//	I2C_BUSY;
//	if (HAL_I2C_Mem_Write(hi2c, 0x40,0x05,I2C_MEMADD_SIZE_8BIT, &intena, 1,_i2c_io_timeout)!=HAL_OK){
//		ips_errno=HAL_I2C_GetError(hi2c);
//		I2C_FREE;
//		ipsSystem::debug->printfl("Failed to enable another interrupts on KB controller @ 0x40 as Master of I2C3 i.f. ERRNO=%d \n",ips_errno);
//	}
//	wait4i2c(hi2c);
/*
	if (HAL_I2C_Mem_Read(hi2c, 0xC1, 0x00, I2C_MEMADD_SIZE_8BIT, &readbuf[0], 1, 1500)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		Error_Handler();
	}
	if (HAL_I2C_Mem_Read(hi2c, (0x40<<0), 0x00, I2C_MEMADD_SIZE_8BIT, &readbuf[0], 1, 1500)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		Error_Handler();
	}
	if (HAL_I2C_Master_Receive(hi2c, 0xC1, readbuf, 10, 1500)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		Error_Handler();
	}
	if (HAL_I2C_Master_Receive(hi2c, 0x41, readbuf, 10, 1500)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		Error_Handler();
	}
*/
	/*
	I2C_BUSY;
	if (HAL_I2C_Master_Receive_IT(hi2c, 0xC1, readbuf, 2)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		I2C_FREE;
		Error_Handler();
	}
	wait4i2c(hi2c);

	I2C_BUSY;
	if (HAL_I2C_Mem_Read_IT(hi2c, 0x40,0x00,I2C_MEMADD_SIZE_8BIT, &readbuf[0], 1)==HAL_ERROR){
		ips_errno=HAL_I2C_GetError(hi2c);
		I2C_FREE;
		Error_Handler();
	}
	wait4i2c(hi2c);
	*/
	//HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_RESET);
	valid=NULL;
	return ips_errno;

}

void ledSetupPWMMode(void){
	for(int i=0x14;i<=0x17;i++){
		writeI2CBlocking(&hi2c3, 0xC0, i, 0xAA);
		writeI2CBlocking(&hi2c3, 0xC2, i, 0xAA);
	}
	if(output_suppress)ipsSystem::debug->printfl("It must be like activation of PWM @ 0xC0 and 0xC2 \n");
}

void ledPWM(uint8_t led,uint8_t color,uint8_t value){
	if((led>16)||(led<=0)||(color>1)){
		return;
	}
	uint8_t ic_reg=TLC59116_BASE_PWM+(((led-1)%8)*2+color);
	writeI2CBlocking(&hi2c3, (led>8)?0xC2:0xC0,ic_reg, value);
	if(output_suppress)ipsSystem::debug->printfl("It looks like you trying to set %s LED%d at %d \n",Color(color),led,value);
	return;
}

void ledON(uint8_t led,uint8_t color){
	ledPWM(led, color, IPS_LED_ON);
}

void ledOFF(uint8_t led,uint8_t color){
	ledPWM(led, color, IPS_LED_OFF);
}

void ledAllON(void){
	for (int i=1;i<=16;i++){
		ledON(i,IPS_LED_RED);
		ledON(i,IPS_LED_GREEN);
	}
}

void ledAllOFF(void){
	for (int i=1;i<=16;i++){
		ledOFF(i,IPS_LED_RED);
		ledOFF(i,IPS_LED_GREEN);
	}
}

void ledAllGreen(uint8_t value){
	for (int i=1;i<=16;i++){
		ledPWM(i, IPS_LED_GREEN, value);
	}
}

void ledAllRed(uint8_t value){
	for (int i=1;i<=16;i++){
		ledPWM(i, IPS_LED_RED, value);
	}
}

/*
void i2cReset(void){
	HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(KB_RESET_GPIO_Port, KB_RESET_Pin, GPIO_PIN_RESET);
}
*/
#ifdef I2C3_IN_NONBLOCKING
void wait4i2c(I2C_HandleTypeDef *handle){
	while((i2cbusy_flag)&&(!i2cerror_flag)){};
	i2cbusy_flag=0;
	i2cerror_flag=0;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(valid==hi2c){
		i2cbusy_flag=0;
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(valid==hi2c){
		i2cbusy_flag=0;
	}
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(valid==hi2c){
		i2cbusy_flag=0;
	}
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(valid==hi2c){
		i2cbusy_flag=0;
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	if(valid==hi2c){
		i2cerror_flag=1;
		ips_errno=HAL_I2C_GetError(valid);
		valid->ErrorCode=0;
	}
}
#else
void wait4i2c(I2C_HandleTypeDef *handle){

}
#endif

const char* Color(uint8_t val){
    switch (val) {
	case 0x00: return "red";
	case 0x01: return "green";
	default: break;
    }
    return "wrong";
    }

void sysKeyboardDiag(void){
    testi2c(&hi2c3);

    I2C_DBG_SUPPRESS
    HAL_Delay(500);
    ledSetupPWMMode();
    ledAllON();
    HAL_Delay(1000);
    ledAllOFF();
    HAL_Delay(500);

    int ld_num=1;
    int ld_color=IPS_LED_GREEN;
    uint8_t dir_flag=0;
    uint32_t t_begin=HAL_GetTick();
    while((HAL_GetTick()-t_begin)<30000)
    {
    ledAllGreen((dir_flag)?(ld_num):(255-ld_num));
	    ledAllRed((!dir_flag)?(ld_num):(255-ld_num));
	    ld_num++;
		    /*
		    if(ld_num>16){
			    ld_num=1;
			    ld_color=(ld_color==IPS_LED_RED)?IPS_LED_GREEN:IPS_LED_RED;
		    }
		    */
	    if(ld_num>=255){
		    ld_num=0;
		    dir_flag=~dir_flag;
	    }
	    HAL_Delay(5);
    }
    ledAllOFF();
    I2C_DBG_RELEASE
}

int i2cRegRead(uint8_t dev,uint8_t reg)
{
	uint8_t value = 0;
	while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Master_Transmit(&hi2c3, dev<<1, &reg, sizeof(reg), _i2c_io_timeout)!= HAL_OK)
	{
		return -1;
	}
	while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
	if (HAL_I2C_Master_Receive(&hi2c3, dev<<1, &value, sizeof(value),_i2c_io_timeout) != HAL_OK)
	{
		return -1;
	}
	return value;

}

static void scan4KBICs(){
    int result=0;
    for (uint8_t i=0;i<0x08;i++){
	result=i2cRegRead(MCP23017_BASE_ADDRESS+i, 0x00);
	if(result>=0){
	    ipsSystem::debug->printfl("Has response @ %d. Port A direction %d\n",MCP23017_BASE_ADDRESS+i,(uint8_t)result);
	}else{
	    ipsSystem::debug->printfl("No response @ %d\n",MCP23017_BASE_ADDRESS+i);
	}
    }
    ipsSystem::debug->printfl("KB Controllers scan ends\n");
}

#endif /* SC4BOPLA */


