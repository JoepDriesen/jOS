#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>
#include <sys/cdefs.h>

int printf(const char* __restrict, ...);
int vprintf(const char* __restrict, va_list);
int putchar(int);
int puts(const char*);

#endif
