/*
 * ipsUart.cpp
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



#include "ipsUartDev.h"
#include "string.h"

#include <ipssystem.hpp>

//extern char rt_buf[1024];

#define LINE_TERM_CRLF

#ifdef __cplusplus
extern "C" {
#endif

//#include "stdarg.h"

#include "printf.h"

extern UART_HandleTypeDef huart7;


static void v_putc(UART_HandleTypeDef *hin,char ch)
{
#ifdef LINE_TERM_CRLF
	if (ch == '\n') v_putc(hin,'\r');
#endif
	while (!(hin->Instance->ISR&USART_ISR_TC_Msk));
	HAL_UART_Transmit(hin, (uint8_t*)&ch, 1, 0x01);
	return;
}

static void v_puts(UART_HandleTypeDef *hin,const char* str)
{
	while (*str)
	{
		v_putc(hin,*str++);
	}
}


#define PUTC(x) v_putc(hin,x)
#define PUTS(x) v_puts(hin,x)

static void vPrintf(UART_HandleTypeDef *hin, const char *fmt, va_list arp)
{

	unsigned int r, i, j, w, f;
	char s[32], c, d, *p;
	long v;
	unsigned long vs;

	for (;;) {
		c = *fmt++;					/* Get a format character */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass it through if not a % sequense */
			PUTC(c);
            continue;
		}
		f = w = 0;					/* Clear parms */
		c = *fmt++;					/* Get first char of the sequense */
		if (c == '0') {				/* Flag: left '0' padded */
            f = 1;
            c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
                f = 2;
                c = *fmt++;
			}
		}
		if (c == '*') {				/* Minimum width from an argument */
			w = va_arg(arp, int);
			c = *fmt++;
        } else {
            while (c >= '0' && c <= '9') {     /* Minimum width */
				w = w * 10 + c - '0';
				c = *fmt++;
			}
		}
		if (c == 'l' || c == 'L') {	/* Prefix: Size is long */
            f |= 4;
            c = *fmt++;
		}
		if (!c) break;				/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
            p = va_arg(arp, char *);
			for (j = 0; p[j]; j++) ;
            while (!(f & 2) && j++ < w) PUTC(' ');
            PUTS(p);
            while (j++ < w) PUTC(' ');
			continue;
		case 'C' :					/* Character */
			PUTC((char)va_arg(arp, int));
            continue;
		case 'B' :					/* Binary */
            r = 2;
            break;
		case 'O' :					/* Octal */
            r = 8;
            break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
            r = 10;
            break;
		case 'X' :					/* Hexdecimal */
            r = 16;
            break;
		default:					/* Unknown type (passthrough) */
			PUTC(c);
            continue;
		}

		/* Get an argument and put it in numeral */

		if (f & 4) {	/* long argument? */
			v = va_arg(arp, long);
		} else {		/* int/short/char argument */
			v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
		if (d == 'D' && v < 0) {	/* Negative value? */
            v = 0 - v;
            f |= 16;
		}
        i = 0;
        vs = v;
		do {
            d = (char)(vs % r);
            vs /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			s[i++] = d + '0';
		} while (vs != 0 && i < sizeof s);
		if (f & 16) s[i++] = '-';
        j = i;
        d = (f & 1) ? '0' : ' ';
        while (!(f & 2) && j++ < w) PUTC(d);
        do PUTC(s[--i]);
        while (i != 0);
        while (j++ < w) PUTC(' ');
	}
}

extern "C" void printfmUART3 (const char *fmt, ...)
{
	va_list arp;

	va_start(arp, fmt);
	vPrintf(&huart7, fmt, arp);
	va_end(arp);
}


#ifdef __cplusplus
}		/// End extern
#endif





ipsUART::ipsUART(UART_HandleTypeDef *uart_hin)
:ttY()
{
	handle=uart_hin;
	rt_buf=(char*)system_cpp.allocateDmaBuffer(sizeof(char[2*1024]));
}

void ipsUART::putc (char ch)
{
	//while (!(handle->Instance->ISR&USART_ISR_TC_Msk));
#ifdef LINE_TERM_CRLF
	if (ch == '\n') putc('\r');
#endif
	//HAL_UART
	HAL_UART_Transmit(handle, (uint8_t*)&ch, 1, 0x01);
	return;
}

void ipsUART::write_t(const char* buf,size_t size)
{
	while (!(handle->Instance->ISR&USART_ISR_TC_Msk));
#ifdef DMAREADY
	if (size<0){
		HAL_UART_Transmit(handle, (uint8_t*)buf, size, size);
	}else{
		memcpy(rt_buf,buf,size);
		HAL_UART_Transmit_DMA(handle, (uint8_t*)rt_buf, size);

		//while (!(handle->Instance->ISR&USART_ISR_TC_Msk)){osDelay(1);};

		//Error_Handler();
	}
#else
	HAL_UART_Transmit(handle, (uint8_t*)buf, size, size);
#endif
	return;
}

void ipsUART::writeln(const char* str)
{
	//while (!(handle->Instance->ISR&USART_ISR_TC_Msk));
	while (*str)
	{
		putc(*str++);
	}
}

/*
void ipsUART::printfl (const char *fmt, ...)
{
	va_list arp;

	va_start(arp, fmt);
	vPrintf(handle, fmt, arp);
	va_end(arp);
}
*/
void ipsUART::init (char port){
	switch (port) {
		case 3:
			MX_USART3_UART_Init();
			//HAL_NVIC_DisableIRQ(DMA1_Stream0_IRQn);
			break;
		default:
			Error_Handler();
			break;
	}
}


void ipsUART::cls(void)
{
	writeln("\033[2J");
}
/*
static void MX_USART3_UART_Init(void)
{


  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV10;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	huart->gState=HAL_UART_STATE_READY;
}


