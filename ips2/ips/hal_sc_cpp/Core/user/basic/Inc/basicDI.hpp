/*
 * basicDI.hpp
 *
 *  Created on: 15 сент. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_BASICDI_HPP_
#define USER_BASIC_INC_BASICDI_HPP_

#include <cstdint>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"

#define KeyboardRise	0xFF
#define KeyboardFall	0xF0

typedef struct{
    uint8_t address;
    uint8_t first;
    uint8_t last;
}key_dev;

class DIUnit
{
public:
    static DIUnit& GetInstance();
    static void run(void* par);

    DIUnit* pointer();
    void ControlllerReady();
    void test();

protected:
    DIUnit();
    DIUnit(const DIUnit&);
    DIUnit& operator=(DIUnit&);

    void refreshTask();

    void init();
    void setupKeys();

    int ReadRegister(uint8_t addr, uint8_t reg);
    void WriteRegister(uint8_t addr,uint8_t reg, uint8_t value);
    int ReadPair(uint8_t addr, uint8_t reg);
    int CreateEvent(uint16_t gpio,uint16_t intf);

    xTaskHandle handle;
    uint16_t capacity=0;
    uint16_t chips_count=0;
    key_dev chips[10];


};


#endif /* USER_BASIC_INC_BASICDI_HPP_ */
