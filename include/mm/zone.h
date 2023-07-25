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

/* this sets a size of biggest block to 4 M (default) */
# define MAX_ORDER 11

enum zone_selector
{
	ZONE_DMA,
	ZONE_NORMAL,
	ZONE_HIGH
};

/* buddy allocator */
struct free_area
{
	/* pages will be chained here */
	struct list_head free_list;
	/* block size */
	unsigned long nr_free;
};

struct zone
{
	char name[ZONE_NAME_MAX];

	/* used to allocate a page */
	struct free_area free_area[MAX_ORDER];
};

/* extern */
void mm_zone_init (void);
struct zone *mm_zone_get (enum zone_selector sel);

void add_page_zone (struct zone *_zone, struct page *page);

#endif
