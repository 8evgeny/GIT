/*
 * basicDI.cpp
 *
 *  Created on: 15 сент. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */
#include "main.h"

#include <basicDI.hpp>
#include <testi2c.h>
#include <ips_globals.hpp>
#include <climits>

#include <basicDO.h>
#include <system/proxy/keyboard_handler.h>
// Registers
#define REGISTER_IODIRA		0x00
#define REGISTER_IODIRB		0x01
#define REGISTER_IPOLA		0x02
#define REGISTER_IPOLB		0x03
#define REGISTER_GPINTENA	0x04
#define REGISTER_GPINTENB	0x05
#define REGISTER_DEFVALA	0x06
#define REGISTER_DEFVALB	0x07
#define REGISTER_INTCONA	0x08
#define REGISTER_INTCONB	0x09
//	IOCON			0x0A
//	IOCON			0x0B
#define REGISTER_GPPUA		0x0C
#define REGISTER_GPPUB		0x0D
#define REGISTER_INTFA		0x0E	//pin caused capture
#define REGISTER_INTFB		0x0F
#define REGISTER_INTCAPA	0x10    //captured
#define REGISTER_INTCAPB	0x11
#define REGISTER_GPIOA		0x12
#define REGISTER_GPIOB		0x13
#define REGISTER_OLATA		0x14
#define REGISTER_OLATB		0x15

#define SCAN_I2C_TIMEOUT	1500

#define KB_INTERRUPT_EVENT	0x01

#define KB_EVENT_RISE		KeyboardRise
#define KB_EVENT_FALL		KeyboardFall
#define KB_EV_MASK		0x0F

#define KBDADDR(x) ((0x20+x)<<1)

static TaskHandle_t runner=nullptr;

extern I2C_HandleTypeDef hi2c3;
extern ufw::KeyboardHandler* input_handle_pointer;

DIUnit& DIUnit::GetInstance()
    {
	static DIUnit instance;
	return instance;
    }

void DIUnit::run(void *par)
    {
    static_cast<DIUnit*>(par)->refreshTask();
    }

DIUnit* DIUnit::pointer ()
{
  return this;
}

DIUnit::DIUnit()
    {
    init();
    xTaskCreate(run, "I/O task", 512, (void*)this, osPriorityNormal, &handle);
    }

void DIUnit::refreshTask()
    {
//    while(1){
//	osDelay(1000);
//    }
#ifdef SC4BOPLA
    runner=handle;
    uint32_t notification=0;
    uint16_t ic_reg_intf=0;
    uint16_t ic_reg_gpio=0;
    while(1){
//	xTaskNotifyWait(0x00,ULONG_MAX , &notification, portMAX_DELAY);
	osDelay(100);
	//for(int i=0;i<chips_count;i++)
	ic_reg_intf=(uint16_t)ReadPair(chips[0].address,REGISTER_INTFA);
	ic_reg_gpio=(uint16_t)ReadPair(chips[0].address,REGISTER_GPIOA);
	ic_reg_intf|=((uint16_t)ReadPair(chips[1].address,REGISTER_INTFA)<<16);
	ic_reg_gpio|=((uint16_t)ReadPair(chips[1].address,REGISTER_GPIOA)<<16);
	if(ic_reg_intf!=0)
	  {
	  ipsSystem::debug->printfl("INTF=%b GPIO=%b \n",ic_reg_intf,ic_reg_gpio);
	  CreateEvent(ic_reg_gpio, ic_reg_intf);
	}
    }
#endif
    }

void DIUnit::init()
    {
#ifdef SC4BOPLA
    setupKeys();
    osDelay(5);
    test();
#endif
    }

#ifdef SC4BOPLA

