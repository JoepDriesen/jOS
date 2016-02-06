#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
 
static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static void printuint(unsigned int num)
{
	if (num >= 10)
	{
		printuint(num / 10);
	}
	putchar('0' + (num % 10));
}

static void printint(int num)
{
	if (num < 0)
	{
		putchar('-');
		num *= -1;
	}
	printuint(num);
}	

static void printrawhex(unsigned int num, char a, char b, char c, char d, char e, char f)
{
	if (num >= 16)
	{
		printrawhex(num / 16, a, b, c, d, e, f);
	}

	num %= 16;

	if (num < 10)
		putchar('0' + num);
	else if (num == 10)
		putchar(a);
	else if (num == 11)
		putchar(b);
	else if (num == 12)
		putchar(c);
	else if (num == 13)
		putchar(d);
	else if (num == 14)
		putchar(e);
	else
		putchar(f);
}

static void printhexlc(int num)
{
	putchar('0');
	putchar('x');
	printrawhex(num, 'a', 'b', 'c', 'd', 'e', 'f');
}

static void printhexuc(int num)
{
	putchar('0');
	putchar('x');
	printrawhex(num, 'A', 'B', 'C', 'D', 'E', 'F');
}
 
int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;
 
	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format;
 
		if ( *(++format) == '%' )
			goto print_c;
 
		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
 
		if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'd' || *format == 'i' )
		{
			format++;
			int n = va_arg(parameters, int);
			printint(n);
		}
		else if ( *format == 'u' )
		{
			format++;
			int n = va_arg(parameters, int);
			printuint(n);
		}
		else if ( *format == 'x' )
		{
			format++;
			int n = va_arg(parameters, int);
			printhexlc(n);
		}
		else if ( *format == 'X' )
		{
			format++;
			int n = va_arg(parameters, int);
			printhexuc(n);
		}
		else
		{
			goto incomprehensible_conversion;
		}
	}
 
	va_end(parameters);
 
	return written;
}
