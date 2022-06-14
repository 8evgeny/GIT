#include "rs232_printf.h"

#if _USE_RS232_FUNC_OUT

#include <stdarg.h>
void (*RS232FuncOut)(unsigned char);	/* Pointer to the output stream */
static char *outptr;

/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

void RS232Putc(char c)
{
    if (_CR_CRLF_RS232 && c == '\n') RS232Putc('\r');		/* CR -> CRLF */

	if (outptr) {		/* Destination is memory */
		*outptr++ = (unsigned char)c;
		return;
	}
    if (RS232FuncOut) {	/* Destination is device */
        RS232FuncOut((unsigned char)c);
	}
}

/*----------------------------------------------*/
/* Put a null-terminated string                 */
/*----------------------------------------------*/

void RS232Puts(      /* Put a string to the default device */
    const char *str				/* Pointer to the string */
)
{
	while (*str) {
        RS232Putc(*str++);
	}
}


void RS232Fputs(      /* Put a string to the specified device */
    void(*func)(unsigned char),	/* Pointer to the output function */
    const char	*str				/* Pointer to the string */
)
{
	void (*pf)(unsigned char);


    pf = RS232FuncOut;		/* Save current output device */
    RS232FuncOut = func;	/* Switch output to specified device */
	while (*str) {		/* Put the string */
        RS232Putc(*str++);
	}
    RS232FuncOut = pf;		/* Restore output device */
}



/*----------------------------------------------*/
/* Formatted string output                      */
/*----------------------------------------------*/
/*  RS232Printf("%d", 1234);			"1234"
    RS232Printf("%6d,%3d%%", -200, 5);	"  -200,  5%"
    RS232Printf("%-6u", 100);			"100   "
    RS232Printf("%ld", 12345678);		"12345678"
    RS232Printf("%llu", 0x100000000);	"4294967296"	<_USE_LONGLONG>
    RS232Printf("%04x", 0xA3);			"00a3"
    RS232Printf("%08lX", 0x123ABC);		"00123ABC"
    RS232Printf("%016b", 0x550F);		"0101010100001111"
    RS232Printf("%*d", 6, 100);			"   100"
    RS232Printf("%s", "String");		"String"
    RS232Printf("%-5s", "abc");			"abc  "
    RS232Printf("%5s", "abc");			"  abc"
    RS232Printf("%c", 'a');				"a"
    RS232Printf("%f", 10.0);            <xprintf lacks floating point support. Use regular printf.>
*/

static void RS232Vprintf(
    const char	*fmt,	/* Pointer to the format string */
    va_list arp			/* Pointer to arguments */
)
{
	unsigned int r, i, j, w, f;
	char s[32], c, d, *p;
#if _USE_LONGLONG_RS232
	_LONGLONG_t v;
	unsigned _LONGLONG_t vs;
#else
	long v;
	unsigned long vs;
#endif


	for (;;) {
		c = *fmt++;					/* Get a format character */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass it through if not a % sequense */
            RS232Putc(c);
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
#if _USE_LONGLONG_RS232
			if (c == 'l' || c == 'L') {	/* Prefix: Size is long long */
                f |= 8;
                c = *fmt++;
			}
#endif
		}
		if (!c) break;				/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
            p = va_arg(arp, char *);
			for (j = 0; p[j]; j++) ;
            while (!(f & 2) && j++ < w) RS232Putc(' ');
            RS232Puts(p);
            while (j++ < w) RS232Putc(' ');
			continue;
		case 'C' :					/* Character */
            RS232Putc((char)va_arg(arp, int));
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
            RS232Putc(c);
            continue;
		}

		/* Get an argument and put it in numeral */
#if _USE_LONGLONG_RS232
		if (f & 8) {	/* long long argument? */
			v = va_arg(arp, _LONGLONG_t);
		} else {
			if (f & 4) {	/* long argument? */
				v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
			} else {		/* int/short/char argument */
				v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
			}
		}
#else
		if (f & 4) {	/* long argument? */
			v = va_arg(arp, long);
		} else {		/* int/short/char argument */
			v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
#endif
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
        while (!(f & 2) && j++ < w) RS232Putc(d);
        do RS232Putc(s[--i]);
        while (i != 0);
        while (j++ < w) RS232Putc(' ');
	}
}

void RS232Printf(    /* Put a formatted string to the default device */
    const char	*fmt,	/* Pointer to the format string */
    ...					/* Optional arguments */
)
{
	va_list arp;


	va_start(arp, fmt);
    RS232Vprintf(fmt, arp);
	va_end(arp);
}

void RS232Sprintf(    /* Put a formatted string to the memory */
    char *buff,			/* Pointer to the output buffer */
    const char	*fmt,	/* Pointer to the format string */
    ...					/* Optional arguments */
)
{
	va_list arp;


	outptr = buff;		/* Switch destination for memory */

	va_start(arp, fmt);
    RS232Vprintf(fmt, arp);
	va_end(arp);

	*outptr = 0;		/* Terminate output string with a \0 */
	outptr = 0;			/* Switch destination for device */
}


