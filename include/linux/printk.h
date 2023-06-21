#ifndef _PRINTK_H_
# define _PRINTK_H_

# include "linux/types.h"

# define PRINTK_BUFFER_SIZE 512

struct printk_info
{
	const char *format;
	char buffer[PRINTK_BUFFER_SIZE + 1];
	uint32_t size;
	uint32_t offset;
};

int printk (const char *format, ...);

#endif
