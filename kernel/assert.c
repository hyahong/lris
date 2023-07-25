#include "lris/assert.h"

void __panic_draw (char *msg, char *file, int line)
{
	char *location = "kernel panic occurs at ";
	uint16_t *vga = (uint16_t *) VGA_BASE_ADDRESS;
	char buffer[8];
	int i, j;

	for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga[i] = (VGA_COLOR_RED << 12) | ' ';

	vga_set_color (VGA_COLOR_RED, VGA_COLOR_WHITE);
	vga_set_position (0, 0);
	for (i = 0; i < strlen (location); i++)
		vga_draw (location[i], 1);
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

	vga_set_position (0, 1);
	for (i = 0; i < strlen (msg); i++)
		vga_draw (msg[i], 1);
}