void RS232Fprintf(      /* Put a formatted string to the specified device */
    void(*func)(unsigned char),	/* Pointer to the output function */
    const char	*fmt,			/* Pointer to the format string */
    ...							/* Optional arguments */
)
{
	va_list arp;
	void (*pf)(unsigned char);


    pf = RS232FuncOut;		/* Save current output device */
    RS232FuncOut = func;	/* Switch output to specified device */

	va_start(arp, fmt);
    RS232Vprintf(fmt, arp);
	va_end(arp);

    RS232FuncOut = pf;		/* Restore output device */
}



/*----------------------------------------------*/
/* Dump a line of binary dump                   */
/*----------------------------------------------*/

void RS232PutDump(
    const void *buff,		/* Pointer to the array to be dumped */
    unsigned long addr,		/* Heading address value */
    int len,				/* Number of items to be dumped */
    int width				/* Size of the items (DF_CHAR, DF_SHORT, DF_LONG) */
)
{
	int i;
	const unsigned char *bp;
	const unsigned short *sp;
	const unsigned long *lp;


    RS232Printf("%08lX ", addr);		/* address */

	switch (width) {
    case DW_RS232_CHAR:
		bp = buff;
		for (i = 0; i < len; i++)		/* Hexdecimal dump */
            RS232Printf(" %02X", bp[i]);
        RS232Putc(' ');
		for (i = 0; i < len; i++)		/* ASCII dump */
            RS232Putc((unsigned char)((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.'));
		break;
    case DW_RS232_SHORT:
		sp = buff;
		do								/* Hexdecimal dump */
            RS232Printf(" %04X", *sp++);
		while (--len);
		break;
    case DW_RS232_LONG:
		lp = buff;
		do								/* Hexdecimal dump */
            RS232Printf(" %08LX", *lp++);
		while (--len);
		break;
	}

    RS232Putc('\n');
}

#endif /* _USE_XFUNC_OUT */



#if _USE_RS232_FUNC_IN
unsigned char (*RS232FuncIn)(void);	/* Pointer to the input stream */

/*----------------------------------------------*/
/* Get a line from the input                    */
/*----------------------------------------------*/

int RS232Gets(   /* 0:End of stream, 1:A line arrived */
    char *buff,	/* Pointer to the buffer */
    int len		/* Buffer length */
)
{
	int c, i;


    if (!RS232FuncIn) return 0;		/* No input function specified */

	i = 0;
	for (;;) {
        c = RS232FuncIn();				/* Get a char from the incoming stream */
		if (!c) return 0;			/* End of stream? */
		if (c == '\r') break;		/* End of line? */
		if (c == '\b' && i) {		/* Back space? */
			i--;
            if (_LINE_ECHO_RS232) RS232Putc((unsigned char)c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
            if (_LINE_ECHO_RS232) RS232Putc((unsigned char)c);
		}
	}
	buff[i] = 0;	/* Terminate with a \0 */
    if (_LINE_ECHO_RS232) RS232Putc('\n');
	return 1;
}


int RS232Fgets(  /* 0:End of stream, 1:A line arrived */
    unsigned char (*func)(void),	/* Pointer to the input stream function */
    char *buff,	/* Pointer to the buffer */
    int len		/* Buffer length */
)
{
	unsigned char (*pf)(void);
	int n;


    pf = RS232FuncIn;			/* Save current input device */
    RS232FuncIn = func;		/* Switch input to specified device */
    n = RS232Gets(buff, len);	/* Get a line */
    RS232FuncIn = pf;			/* Restore input device */

	return n;
}


/*----------------------------------------------*/
/* Get a value of the string                    */
/*----------------------------------------------*/
/*	"123 -5   0x3ff 0b1111 0377  w "
	    ^                           1st call returns 123 and next ptr
	       ^                        2nd call returns -5 and next ptr
                   ^                3rd call returns 1023 and next ptr
                          ^         4th call returns 15 and next ptr
                               ^    5th call returns 255 and next ptr
                                  ^ 6th call fails and returns 0
*/

int RS232Atoi(    /* 0:Failed, 1:Successful */
    char **str,		/* Pointer to pointer to the string */
    long *res		/* Pointer to the valiable to store the value */
)
{
	unsigned long val;
	unsigned char c, r, s = 0;


	*res = 0;

    while ((c = **str) == ' ')(*str)++;	/* Skip leading spaces */

	if (c == '-') {		/* negative? */
		s = 1;
		c = *(++(*str));
	}

	if (c == '0') {
		c = *(++(*str));
		switch (c) {
		case 'x':		/* hexdecimal */
            r = 16;
            c = *(++(*str));
			break;
		case 'b':		/* binary */
            r = 2;
            c = *(++(*str));
			break;
		default:
			if (c <= ' ') return 1;	/* single zero */
			if (c < '0' || c > '9') return 0;	/* invalid char */
			r = 8;		/* octal */
		}
	} else {
		if (c < '0' || c > '9') return 0;	/* EOL or invalid char */
		r = 10;			/* decimal */
	}

	val = 0;
	while (c > ' ') {
		if (c >= 'a') c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;	/* invalid char */
		}
		if (c >= r) return 0;		/* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));
	}
	if (s) val = 0 - val;			/* apply sign if needed */

	*res = val;
	return 1;
}

#endif /* RS232_PRINTF_H */
