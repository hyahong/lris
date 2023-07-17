#include "vga.h"
#include "driver/keyboard.h"
#include "cpu/segment.h"
#include "cpu/page.h"
#include "cpu/interrupt.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "peripheral/getty.h"
#include "lris/assert.h"

char *logo = "\n" \
"       :::        :::::::::  ::::::::::: ::::::::\n" \
"      :+:        :+:    :+:     :+:    :+:    :+:\n" \
"     +:+        +:+    +:+     +:+    +:+\n" \
"    +#+        +#++:++#:      +#+    +#++:++#++\n" \
"   +#+        +#+    +#+     +#+           +#+\n" \
"  #+#        #+#    #+#     #+#    #+#    #+#\n" \
" ########## ###    ### ########### ########\n";

void kernel_init (void)
{
	interrupt_init ();
	vga_init ();
	keyboard_init ();

	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
	printk ("%s\n\n\n", logo);
	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_WHITE);

	char *test = "this is test message. can you find this message in memory using a dump?";

	printk ("%x %x\n", test, &test);

	getty_init ();

	while (1)
	{
		keyboard_handler ();
	}
}
