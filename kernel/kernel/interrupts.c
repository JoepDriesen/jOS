#include <stdarg.h>
#include <stdio.h>
#include <sys/interrupts.h>

void_callback_arg_t interruptHandlers[256];

void register_interrupt_handler(uint8_t n, void_callback_arg_t handler)
{
	interruptHandlers[n] = handler;
}

void unhandled_interrupt(uint32_t intNum, ...)
{
	printf("Received unhandled interrupt: %u\n", (unsigned int)intNum);
}

void general_protection_fault_handler(uint32_t intNum, ...)
{
	va_list args;
	
	va_start(args, intNum);

	int errCode = va_arg(args, uint32_t);
	
	printf("General Protection Fault - Segment %X\n", errCode);

	while (1)
		;
}
