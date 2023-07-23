#ifndef _MMZONE_H_
# define _MMZONE_H_

# include "lris/types.h"

/*
 * ######    Physical                        Virtual
 * HIGH    < 896 ~ 1024 MB	                 ~ 4 GB (0xFFFF FFFF)       changeable paging
 * ######
 * NORMAL  < 16 ~ 896 MB (0x3800 0000)                                  kernel mapped
 * ######
 * DMA     < 0 ~ 16 MB (0x0100 0000)         3 GB (0xC000 0000) ~       kernel mapped
 * ######
 */

# define ZONE_COUNT 3
# define ZONE_NAME_MAX 16

enum _mm_zone_name
{
	ZONE_DMA,
	ZONE_NORMAL,
	ZONE_HIGH
};

typedef struct mm_zone_struct mm_zone_t;
struct mm_zone_struct
{
	char name[ZONE_NAME_MAX];

	/* paged address */
	uint32_t mapped;
	uint32_t limit;
};

/* sample */
/* zones are stacked in kernel memory space in order */
static mm_zone_t _mm_zone[ZONE_COUNT] = {
	{ "DMA", 0, 0x1000000 },
	{ "NORMAL", 0, 0x37000000 },
	{ "HIGH", 0, 0x8000000 }
};

/* extern */
extern mm_zone_t mm_zone[ZONE_COUNT];

void mm_zone_init (void);

#endif
