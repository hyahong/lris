#include "vga.h"
#include "port.h"

struct vga sys_vga;

void vga_init (void)
{
	/* VGA Video RAM address */
	sys_vga.buffer = (uint16_t *) VGA_BASE_ADDRESS;
	sys_vga.cursor.x = 0;
	sys_vga.cursor.y = 0;
	/* default color: block background and white foreground */
	sys_vga.color.back = VGA_COLOR_BLACK;
	sys_vga.color.fore = VGA_COLOR_WHITE;
}

static void _vga_draw (char character)
{
	int index;

	index = sys_vga.cursor.x + sys_vga.cursor.y * VGA_WIDTH;
	sys_vga.buffer[index] = (*((uint8_t *) &sys_vga.color) << 8) | character;
}

/*
 * escape sequence
 */
static void _vga_draw_escape (char escape)
{
	switch (escape)
	{
		/* back space */
		case 0x8:
			break;

		/* horizon tab */
		case 0x9:
			break;

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

	if (forward)
	{
		sys_vga.cursor.x++;
		if (sys_vga.cursor.x == VGA_WIDTH)
			vga_lf ();
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

static void _vga_set_cursor (uint8_t x, uint8_t y)
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

void vga_backspace (void)
{
	int index;

	index = sys_vga.cursor.x + sys_vga.cursor.y * VGA_WIDTH;
	sys_vga.buffer[index - 1] = VGA_NULL;
	if (sys_vga.cursor.x == 0)
	{
		/* handling */
	}
	else
	{
		sys_vga.cursor.x--;
	}
}

void vga_lf (void)
{
	sys_vga.cursor.x = 0;
	sys_vga.cursor.y++;
	
	if (sys_vga.cursor.y == VGA_HEIGHT)
		vga_scroll ();
}

void vga_scroll (void)
{
	int i;

	for (i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
		sys_vga.buffer[i] = sys_vga.buffer[i + VGA_WIDTH];
	for (i = 0; i < VGA_WIDTH; i++)
		sys_vga.buffer[i + VGA_WIDTH * (VGA_HEIGHT - 1)] = VGA_NULL;
	--sys_vga.cursor.y;
	vga_set_cursor ();
}

void vga_clear (void)
{
	int i;

	for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		sys_vga.buffer[i] = VGA_NULL;
	sys_vga.cursor.x = 0;
	sys_vga.cursor.y = 0;
	vga_set_cursor ();
}
