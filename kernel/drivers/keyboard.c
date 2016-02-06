#include <stdio.h>
#include <stdint.h>
#include <sys/interrupts.h>
#include <kernel/io.h>
#include <kernel/pic.h>

void keyboard_interrupt_handler(uint32_t, ...);



void keyboard_init()
{
	asm("cli");
	
	// Keyboard interrupt is IRQ1, which is remapped to ISR 0x21
	register_interrupt_handler(0x21, &keyboard_interrupt_handler);
	
	PIC_clear_irq_mask(0x1);
	
	asm("sti");
}

void keyboard_interrupt_handler(uint32_t int_no, ...)
{
	unsigned char scancode;

	scancode = inb(0x60);
	io_wait();
	
	if (scancode & 0x80)
	{
		// Top bit set -> Key was released
		scancode &= ~0x80;
		
	}
}
