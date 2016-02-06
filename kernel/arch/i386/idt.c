#include <kernel/pic.h>
#include <kernel/idt.h>
#include <sys/callback.h>
#include <sys/interrupts.h>
#include <sys/callback.h>
#include <stdio.h>
#include <string.h>

extern void idt_flush(unsigned int);
extern void isr0();
extern void isr1();

extern void isr8();
extern void isr9();

extern void irq0();
extern void irq1();

static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

idt_entry_t idt[256];
idt_ptr_t idtPointer;

extern void_callback_arg_t interruptHandlers[];

void idt_init()
{
	idtPointer.limit = sizeof(idt_entry_t) * 256 - 1;
	idtPointer.base = (unsigned int) &idt;
	
	memset(&idt, 0, sizeof(idt_entry_t) * 256);
	
	int i;
	unsigned int isrLen = (unsigned int)isr1 - (unsigned int)isr0;
	
	if (((unsigned int)isr9 - (unsigned int)isr8) != isrLen)
		printf("WARNING: ISR_ERRCODE Macro size (%u) is not the same as ISR_NOERRCODE Macro size (%u)",
				(unsigned int)isr9 - (unsigned int)isr8, isrLen);
	if (((unsigned int)irq1 - (unsigned int)irq0) != isrLen)
		printf("WARNING: IRQ Macro size (%u) is not the same as ISR_NOERRCODE Macro size (%u)",
				(unsigned int)irq1 - (unsigned int)irq0, isrLen);
	
	// Initialize the IDT with the stubs
	for (i = 0; i < 128; i++)
	{
		idt_set_gate(i, (unsigned int)isr0 + (isrLen * i), 0x08, 0x8E);
	}
	
	// Initialize ISR to the default state
	for (i=0; i < 256; i++)
	{
		register_interrupt_handler(i, unhandled_interrupt);
	}
	
	idt_flush((unsigned int) &idtPointer);
}

static void idt_set_gate(uint8_t n, uint32_t offset, uint16_t selector, uint8_t flags)
{
	idt[n].offset_low = offset & 0xFFFF;
	idt[n].offset_high = (offset >> 16) & 0xFFFF;
	
	idt[n].selector = selector;
	idt[n].zero = 0;
	idt[n].type_attr = flags;
}

void isr_handler(struct regs *r)
{
	void_callback_arg_t handler = interruptHandlers[r->int_no];
	handler(r->int_no, r->err_code, r->eax, r->ebx, r->ecx, r->edx, r->edi, r->esi);
}

void irq_handler(struct regs *r)
{
	PIC_sendEOI(r->int_no);
	
	void_callback_arg_t handler = interruptHandlers[r->int_no];
	handler(r->int_no, r->err_code, r->eax, r->ebx, r->ecx, r->edx, r->edi, r->esi);
}

inline bool are_interrupts_enabled()
{
	unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}	
