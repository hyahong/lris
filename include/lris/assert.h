#ifndef _ASSERT_H_
# define _ASSERT_H_

# include "lris/printk.h"

# define halt(x) do { asm volatile ("hlt"); } while (0);
# define panic(x) do { printk ("\nkernel panic occurs at %s:%d\n", __FILE__, __LINE__); halt (); } while (0);
# define assert(x) do { if (!(x)) panic(x) } while (0);

#endif
