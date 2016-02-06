#ifndef _IDT_H
#define _IDT_H

#include <stdbool.h>
#include <stdint.h>

struct idt_entry_struct
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
}__attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

struct regs
{
	unsigned int ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

void idt_init();
bool are_interrupts_enabled();

#endif
