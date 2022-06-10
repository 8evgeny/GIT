#ifndef DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_DEBUG_FUNC_OUT     1           /* 1: Use output functions */
#define	_CR_CRLF_DEBUG          1           /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG_DEBUG     1           /* 1: Enable long long integer in type "ll". */
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
#define _LONGLONG_t         long long int

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
