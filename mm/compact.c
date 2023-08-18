#include "cpu/paging.h"

#include "mm/allocator.h"
#include "mm/zone.h"
#include "lris/string.h"
#include "lris/assert.h"

void kmalloc_compact_init (void)
{
	struct compact_area *compact;
	struct zone *zone;
	int count[] = COMPACT_PAGE_COUNT;
	int i;

	zone = mm_zone_get (ZONE_NORMAL);

	for (i = 0; i < COMPACT_COUNT; i++)
	{
		compact = &zone->compact_area[i];

		compact->size = 32 << i;
		memset (compact->bitmap, 0, sizeof (uint32_t) * COMPACT_BITMAP_MAX_SIZE);
		compact->available = count[i] * PAGE_SIZE / compact->size;
		compact->page.ptr = alloc_pages (zone, count[i])->virtual;
		compact->page.count = count[i];
	}
}

static uint32_t kmalloc_compact_get_order (uint32_t size)
{
	if (0 < size && size <= 32)
		return 0;
	else if (32 < size && size <= 64)
		return 1;
	else if (64 < size && size <= 128)
		return 2;
	else if (128 < size && size <= 256)
		return 3;
	else if (256 < size && size <= 512)
		return 4;
	else if (512 < size && size <= 1024)
		return 5;
	else if (1024 < size && size <= 2048)
		return 6;
	return -1;
}

static void *kmalloc_get_compact (struct compact_area *compact)
{
	void *address;
	int i;

	if (compact->available == 0)
	{
		panic ("there are no more compact blocks");
	}

	for (i = 0; i < COMPACT_BITMAP_MAX_SIZE * 32; i++)
	{ 
		if (((compact->bitmap[i / 32] >> (i % 32)) & 1) == 0)
		{	
			break;
		}
	}

	compact->bitmap[i / 32] |= (1 << (i % 32));
	address = (void *) (((char *) compact->page.ptr) + i * compact->size);
	
	compact->available--;

	return address;
}

int kmalloc_is_compact (const void *ptr)
{
	struct compact_area *compact;
	struct zone *zone;
	int i;

	zone = mm_zone_get (ZONE_NORMAL);

	for (i = 0; i < COMPACT_COUNT; i++)
	{
		compact = &zone->compact_area[i];

		if (compact->page.ptr <= ptr && ptr < compact->page.ptr + PAGE_SIZE * compact->page.count)
		{
			return 1;
		}
	}
	return 0;
}

void *kmalloc_compact (uint32_t size, gfp_t flags)
{
	struct zone *zone;
	int order;

	assert (flags & GFP_KERNEL);

	zone = mm_zone_get (ZONE_NORMAL);
	order = kmalloc_compact_get_order (size);
	if (order == -1)
	{
		panic ("can't find the propriate order");
	}

	return kmalloc_get_compact (&zone->compact_area[order]);
}

void *kfree_compact (const void *ptr)
{
	struct compact_area *compact;
	struct zone *zone;
	int i;

	zone = mm_zone_get (ZONE_NORMAL);

	for (i = 0; i < COMPACT_COUNT; i++)
	{
		compact = &zone->compact_area[i];

		if (compact->page.ptr <= ptr && ptr < compact->page.ptr + PAGE_SIZE * compact->page.count)
		{
			break;
		}
	}

	i = ((uint32_t) ptr - (uint32_t) compact->page.ptr) / compact->size;
	compact->bitmap[i / 32] ^= (1 << (i % 32));
}

size_t ksize_compact (const void *ptr)
{
	struct compact_area *compact;
	struct zone *zone;
	int i;

	zone = mm_zone_get (ZONE_NORMAL);

	for (i = 0; i < COMPACT_COUNT; i++)
	{
		compact = &zone->compact_area[i];

		if (compact->page.ptr <= ptr && ptr < compact->page.ptr + PAGE_SIZE * compact->page.count)
		{
			return compact->size;
		}
	}
	return 0;
}
