#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_early(void)
{
	terminal_initialize();

	gdt_init();
}

void kernel_main(void)
{
	gdt_ptr_t i;
	asm("sgdt %0\n" :: "m"(i));
    
	printf("base: %X", (uint32_t)i.base);
}
