#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

void PIC_clear_irq_mask(uint8_t);

void pic_init(void);

void PIC_remap_irqs(int, int);

void PIC_sendEOI(uint8_t);

void PIC_set_irq_mask(uint8_t);

#endif
