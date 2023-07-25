#include "cpu/paging.h"

#include "mm/zone.h"
#include "lris/assert.h"
#include "lris/string.h"
#include "lris/list.h"

/* zones are stacked in kernel memory space in order */
static struct zone zone[ZONE_COUNT];

static void mm_zone_reset (void)
{
	char *names[ZONE_COUNT] = {
		"DMA", "NORMAL", "HIGH"
	};
	int i, j;

	/* set a each name */
	for (i = 0; i < ZONE_COUNT; i++)
	{
		strcpy (zone[i].name, names[i]);

		for (j = 0; j < MAX_ORDER; j++)
		{
			LIST_HEAD_INIT (zone[i].free_area[j].free_list);
			zone[i].free_area[j].nr_free = 0;
		}
	}
}

struct zone *mm_zone_get (enum zone_selector sel)
{
	return &zone[sel];
}

static struct page *mm_zone_find_pair (struct zone *_zone, struct page *p, int order)
{
	struct page *it;
	int index;
	int align;

	align = 1 << (order + 1);
	/* if a result is 0, a index of pair page is the sum of index of p and align / 2 */
	index = get_frame_index (p);
	if (index % align)
		index -= (align >> 1);
	else
		index += (align >> 1);

	list_for_each_entry (it, &_zone->free_area[order].free_list, chain)
	{
		if (get_frame_index (it) == index)
			return it;
	}
	return NULL;
}

static struct page *get_pair_leader (struct page *p1, struct page *p2)
{
	if (get_frame_index (p1) > get_frame_index (p2))
		return p2;
	return p1;
}

void add_page_zone (struct zone *_zone, struct page *page)
{
	struct page *pair;
	int order;

	order = 0;
	while (1)
	{
		/* if order has reached the end */
		if (order == MAX_ORDER - 1)
			goto insert;

		/* find a pair of current page without caring the order */
		pair = mm_zone_find_pair (_zone, page, order);
		if (pair)
		{
			list_del (&pair->chain);
			_zone->free_area[order].nr_free--;
			page = get_pair_leader (page, pair);
			order++;
			continue;
		}

insert:
		/* insert the page */
		/* in this point, a pointer page indicates leader of pair */
		list_add (&page->chain, &_zone->free_area[order].free_list);
		_zone->free_area[order].nr_free++;
		break;
	}
}

void mm_zone_area_init (void)
{
	struct page *page;

	/* reserved pages are stacked in buddy allocator */
	while ((page = get_reserved_page ()))
		add_page_zone (&zone[ZONE_NORMAL], page);
}

void mm_zone_init (void)
{
	mm_zone_reset ();
	mm_zone_area_init ();
}
