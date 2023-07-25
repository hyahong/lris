#ifndef _ASSERT_H_
# define _ASSERT_H_

# include "driver/vga.h"

# include "lris/string.h"

void __panic_draw (char *msg, char *file, int line);

# define halt(x) do { asm volatile ("hlt"); } while (0);
# define panic(x) do { __panic_draw (x, __FILE__, __LINE__); halt (); } while (0);
# define assert(x) do { if (!(x)) panic("assertion failed") } while (0);

#endif
