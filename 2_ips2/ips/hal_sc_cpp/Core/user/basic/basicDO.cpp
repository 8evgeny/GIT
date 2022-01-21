
/*
 * baseLedDO.cpp
 *
 *  Created on: 27 июл. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */
#include <basicDO.h>

#include <ips_globals.hpp>

#ifdef SC2DIOBOARD
#include <system_c.h>
#endif
#ifdef SC4BOPLA
#include "testi2c.h"

#define TLC59116_PWM	0x02
extern I2C_HandleTypeDef hi2c3;
#endif

#define LEDADDR(x) ((0x60+x)<<1)
#define READ_KBD_LEDS(device,regist) i2cRegRead(device>>1,regist) //TODO: C-codebase compliability! Rewrite

DOUnit::DOUnit() {
	init();
}

uint16_t DOUnit::getCount(void) {
	return capacity;
    }

DOUnit& DOUnit::getInstance()
    {
	static DOUnit instance;
    	return instance;
    }

void DOUnit::setDO(uint16_t output)
    {
#ifdef SC2DIOBOARD
    sysDiscreteOutputON(output&0xFF);
#endif
#ifdef SC4BOPLA

	key_id_t o_key={.w=output};
	_ledON(o_key.output, (o_key.block==LED_GREEN)?IPS_LED_GREEN:IPS_LED_RED);

#endif
    }

void DOUnit::resetDO(uint16_t output)
    {
#ifdef SC2DIOBOARD
    sysDiscreteOutputOFF(output&0xFF);
#endif
#ifdef SC4BOPLA
	key_id_t o_key={.w=output};
	_ledOFF(o_key.output, (o_key.block==LED_GREEN)?IPS_LED_GREEN:IPS_LED_RED);
#endif
    }

void DOUnit::toggleDO(uint16_t output)
    {
#ifdef SC2DIOBOARD
    sysDiscreteOutputToggle(output&0xFF);
#endif
#ifdef SC4BOPLA
	int blabla_state=state(output);
	if(blabla_state!=IPS_LED_OFF){
	    resetDO(output);
	}else{
	    setDO(output);
	}

#endif
    }

uint8_t DOUnit::state(uint16_t output)
    {
#ifdef SC2DIOBOARD
    return sysDiscreteOutputRead(output&0xFF);
#endif
#ifdef SC4BOPLA

    key_id_t o_key={.w=output};
    return _ledReadState(o_key.output, (o_key.block==LED_GREEN)?IPS_LED_GREEN:IPS_LED_RED);

#endif
    }


void DOUnit::init()
    {
#ifdef SC2DIOBOARD
    capacity=8;
#endif
#ifdef SC4BOPLA
    ledsSetup();
    //_test();
#endif
    }


#ifdef SC4BOPLA

void DOUnit::scanICs()
    {
    }

uint8_t DOUnit::getICAddress(uint16_t led_n)
    {
	for(uint16_t i=0;i<chips_count;i++){
	    if((led_n>=chips[i].first)&&(led_n<=chips[i].last))return chips[i].address;
	}
	return 0xFF;
    }

void DOUnit::ledsSetup()
    {
    uint8_t first=1;
    for (uint8_t i=0;i<0x08;i++){
	int mode=READ_KBD_LEDS(LEDADDR(i),0x00);
	if ((mode==0x11)||(mode==0x0F)){
	    writeI2CBlocking(&hi2c3, LEDADDR(i), 0x00, 0x0F);
	    for(int j=0x14;j<=0x17;j++){
		writeI2CBlocking(&hi2c3,LEDADDR(i),j,0xAA);
	    }
	    chips[chips_count].address=LEDADDR(i);
	    chips[chips_count].first=first;
	    first+=8;
	    chips[chips_count].last=first-1;
	    ipsSystem::debug->printfl("Found driver@%d, assigned to %d - %d\n",i,chips[chips_count].first,chips[chips_count].last);
	    chips_count++;
	    capacity+=8;
	}
    }
    }

void DOUnit::_ledPWM(uint8_t led, uint8_t color, uint8_t value)
    {
	if((led>capacity)||(led<=0)||(color>1)){
	    return;
    	}
    	uint8_t ic_reg=TLC59116_PWM+(((led-1)%8)*2+color);
    	writeI2CBlocking(&hi2c3, getICAddress(led),ic_reg, value);
    	return;
    }

void DOUnit::_ledON(uint8_t led, uint8_t color)
    {
	_ledPWM(led, color, IPS_LED_ON);
    }

void DOUnit::_ledOFF(uint8_t led, uint8_t color)
    {
	_ledPWM(led, color, IPS_LED_OFF);
    }

void DOUnit::_ledAllON(void)
    {
	for (uint16_t i=1;i<=capacity;i++){
    		_ledON(i,IPS_LED_RED);
    		_ledON(i,IPS_LED_GREEN);
    	}
    }

void DOUnit::_ledAllOFF(void)
    {
	for (uint16_t i=1;i<=capacity;i++){
    		_ledOFF(i,IPS_LED_RED);
    		_ledOFF(i,IPS_LED_GREEN);
    	}
    }

void DOUnit::_ledAllGreen(uint8_t value)
    {
	for (uint16_t i=1;i<=capacity;i++){
    		_ledPWM(i, IPS_LED_GREEN, value);
    	}
    }

void DOUnit::_ledAllRed(uint8_t value)
    {
	for (uint16_t i=1;i<=capacity;i++){
    		_ledPWM(i, IPS_LED_RED, value);
    	}
    }

uint8_t DOUnit::_ledReadState(uint8_t led, uint8_t color)
    {
	uint8_t ic_reg=TLC59116_PWM+(((led-1)%8)*2+color);
	int result=READ_KBD_LEDS(getICAddress(led),ic_reg);
	return ((result>15)?result:0)&0xFF;
    }

void DOUnit::_test()
    {
	_ledAllOFF();
	for (uint16_t i=1;i<=capacity;i++){
	    _ledOFF(i-1, IPS_LED_GREEN);
	    _ledON(i, IPS_LED_GREEN);
	    HAL_Delay(20);
	}
	_ledAllOFF();
	for (uint16_t i=1;i<=capacity;i++){
	    _ledOFF(i-1, IPS_LED_RED);
	    _ledON(i, IPS_LED_RED);
	    HAL_Delay(20);
	}
	_ledAllOFF();
	HAL_Delay(10);
	_ledAllON();
	HAL_Delay(30);
	_ledAllOFF();
	HAL_Delay(30);
	_ledAllGreen(0xFF);
	HAL_Delay(30);
	_ledAllOFF();
	_ledAllRed(0xFF);
	HAL_Delay(30);
	_ledAllGreen(0xFF);
	HAL_Delay(30);
	_ledAllOFF();
	_ledAllRed(0xFF);
	_ledAllOFF();
	HAL_Delay(30);
	_ledAllGreen(0xFF);
	HAL_Delay(160);
	_ledAllOFF();
}

DOUnit* DOUnit::pointer ()
{
  return this;
}

#endif

