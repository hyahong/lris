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

/* zone */
# define ZONE_COUNT 3
# define ZONE_NAME_MAX 16

enum zone_selector
{
	ZONE_DMA,
	ZONE_NORMAL,
	ZONE_HIGH
};

/* buddy allocator */

/* this sets a size of biggest block to 4 M (default) */
# define MAX_ORDER 11

struct free_area
{
	/* pages will be chained here */
	struct list_head free_list;
	/* block size */
	unsigned long nr_free;
};

/* compact */
# define COMPACT_COUNT 7

/* 32, 64, 128, 256, 512, 1024, 2048 */
# define COMPACT_PAGE_COUNT { 1, 1, 1, 1, 1, 1, 1 }
# define COMPACT_BITMAP_MAX_SIZE (1 * PAGE_SIZE / 32 / 32)

struct compact_area
{
	uint16_t size;
	uint32_t bitmap[COMPACT_BITMAP_MAX_SIZE];

	uint16_t available;

	struct
	{
		void *ptr;
		uint16_t count;
	} page;
};

struct zone
{
	char name[ZONE_NAME_MAX];

	/* used to allocate a page */
	struct free_area free_area[MAX_ORDER];

	/* compact allocator */
	struct compact_area compact_area[COMPACT_COUNT];
};

/* extern */
void mm_zone_init (void);
struct zone *mm_zone_get (enum zone_selector sel);

void add_page_zone (struct zone *_zone, struct page *page);

#endif
