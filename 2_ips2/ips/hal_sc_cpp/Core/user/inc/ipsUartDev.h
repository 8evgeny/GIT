/*
 * ipsUart.h
 *
 *  Created on: May 26, 2020
 *      Author: Donskikh Dmitry
 */
/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#pragma once

//#include "stm32h7xx_hal.h"
//#include "ttY.h"
#include "main.h"
#include "cmsis_os.h"
#include <devices/stream_device.h>

#define DMAREADY

#define TERMM_CLS "\033[2J"

#define WAIT_TX_CMPLT(x) while (!(x->Instance->ISR&USART_ISR_TC_Msk));


class ipsUART :public ttY {
private:
	UART_HandleTypeDef *handle;
	char* rt_buf=nullptr;

public:
	ipsUART(UART_HandleTypeDef *uart_hin);

	void putc (char ch);
	void write_t (const char* buf,size_t size);
	void writeln (const char* str);
	//void printfl (const char *fmt, ...) override;
	void init (char port);
	void cls (void);

};


/*static*/ void MX_USART3_UART_Init(void);
//extern "C" void printfmUART3 (const char *fmt, ...);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
