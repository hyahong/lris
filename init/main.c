#include "vga.h"
#include "linux/string.h"
#include "linux/keyboard.h"
#include "linux/printk.h"
#include "cpu/interface.h"

void kernel_init (void)
{
	char *test = "test message thank you and this is test kernel to study referenced \
				  by KFS-1 now we can boot this kernel with qemu emulator";

	segment_init ();
	vga_init ();
	keyboard_init ();

	printk ("test: %X &test: %X\n", test, &test);

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
		keyboard_handler ();
	}
}
