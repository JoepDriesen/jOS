#ifndef _DEBUG_H
#define _DEBUG_H

void panic(char*, ...);

volatile int* stack_pointer();

#endif
