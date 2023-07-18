#ifndef _ASSERT_H_
# define _ASSERT_H_

# include "vga.h"

# include "lris/string.h"

inline void __panic_draw (char *file, int line)
{
	char *msg = "kernel panic occurs at ";
	char buffer[8];
	int i, j;

	vga_set_position (0, 0);
	vga_set_color (VGA_COLOR_RED, VGA_COLOR_WHITE);
	for (i = 0; i < VGA_WIDTH * VGA_HEIGHT - 1; i++)
		vga_draw (' ', 1);

	vga_set_position (0, 0);
	for (i = 0; i < strlen (msg); i++)
		vga_draw (msg[i], 1);
	for (i = 0; i < strlen (file); i++)
		vga_draw (file[i], 1);
	
	vga_draw (':', 1);
	i = 0;
	do
	{
		buffer[i] = '0' + line % 10;
		i++;
	}
	while (line /= 10);
	for (j = 0; j < i; j++)
		vga_draw (buffer[i - j - 1], 1);
}

# define halt(x) do { asm volatile ("hlt"); } while (0);
# define panic(x) do { __panic_draw (__FILE__, __LINE__); halt (); } while (0);
# define assert(x) do { if (!(x)) panic(x) } while (0);

#endif
