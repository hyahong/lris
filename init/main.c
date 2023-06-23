#include "vga.h"
#include "asm-i386/port.h"
#include "linux/string.h"
#include "linux/keyboard.h"
#include "linux/printk.h"

void kernel_init (void)
{
	vga_init ();
	keyboard_init ();

	char *test = "test message thank you and this is test kernel to study referenced \
				  by KFS-1 now we can boot this kernel with qemu emulator";

	for (int i = 0; i < strlen (test); i++)
	{
		vga_draw (test[i], 1);
		vga_set_color (i % 15, 15 - i % 15);
	}

	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_WHITE);
	vga_draw ('.', 1);
	vga_lf ();
	vga_set_cursor ();

	while (1)
	{
		int d;

		if (!(inb (0x64) & 1))
			continue;

		keyboard_handler ();
	}
}
