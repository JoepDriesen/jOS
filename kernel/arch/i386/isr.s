.extern isr_handler
.extern irq_handler

.section .text

# All the ISR macros call this function
isr_common_stub:
	# Save all the GP registers
	pusha

	# Get the data segment
	mov %ds, %eax
	# Save it to the stack
	push %eax
	# Change to the kernel segment
	mov $0x10, %eax
	# In all the segment registers
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs

	# Store register pointer
	mov %esp, %ebx
	
	# Make room for calling function
	sub $4, %esp
	
	# Align stack to 16 Bytes
	and $0xFFFFFFF0, %esp

	# Put register pointer into stack
	mov %ebx, (%esp)

	# Call the C handler for interrupts
	call isr_handler

	# Restore Stack for calling function
	mov %ebx, %esp
	
	# Restore original segment
	pop %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	
	# Restore the general purpose registers
	popa
	
	sti
	
	# Clean up the Stack
	add $8, %esp
	
	# Return to the interrupted function
	iret

# All the IRQ macros call this function
irq_common_stub:
	# Save all the GP registers
	pusha
	
	# Get the data segment
	mov %ds, %eax
	
	# Save it to the stack
	push %eax
	
	# Change to the kernel segment
	mov $0x10, %eax
	# In all the segment registers
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	
	# Save the stack pointer in ebx
	mov %esp, %ebx
	
	# Add spot in stack
	sub $4, %esp
	
	# Align stack for gcc (16 byte boundary)
	and $0xFFFFFFF0, %esp
	
	# Putin interrupted register pointer on the stack
	mov %ebx, (%esp)
	
	# Call the C handler for IRQs	
	call irq_handler
	
	# Restore old stack
	mov %ebx, %esp
	
	# Restore original segment
	pop %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	
	# Restore general purpose registers
	popa
	# Clean up stack
	add $8, %esp
	
	sti
	iret



.altmacro

# Most interrupts are software, they don't have error codes
.macro ISR_NOERRCODE num
	.global isr\num
	isr\num:
		cli
		# Fake error code
		push $0
		# Interrupt number
		push \num
		# Force a far jump to pad them all
		jmp $0x08, $isr_common_stub
.endm

# Some CPU exceptions have an error code
.macro ISR_ERRCODE num
	.global isr\num
	isr\num:
		cli
		nop
		nop
		nop
		nop
		nop
		nop
		push $\num
		jmp $0x08, $isr_common_stub
.endm

# IRQs require additional C code
.macro IRQ num, line
	.global irq\line
	irq\line:
		cli
		push 0
		push $\num
		ljmp $0x08, $irq_common_stub
.endm


# Interrupt Declarations
.set i, 0

# The first 8 interrupts don't have error codes
.rept 8
	ISR_NOERRCODE %i
	.set i, i + 1
.endr

# 9 does have error codes
ISR_ERRCODE 8
# 10 doesn't
ISR_NOERRCODE 9

.set i, 10

# Next 5 have error codes
.rept 5
	ISR_ERRCODE %i
	.set i, i + 1
.endr

.rept 17
	ISR_NOERRCODE %i
	.set i, i + 1
.endr

# 16 IRQs
.rept 16
	# J is the IRQ line number
	.set j, i - 32
	IRQ %i, %j
	.set i, i + 1
.endr

.rept 80
	ISR_NOERRCODE %i
	.set i, i + 1
.endr
