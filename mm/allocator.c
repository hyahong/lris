#include "cpu/paging.h"

#include "mm/zone.h"
#include "mm/allocator.h"
#include "lris/list.h"
#include "lris/assert.h"

static int get_order_size (size_t size)
{
	int order;
	int roundup;

	roundup = 0;
	if (size & 0xFFF)
		roundup = 1;
	
	size >>= 13;
	for (order = 0; size; order++)
		size >>= 1;

	return order + roundup;
}

static int get_order_page (int num)
{
	int order;

	if (num > (1 << (MAX_ORDER - 1)))
		panic ("num is too large");

	order = 0;
	while (1)
	{
		if ((1 << order) >= num)
			break;
		order++;
	}

	return order;
}

struct page *get_block_by_order (struct zone *zone, int order)
{
	struct page *block, *remains;

	if (zone->free_area[order].nr_free)
	{
		/* first block */
		block = list_entry (zone->free_area[order].free_list.next, struct page, chain);
		/* get it */
		list_del (&block->chain);
		zone->free_area[order].nr_free--;
		
		return block;
	}
	
	/* if current order is the end */
	if (order == MAX_ORDER - 1)
		panic ("there is no more memory block");

	/* split the upper block */
	block = get_block_by_order (zone, order + 1);
	/* use first block and insert the rest block into list */
	remains = get_frame (get_frame_index (block) + (1 << order));
	list_add (&remains->chain, &zone->free_area[order].free_list);
	zone->free_area[order].nr_free++;

	return block;
}

struct page *alloc_page (struct zone *zone, int num)
{
	/* this function allocates num pages from zone */
	struct page *leader, *cur, *next = NULL;
	int order, leader_index;
	int i;

	order = get_order_page (num);
	/* only get the leader block */
	leader = get_block_by_order (zone, order);
	leader->chain.prev = NULL;
	/* chaining */
	leader_index = get_frame_index (leader);
	for (i = 0; i < (1 << order) - 1; i++)
	{
		cur = get_frame (leader_index + i);
		next = get_frame (leader_index + i + 1);
		
		cur->chain.next = &next->chain;
		next->chain.prev = &cur->chain;
	}
	if (next)
		next->chain.next = NULL;

	return leader;
}

void *free_page (struct zone *zone, struct page *page)
{
	struct list_head *head, *next;

	head = &page->chain;
	while (head)
	{
		next = head->next;
		/* break the link */
		head->prev = NULL;
		head->next = NULL;
		/* return the page */
		add_page_zone (zone, list_entry (head, struct page, chain));
		head = next;
	}
}

static void *buddy_alloc (size_t size, gfp_t flags)
{
	struct zone *zone;
	struct page *page;
	int order;

	assert (flags & GFP_KERNEL);

	order = get_order_size (size);

	zone = mm_zone_get (ZONE_NORMAL);
	page = alloc_page (zone, 1 << order);

	return page->virtual;
}

static void buddy_free (const void *ptr)
{
	struct zone *zone;
	uint32_t frame_index;

	frame_index = get_physical_address (ptr) >> PAGE_SHIFT;
	zone = mm_zone_get (ZONE_NORMAL);

	free_page (zone, get_frame (frame_index));
}

static void *slab (size_t size, gfp_t flags)
{
	printk ("slab\n");

	return NULL;
}

void *kmalloc (size_t size, gfp_t flags)
{
//	if (size < PAGE_SIZE * 2)
//		return slab (size, flags);
	return buddy_alloc (size, flags);
}

void kfree (const void *ptr)
{
	/* slab or buddy */
	/* if buddy */
	buddy_free (ptr);
}
