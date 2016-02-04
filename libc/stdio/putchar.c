#include <stdio.h>

#if defined(__is_jos_kernel)
#include <kernel/tty.h>
#endif

int putchar(int ic)
{
#if defined(__is_jos_kernel)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: Implement write system call
#endif
	return ic;
}

int putint(int num)
{
	int pow = 1;
	int max = 10;
	
	while (num >= max)
	{
		max *= 10;
		pow++;
	}
	
//	while (pow > 0)
//	{
//		max /= 10;
//		pow--;
		
//		int high = num / max;
		putchar('0' + num);
//	}
}
