# Declare constants used for creating a multiboot header.
.set ALIGN,		1<<0			# align loaded modules on page boundaries
.set MEMINFO,	1<<1			# provide memory map
.set FLAGS,		ALIGN | MEMINFO	# this is the Multiboot 'flag' field
.set MAGIC,		0x1BADB002		# 'magic number' lets bootloader find the header
.set CHECKSUM,	-(MAGIC + FLAGS)# checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot standard. We put this into a special section so we can force the 
# header to be in the start of the final program.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# $esp points at anything an using it may cause massive harm. Instead, allocate room for our own stack by
# creating a symbol at the bottom of it, then allocating 16384 bytes for it, and finally creating a symbol
# at the top.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the bootloader will jump to
# this position once the kernel has been loaded.
.section .text
.global _start
.type _start, @function
_start:
	# KERNEL MODE

	# To set up a stack, we simply set the esp register to point to the top of our stack (as it grows
	# downwards).
	movl $stack_top, %esp
	
	# We can now execute C code.
	
	# Initialize the core kernel before running the global constructors
	call kernel_early
	
	# Call the global constructors
	call _init
	
	# Transfer control to the main kernel
	call kernel_main
	
	# Infinite loop, don't go here
	cli
	hlt
.Lhang:
	jmp .Lhang

# Set the size of the _start symbol to the current location '.' minus its start. This is useful when
# debugging or when implementing call tracing
.size _start, . - _start