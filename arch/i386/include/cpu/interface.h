#ifndef _SEGMENT_H_
# define _SEGMENT_H_

# include "linux/types.h"

# define GDT_BASE_ADDRESS 0x800

#define GDT_ENTRY_KERNEL_CS		1
#define GDT_ENTRY_KERNEL_DS		2
#define GDT_ENTRY_KERNEL_SS		3

#define GDT_ENTRY_USER_CS		4
#define GDT_ENTRY_USER_DS		5
#define GDT_ENTRY_USER_SS		6

#define GDT_SEGMENT_KERNEL_CODE 

struct gdt_entry
{
	uint32_t limit_low:16;
	uint32_t base_low:24;
	uint32_t access:8;
	uint32_t limit_high:4;
	uint32_t flag:4;
	uint32_t base_high:8;
} __attribute__((packed));

void segment_init (void);

#endif
