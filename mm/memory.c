#include "cpu/segment.h"
#include "cpu/paging.h"

#include "mm/mmzone.h"
#include "mm/memory.h"

void mm_init (void)
{
	/* config */
	mm_zone_init ();

	segment_init ();
	paging_init ();
}
