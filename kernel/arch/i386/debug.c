#include <stdarg.h>
#include <stdio.h>
#include <kernel/debug.h>

void panic(char* message, ...)
{
	va_list argptr;
	va_start(argptr, message);
	
    printf("************************\n");
    printf("KERNEL PANIC:\n");
    printf("    ");
	vprintf(message, argptr);
	printf("\n\n");
    printf("System halted\n");
    printf("************************");
	
	va_end(argptr);
	
    while (1)
        ;
}

volatile int* stack_pointer()
{
	volatile int *esp = 0;

	asm __volatile__("movl %%esp, %0" : "=r"(esp));
	
	return esp;
}
