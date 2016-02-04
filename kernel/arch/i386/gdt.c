#include <kernel/gdt.h>
#include <stdio.h>

extern void gdt_flush(unsigned);

void gdt_set_entry(int, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt[5];
gdt_ptr_t gdtPointer;

void gdt_init()
{
	gdtPointer.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdtPointer.base = (unsigned) &gdt;
	
	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	printf("GDT Pointer Base: %X", gdtPointer.base);
	gdt_flush((unsigned) &gdtPointer);
}

void gdt_set_entry(int n, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
	gdt[n].base_low			= (base & 0xFFFF);
	gdt[n].base_mid			= (base >> 16) & 0xFF;
	gdt[n].base_high		= (base >> 24) & 0xFF;
	
	gdt[n].limit_low		= (limit & 0xFFFF);
	gdt[n].flags_limit_high = ((limit >> 16) & 0x0F);
	gdt[n].flags_limit_high |= flags & 0xF0;
	gdt[n].access			= access;	
}
