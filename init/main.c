#include "vga.h"
#include "driver/keyboard.h"
#include "cpu/interface.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "peripheral/getty.h"

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
	segment_init ();
	vga_init ();
	keyboard_init ();

	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
	printk ("%s\n\n\n", logo);
	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_WHITE);

	char *test = "this is test message. can you find this message in memory using a dump?";
	char *test2 = "what...";

	printk ("%x\n", &test);
	printk ("%x\n", &test2);
	printk ("%x\n", test2);

	getty_init ();

	while (1)
	{
		keyboard_handler ();
	}
}
