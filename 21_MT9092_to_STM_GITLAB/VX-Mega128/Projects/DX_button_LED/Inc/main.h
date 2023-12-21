#pragma once
#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool Levers[6] = {
    true,   //1 рычаг существует
    true,   //2 рычаг существует
    true,   //3 рычаг существует
    true,   //4 рычаг существует
    true,   //5 рычаг существует
    true    //6 рычаг существует
};
