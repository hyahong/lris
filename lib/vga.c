#include "vga.h"
#ifdef i386
# include "asm-i386/port.h"
#else
# error "this build target is not supported."
#endif

struct vga sys_vga;

void vga_init (void)
{
	/* VGA Video RAM address B800:0000 */
	sys_vga.buffer = (uint16_t *) 0xB8000;
	sys_vga.cursor.x = 0;
	sys_vga.cursor.y = 0;
	/* default color: block background and white foreground */
	sys_vga.color.back = VGA_COLOR_BLACK;
	sys_vga.color.fore = VGA_COLOR_WHITE;
}

void _vga_draw (char character)
{
	int index;

	index = sys_vga.cursor.x + sys_vga.cursor.y * VGA_WIDTH;
	sys_vga.buffer[index] = (*((uint8_t *) &sys_vga.color) << 8) | character;	
}

void _vga_draw_escape (char escape)
{
	switch (escape)
	{
		/* line feed */
		case 0xA:
			vga_lf ();
			break;
		
		default:
			break;
	}
}

void vga_draw (char character, uint8_t forward)
{
	char escape[5] = {
		0x8, 0x9, 0xA, 0xB
	};
	int i;
	
	for (i = 0; i < 4; i++)
	{
		if (escape[i] == character)
		{
			_vga_draw_escape (character);
			return ;
		}
	}
	_vga_draw (character);

end:
	if (forward)
	{
		sys_vga.cursor.x++;
		if (sys_vga.cursor.x == VGA_WIDTH)
		{
			sys_vga.cursor.x = 0;
			sys_vga.cursor.y++;
		}
	}
}

void vga_set_color (uint8_t back, uint8_t fore)
{
	sys_vga.color.back = back;
	sys_vga.color.fore = fore;
}

void vga_set_position (uint8_t x, uint8_t y)
{
	sys_vga.cursor.x = x;
	sys_vga.cursor.y = y;
}

void _vga_set_cursor (uint8_t x, uint8_t y)
{
	int index;

	index = x + y * VGA_WIDTH;

	outb (0x3D4, 0x0E);
	outb (0x3D5, (uint8_t) ((index >> 8) & 0xFF));
	outb (0x3D4, 0x0F);
	outb (0x3D5, (uint8_t) (index & 0xFF));
}

void vga_set_cursor (void)
{
	_vga_set_cursor (sys_vga.cursor.x, sys_vga.cursor.y);
}

void vga_lf (void)
{
	sys_vga.cursor.x = 0;
	sys_vga.cursor.y++;
}

