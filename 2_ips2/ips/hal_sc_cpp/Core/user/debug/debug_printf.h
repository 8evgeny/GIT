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


#ifndef DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_DEBUG_FUNC_OUT     1           /* 1: Use output functions */
#define	_CR_CRLF_DEBUG          1           /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG_DEBUG     0           /* 1: Enable long long integer in type "ll". */
#define	_DEBUG_LONGLONG_t		long long	/* Platform dependent long long integer type */

#define _USE_DEBUG_FUNC_IN      1           /* 1: Use input function */
#define	_LINE_ECHO_DEBUG        1           /* 1: Echo back input chars in gets function */


#if _USE_DEBUG_FUNC_OUT

#define debugDevOut(func) debugFuncOut = (void(*)(unsigned char))(func)

extern void (*debugFuncOut)(unsigned char);

void debugPutc(char c);
void debugPuts(const char *str);
void debugFputs(void (*func)(unsigned char), const char *str);
void debugPrintf(const char *fmt, ...);
void debugSprintf(char *buff, const char *fmt, ...);
void debugFprintf(void (*func)(unsigned char), const char	*fmt, ...);
void debugPutDump(const void *buff, unsigned long addr, int len, int width);

#define DW_DEBUG_CHAR		sizeof(char)
#define DW_DEBUG_SHORT      sizeof(short)
#define DW_DEBUG_LONG		sizeof(long)

#endif

#if _USE_DEBUG_FUNC_IN

#define debugDevIn(func) debugFuncIn = (unsigned char(*)(void))(func)

extern unsigned char (*debugFuncIn)(void);

int debugGets(char *buff, int len);
int debugFgets(unsigned char (*func)(void), char *buff, int len);
int debugAtoi(char **str, long *res);

#endif

#ifdef __cplusplus
}
#endif

#endif