void DIUnit::setupKeys()
    {
    uint8_t first=1;
     for (uint16_t i=0; i<0x08;i++){
	 if(ReadRegister(KBDADDR(i), REGISTER_IODIRA)>=0){
	     chips[chips_count].address=KBDADDR(i);
	     chips[chips_count].first=first;
	     first+=16;
	     chips[chips_count].last=first-1;
	     ipsSystem::debug->printfl("Found driver@%d, assigned to %d - %d\n",i,chips[chips_count].first,chips[chips_count].last);
	     WriteRegister(KBDADDR(i), 0x0A, 0x40);
	     WriteRegister(KBDADDR(i), REGISTER_GPINTENA, 0xFF);
	     WriteRegister(KBDADDR(i), REGISTER_GPINTENB, 0xFF);
	     WriteRegister(KBDADDR(i), REGISTER_IPOLA, 0xFF);
	     WriteRegister(KBDADDR(i), REGISTER_IPOLB, 0xFF);
	     chips_count++;
	     capacity+=16;
	     ipsSystem::sys.stddebug()<<"REGISTER_INTFA: "<<ReadPair(KBDADDR(i),REGISTER_INTFA)<<"REGISTER_GPIOA: "<<ReadPair(KBDADDR(i),REGISTER_GPIOA)<<"\n";
	 }
     }
    }

int DIUnit::ReadRegister(uint8_t addr, uint8_t reg)
    {
    uint8_t value=0;
    while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
    if(HAL_I2C_Mem_Read(&hi2c3, addr, reg, I2C_MEMADD_SIZE_8BIT, &value, sizeof(value), SCAN_I2C_TIMEOUT)!=HAL_OK)
      {
	ipsSystem::sys.stddebug()<<__PRETTY_FUNCTION__<<"\n";
	return -1;
    }
    return value;
    }



void DIUnit::WriteRegister(uint8_t addr, uint8_t reg, uint8_t value)
    {
    while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
    if(HAL_I2C_Mem_Write(&hi2c3, addr, reg, I2C_MEMADD_SIZE_8BIT, &value, sizeof(value), SCAN_I2C_TIMEOUT)!=HAL_OK)
      {
	ipsSystem::sys.stddebug()<<__PRETTY_FUNCTION__<<"\n";
      }
    }

void DIUnit::test()
    {
    for (uint16_t i=0; i<0x08;i++){
	 ipsSystem::debug->printfl("Seek @ %d ....",KBDADDR(i));
    	 if(ReadRegister(KBDADDR(i), REGISTER_IODIRA)>=0){
    	     ipsSystem::debug->printfl("OK!\n");
    	     ipsSystem::debug->printfl("GPIOA=%b \nGPIOB=%b \n",(uint8_t)ReadRegister(KBDADDR(i), REGISTER_GPIOA),(uint8_t)ReadRegister(KBDADDR(i), REGISTER_GPIOB));
    	 }else{
    	     ipsSystem::debug->printfl("FAIL!\n");
    	 }
    }

    }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    // :TODO Smthng
    ipsSystem::debug->printfl("Interrupt: GPIO Pin %d \n",GPIO_Pin);
    //DOUnit::getInstance().toggleDO(GPIO_Pin);
    if(runner)xTaskNotifyFromISR(runner,KB_INTERRUPT_EVENT,eSetValueWithOverwrite,NULL);
    }

int DIUnit::ReadPair(uint8_t addr, uint8_t reg)
    {
    uint16_t value=0;
    while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
    if(HAL_I2C_Mem_Read(&hi2c3, addr, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&value, sizeof(value), SCAN_I2C_TIMEOUT)!=HAL_OK){
	return -1;
    }
    return value;
    }
#endif

void DIUnit::ControlllerReady ()
{
}


int DIUnit::CreateEvent(uint16_t gpio, uint16_t intf)
    {
  ufw::Event kb_ev;
#ifdef SC4BOPLA
     int k_num = 32-__CLZ(intf);
     uint8_t k_event=(gpio&intf)?KB_EVENT_RISE:KB_EVENT_FALL;//Look at this! check polarity and pull-up/-down
     kb_ev.op=k_event;
     kb_ev.owner=k_num;
     if(input_handle_pointer)input_handle_pointer->sendMsg(&kb_ev);

     if(k_event&KB_EV_MASK){
	 ipsSystem::systerm->printfl("Rise(%d)",k_num);
     }else{
	 ipsSystem::systerm->printfl("Fall(%d)",k_num);
     }

     return k_num;
#else
     return 0;
#endif

    }
