#include "cpu/segment.h"
#include "cpu/paging.h"

#include "mm/zone.h"

void mm_init (void)
{
	segment_init ();
	paging_init ();

	/* buddy */
	mm_zone_init ();
	/* compact allocator */
	kmalloc_compact_init ();
}
