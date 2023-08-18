#include "driver/vga.h"
#include "driver/keyboard.h"
#include "cpu/paging.h"
#include "cpu/interrupt.h"

#include "mm/memory.h"
#include "mm/zone.h"
#include "mm/allocator.h"
#include "mm/compact.h"
#include "lris/list.h"
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

extern void unittest (void);

void kernel_init (void)
{
	vga_init ();
	interrupt_init ();
	mm_init ();
	keyboard_init ();

	vga_clear ();

	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
	printk ("\n\n%s\n\n\n", logo);
	vga_set_color (VGA_COLOR_BLACK, VGA_COLOR_WHITE);

	getty_init ();

	while (1)
	{
		keyboard_handler ();
	}
}
