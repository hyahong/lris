#ifndef _PRINTK_H_
# define _PRINTK_H_

# include "lris/types.h"
# include "lris/stdarg.h"

# define PRINTK_BUFFER_SIZE 512

# define PRINTK_FLAG_WIDTH 0x01

struct printk_info
{
	const char *format;
	char buffer[PRINTK_BUFFER_SIZE + 1];
	uint32_t size;
	uint32_t offset;

	/* flags */
	uint32_t flags;
	struct
	{
		int width;
		char fill;
	} flag;
};

/* extern */
int printk (const char *format, ...);
int vprintk (const char *format, va_list args);

void klog (const char *format, ...);

#endif
