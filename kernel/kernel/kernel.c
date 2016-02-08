#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <sys/multiboot.h>
#include <kernel/debug.h>
#include <sys/memory.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>
#include <kernel/idt.h>
#include <kernel/frames.h>

void kernel_early(multiboot_info_t* mbd, unsigned int magic)
{
	terminal_initialize();

	printf("Starting jOS Kernel\n");
	printf("===================\n\n");
	
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		panic("Bootloader is not Multiboot Compliant");
	
	memory_init(mbd);

	printf("[x] Memory Initialized\n");
	
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
	
	phys_mem_management_init();
	
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
