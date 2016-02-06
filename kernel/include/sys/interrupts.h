#ifndef _SYS_INTERRUPTS_H
#define _SYS_INTERRUPTS_H

#include <stdint.h>
#include <sys/callback.h>

void register_interrupt_handler(uint8_t n, void_callback_arg_t handler);

void unhandled_interrupt(uint32_t intNum, ...);

#endif
