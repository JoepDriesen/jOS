#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

#define TAB_SPACES	4

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static void increase_terminal_row(void)
{
	if (++terminal_row > VGA_HEIGHT)
	{
		terminal_row = 0;
	}
}

static void increase_terminal_column(void)
{
	if (++terminal_column > VGA_WIDTH)
	{
		increase_terminal_row();
		terminal_column = 0;
	}
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\n')
	{
		terminal_column = 0;
		increase_terminal_row();
	}
	else if (c == '\t')
	{
		for (int i = 0; i < TAB_SPACES; i++)
			terminal_putchar(' ');
	}
	else
	{	
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		
		increase_terminal_column();	
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
