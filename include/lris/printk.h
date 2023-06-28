#ifndef _PRINTK_H_
# define _PRINTK_H_

# include "lris/types.h"

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

int printk (const char *format, ...);

#endif
