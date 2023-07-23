#include "cpu/paging.h"

#include "lris/assert.h"
#include "lris/string.h"
#include "mm/mmzone.h"

/* zones are stacked in kernel memory space in order */
mm_zone_t mm_zone[ZONE_COUNT];

static void mm_zone_reset (void)
{
	int i;

	for (i = 0; i < ZONE_COUNT; i++)
	{
		strcpy (mm_zone[i].name, _mm_zone[i].name);
		mm_zone[i].mapped = _mm_zone[i].mapped;
		mm_zone[i].limit = _mm_zone[i].limit;
	}
}

static void mm_zone_check (void)
{
	int spaces;
	int i;

	spaces = 0;
	for (i = 0; i < ZONE_COUNT; i++)
		spaces += mm_zone[i].limit;

	assert (spaces == KERNEL_MEMORY_SIZE);
}

void mm_zone_init (void)
{
	mm_zone_reset ();
	mm_zone_check ();
}
