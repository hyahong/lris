#include "cpu/segment.h"
#include "cpu/paging.h"

#include "mm/zone.h"
#include "mm/memory.h"

void mm_init (void)
{
	segment_init ();
	paging_init ();

	mm_zone_init ();
}
