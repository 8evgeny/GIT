/*
 * basicDO.h
 *
 *  Created on: 27 июл. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_INC_BASICDO_H_
#define USER_INC_BASICDO_H_
#include <cstdint>
#include "main.h"



//#define IPC_KEY_ON	GPIO_PIN_SET
//#define IPC_KEY_OFF	GPIO_PIN_RESET

#define IPS_LED_ON		0xFF
#define IPS_LED_OFF		0x00

#define IPS_LED_RED		0x01
#define IPS_LED_GREEN		0x00

enum KeyBlock{
    LED_GREEN=(uint8_t)0x00,
    LED_RED=(uint8_t)0x01,
    DO=(uint8_t)0x02,
    RLY=(uint8_t)0x03
//    KEY=0x11,
//    NUMPAD=0x12,
};

typedef union{
    uint16_t w;
    struct{
	uint8_t output;
	uint8_t block;
    };
}key_id_t;

typedef struct{
    uint8_t address;
    uint8_t first;
    uint8_t last;
}led_dev;

class DOUnit
{
public:
	static DOUnit& getInstance();
	DOUnit* pointer();
	uint16_t getCount(void);
	virtual void setDO(uint16_t output);
	virtual void resetDO(uint16_t output);
	virtual void toggleDO(uint16_t output);
	virtual uint8_t state(uint16_t output);
	//uint8_t

protected:
	DOUnit();
	DOUnit(const DOUnit&);
	DOUnit& operator=(DOUnit&);
	virtual void init();
	uint16_t capacity=0;
	uint8_t chips_count=0;
	led_dev chips[10];
	void* _set_fcn=nullptr;
	void* _reset_fcn=nullptr;
	void* _toggle_fcn=nullptr;

	void scanICs();


	uint8_t getICAddress(uint16_t led_n);
	void ledsSetup();
	void _ledPWM(uint8_t led,uint8_t color,uint8_t value);
	void _ledON(uint8_t led,uint8_t color);
	void _ledOFF(uint8_t led,uint8_t color);
	void _ledAllON(void);
	void _ledAllOFF(void);
	void _ledAllGreen(uint8_t value);
	void _ledAllRed(uint8_t value);
	uint8_t _ledReadState(uint8_t led,uint8_t color);

	void _test();


};




#endif /* USER_INC_BASICDO_H_ */
