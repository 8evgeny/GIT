#ifndef RS232_PRINTF_H
#define RS232_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_RS232_FUNC_OUT     1           /* 1: Use output functions */
#define	_CR_CRLF_RS232          1           /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG_RS232     0           /* 1: Enable long long integer in type "ll". */
#define	_RS232_LONGLONG_t		long long	/* Platform dependent long long integer type */

#define _USE_RS232_FUNC_IN      1           /* 1: Use input function */
#define	_LINE_ECHO_RS232		1           /* 1: Echo back input chars in gets function */

#if _USE_RS232_FUNC_OUT

#define RS232DevOut(func) RS232FuncOut = (void(*)(unsigned char))(func)

extern void (*RS232FuncOut)(unsigned char);

void RS232Putc(char c);
void RS232Puts(const char *str);
void RS232Fputs(void (*func)(unsigned char), const char *str);
void RS232Printf(const char *fmt, ...);
void RS232Sprintf(char *buff, const char *fmt, ...);
void RS232Fprintf(void (*func)(unsigned char), const char	*fmt, ...);
void RS232PutDump(const void *buff, unsigned long addr, int len, int width);

#define DW_RS232_CHAR		sizeof(char)
#define DW_RS232_SHORT      sizeof(short)
#define DW_RS232_LONG		sizeof(long)

#endif

#if _USE_RS232_FUNC_IN

#define RS232DevIn(func) RS232FuncIn = (unsigned char(*)(void))(func)

extern unsigned char (*RS232FuncIn)(void);

int RS232Gets(char *buff, int len);
int RS232Fgets(unsigned char (*func)(void), char *buff, int len);
int RS232Atoi(char **str, long *res);

#endif

#ifdef __cplusplus
}
#endif

#endif
