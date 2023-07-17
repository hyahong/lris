#ifndef _MMZONE_H_
# define _MMZONE_H_

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

# define ZONE_DMA 0
# define ZONE_NORMAL 1
# define ZONE_HIGH 2

typedef int pageoff_t;

typedef struct mm_zone mm_zone_t;
struct mm_zone
{
	char name[ZONE_NAME_MAX];

	/* a pg_end'th page is not included in zone */
	pageoff_t pg_start;
	pageoff_t pg_end;
};

/* extern */
extern mm_zone_t mem_zone[ZONE_COUNT];

void mm_zone_init (void);

#endif
