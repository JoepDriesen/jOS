#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/multiboot.h>

// We'll divide the memory into chunks of 4KiB and will assume a size of 4GiB (1 048 576 blocks).
// A bitmap will be used as bookkeeping, where a 1 means a memory block is used, and a 0 means it is
// free. Bit i represent the memory block at address i * 4KiB.
// So we need 1 048 576 / 32 = 32 768 uint32_t's to represent our entire memory
#define MEMORY_BITMAP_SIZE 32768
#define BLOCK_SIZE 4096

// Space was reserved for this bitmap in the boot and linker script
extern uint32_t memory_bitmap[];

extern uint32_t kernelStart;
extern uint32_t kernelEnd;

// We cannot use the multiboot_memory_map_t structure from multiboot.h, since this uses int64's, which
// are not supported
typedef struct memory_map {
	uint32_t size;
	uint32_t addr_low,addr_high;
	uint32_t len_low,len_high;
	uint32_t type;
} memory_map_t;

static void set_memory_availability(uint32_t, uint32_t, bool);

void memory_init(multiboot_info_t* mbt)
{
	// Start with all memory flagged as unavailable
	memset(memory_bitmap, 0xFF, MEMORY_BITMAP_SIZE);
	
	// Check the memory map provided by grub to find available memory
	memory_map_t* mmap = (memory_map_t*) mbt->mmap_addr;
	while (mmap < (memory_map_t*) mbt->mmap_addr + mbt->mmap_length) {
		// Memory above 4GiB is not supported
		if (mmap->addr_high > 0)
			continue;

		if (mmap->type == 1)
		{
			uint32_t startAddr = mmap->addr_low / BLOCK_SIZE;
			if (mmap->addr_low % BLOCK_SIZE != 0)
				startAddr += BLOCK_SIZE;
			
			uint32_t endAddr = mmap->addr_low + mmap->len_low;
			// Check for overflow in case there is more than 4GiB of RAM
			if (endAddr < startAddr)
				endAddr = 0xFFFFFFFF;
			
			// This entry describes memory that is available for use
			set_memory_availability(startAddr, endAddr, true);
		}
		
		// Get the next memory map entry
		mmap = (memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}
	
	// Memory below 1 MiB is off limits
	set_memory_availability(0, 0x100000, false);
	
	// Kernel address space is off limits
	set_memory_availability((uint32_t) &kernelStart, (uint32_t) &kernelEnd + BLOCK_SIZE, false);
}

static void set_memory_availability(uint32_t startAddr, uint32_t endAddr, bool isAvailable)
{
	// Align to 4KiB
	uint32_t startBlock = startAddr / BLOCK_SIZE;
	
	uint32_t numBlocks = (endAddr - startBlock) / BLOCK_SIZE;
	
	uint32_t bitmapIndex = startBlock / 32;
	uint32_t bitIndex = startBlock % 32;
	
	for (uint32_t i = 0; i < numBlocks; i++)
	{
		if (bitIndex >= 32)
		{
			bitmapIndex++;
			bitIndex = 0;
		}
		
		if (isAvailable)
			memory_bitmap[bitmapIndex] &= ~(1 << bitIndex);
		else
			memory_bitmap[bitmapIndex] |= 1 << bitIndex;
		
		bitIndex++;
	}
}
