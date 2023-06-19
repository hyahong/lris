#ifndef _VGA_H_
# define _VGA_H_

# define VGA_WIDTH 80
# define VGA_HEIGHT 25

# include "linux/types.h"

struct vga
{
	uint16_t *buffer;
	struct
	{
		uint8_t x;
		uint8_t y;
	} cursor;
	struct
	{
		uint8_t fore:4;
		uint8_t back:4;
	} color;
};

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void vga_init (void);
void vga_draw (char character, uint8_t forward);

#endif
