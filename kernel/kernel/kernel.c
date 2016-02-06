#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>
#include <kernel/idt.h>
#include <drivers/keyboard.h>

void kernel_early(void)
{
	terminal_initialize();

	printf("Initializing Kernel\n");

	gdt_init();
	
	printf("GDT Initialized, entering Protected Mode\n");
	
	printf("Enabling IDT:\n");

	pic_init();

	printf("\t[x] PIC IRQs remapped\n");
	
	idt_init();

	printf("\t[x] IDT Initialized\n");
	if (are_interrupts_enabled())
		printf("\t[x] Hardware Interrupts enabled\n");
	else
		printf("\t[ ] Error enabling Hardware Interrupts\n");
	
	keyboard_init();
	
	printf("Keyboard enabled\n");
}

void kernel_main(void)
{
	//gdt_ptr_t i;
	//asm("sgdt %0\n" :: "m"(i));
    
	//printf("base: %X", (uint32_t)i.base);
	
	while (1)
	{
		;
	}
}
