/*
** <sys/callback.h>
** Callbacks are function holders. We can "throw" functions into them
** For dynamic calling during runtime. They are VERY Useful.
*/
#ifndef _SYS_CALLBACK_H
#define _SYS_CALLBACK_H

#include <stdarg.h>
#include <stdint.h>

// Void Return Primatives
typedef void (*void_callback_t)(void);
typedef void (*void_callback_int8_t)(int8_t);
typedef void (*void_callback_int16_t)(int16_t);
typedef void (*void_callback_int32_t)(int32_t);
/// Unsigned Versions
typedef void (*void_callback_uint8_t)(uint8_t);
typedef void (*void_callback_uint16_t)(uint16_t);
typedef void (*void_callback_uint32_t)(uint32_t);
/// Special Unlimited Variable version.
typedef void (*void_callback_arg_t)(uint32_t, ...);

// Char Return Primatives
typedef char (*char_callback_t)(void);
typedef char (*char_callback_char_t)(char);
typedef char (*char_callback_short_t)(short);
typedef char (*char_callback_int_t)(int);
/// Unsigned Versions
typedef char (*char_callback_uchar_t)(unsigned char);
typedef char (*char_callback_ushort_t)(unsigned short);
typedef char (*char_callback_uint_t)(unsigned int);

// Int Return Primatives
typedef int (*int_callback_t)(void);
typedef int (*int_callback_char_t)(char);
typedef int (*int_callback_short_t)(short);
typedef int (*int_callback_int_t)(int);
/// Unsigned Versions
typedef int (*int_callback_uchar_t)(unsigned char);
typedef int (*int_callback_ushort_t)(unsigned short);
typedef int (*int_callback_uint_t)(unsigned int);

// Short Return Primatives
typedef short (*short_callback_t)(void);
typedef short (*short_callback_char_t)(char);
typedef short (*short_callback_short_t)(short);
typedef short (*short_callback_int_t)(int);
/// Unsigned Versions
typedef short (*short_callback_uchar_t)(unsigned char);
typedef short (*short_callback_ushort_t)(unsigned short);
typedef short (*short_callback_uint_t)(unsigned int);

#endif

