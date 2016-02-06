#include <stdio.h>
#include <stdint.h>
#include <kernel/io.h>
#include <kernel/pic.h>

#define MASTER_PIC_IRQ_OFFSET	0x20
#define SLAVE_PIC_IRQ_OFFSET	0x28

#define MASTER_PIC_COMMAND_PORT	0x20
#define MASTER_PIC_DATA_PORT	0x21
#define SLAVE_PIC_COMMAND_PORT	0xA0
#define SLAVE_PIC_DATA_PORT		0xA1

#define ICW1_ICW4		0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE		0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL		0x08		/* Level triggered (edge) mode */
#define ICW1_INIT		0x10		/* Initialization - required! */
 
#define ICW4_8086		0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO		0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM		0x10		/* Special fully nested (not) */

#define PIC_END_OF_INTERRUPT	0x20

// Enable the line for a given IRQ, this will allow the respective device to send interrupts
void PIC_clear_irq_mask(uint8_t irq)
{
	uint16_t port;
	uint8_t mask;

	if (irq < 8)
		port = MASTER_PIC_DATA_PORT;
	else if (irq < 16)
	{
		port = SLAVE_PIC_DATA_PORT;
		irq -= 8;
	}
	else
	{
		printf("Cannot clear IRQ > 15: %u", irq);
		return;
	}

	mask = inb(port) & ~(1 << irq);
	io_wait();

	outb(port, mask);
	io_wait();
}

void pic_init()
{
	// Remap Master PIC IRQs to 0x20 and Slave PIC IRQs to 0x28
	PIC_remap_irqs(MASTER_PIC_IRQ_OFFSET, SLAVE_PIC_IRQ_OFFSET);
}

/* A device sends a PIC chip an interrupt, and the PIC tells the CPU an interrupt occurred. When the CPU 
 * acknowledges the "interrupt occurred" signal, the PIC chip sends the interrupt number (between 00h and 
 * FFh, or 0 and 255 decimal) to the CPU. When the system first starts up, IRQs 0 to 7 are set to interrupts 
 * 08h to 0Fh, and IRQs 8 to 15 are set to interrupts 70h to 77h.
 * The CPU has reserved interrupts 0-31, yet IRQs 0-7 are set to interrupts 08-0Fh. So we tell the PIC to 
 * map the IRQs to different interrupts, such as INT 78h-7Fh. Note that IRQs can only be mapped to INTs that 
 * are multiples of 08h: 00h-07h, 08h-0Fh, 10h-17h, 17h-1Fh. And you probably want to use 20h-27h, or 
 * greater, since 00h-1Fh are reserved by the CPU.
 */ 
void PIC_remap_irqs(int masterOffset, int slaveOffset)
{
	/* The first command you will need to give the two PICs is the initialise command (code 0x11). This 
	 * command makes the PIC wait for 3 extra "initialisation words" on the data port. These bytes 
	 * give the PIC:
	 *
	 *	- Its vector offset. (ICW2)
     *	- Tell it how it is wired to master/slaves. (ICW3)
     *	- Gives additional information about the environment. (ICW4) 
	 */
	// starts the initialization sequence (in cascade mode)
	outb(MASTER_PIC_COMMAND_PORT, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(SLAVE_PIC_COMMAND_PORT, ICW1_INIT+ICW1_ICW4);
	io_wait();
	
	outb(MASTER_PIC_DATA_PORT, masterOffset);	// ICW2: Master PIC vector offset
	io_wait();
	outb(SLAVE_PIC_DATA_PORT, slaveOffset);		// ICW2: Slave PIC vector offset
	io_wait();
	
	outb(MASTER_PIC_DATA_PORT, 4);	// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(SLAVE_PIC_DATA_PORT, 2);	// ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(MASTER_PIC_DATA_PORT, ICW4_8086);
	io_wait();
	outb(SLAVE_PIC_DATA_PORT, ICW4_8086);
	io_wait();

	// At this point, init is complete, and the pic goes into normal operating mode. 
	// Set the Interrupt Masks so we don't get interrupts until we set up the handlers for the respective 
	// IRQ Lines.	
	outb(MASTER_PIC_DATA_PORT, 0xFF);	// IRQ 0-7 are now all masked
	io_wait();
	outb(SLAVE_PIC_DATA_PORT, 0xFF);	// IRQ 8-15 are now all masked
	io_wait();
}

void PIC_sendEOI(uint8_t irq)
{
	if (irq > 8)
		outb(SLAVE_PIC_COMMAND_PORT, PIC_END_OF_INTERRUPT);
	
	outb(MASTER_PIC_COMMAND_PORT, PIC_END_OF_INTERRUPT);
}

// Disable the line for a given IRQ, this will prevent the respective device to send interrupts
void PIC_set_irq_mask(uint8_t irq)
{
	uint16_t port;
	uint8_t mask;

	if (irq < 8)
		port = MASTER_PIC_DATA_PORT;
	else if (irq < 16)
	{
		port = SLAVE_PIC_DATA_PORT;
		irq -= 8;
	}
	else
	{
		printf("Cannot set IRQ > 15: %u", irq);
		return;
	}

	mask = inb(port) | (1 << irq);
	outb(port, mask);
}

