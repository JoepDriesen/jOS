.section .text
.global gdt_flush
.type gdt_flush, @function

gdt_flush:
	# Get pointer to gdt table off the stack
	mov 4(%esp), %eax
	
	# Load the pointer into the GDT register
	lgdt (%eax)
	
	# 0x10 is the offset in the GDT to our data segment	
	mov $0x10, %ax
	
	# Load all data segment selectors
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	
	# 0x08 is the offset to our code segment: Far jump!
	ljmp $0x08, $flush
		
flush:	
	# return to C calling function
	ret				
