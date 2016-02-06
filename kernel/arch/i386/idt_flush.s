.section .text
.global idt_flush
.type idt_flush, @function

idt_flush:
	# Get the IDT pointer location from the stack
	mov 4(%esp), %eax

	# Load into Control Register
	lidt (%eax)

	# Enable Hardware Interrupts
	sti

	ret
