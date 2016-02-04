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
