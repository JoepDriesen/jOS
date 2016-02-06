#include <sys/interrupts.h>
#include <stdio.h>

void_callback_arg_t interruptHandlers[256];

void register_interrupt_handler(uint8_t n, void_callback_arg_t handler)
{
	interruptHandlers[n] = handler;
}

void unhandled_interrupt(uint32_t intNum, ...)
{
	printf("Received unhandled interrupt: %u\n", (unsigned int)intNum);
}
