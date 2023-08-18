#include "cpu/paging.h"

#include "mm/zone.h"
#include "mm/allocator.h"
#include "mm/compact.h"
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
	{
		return NULL;
		panic ("there is no more memory block");
	}

	/* split the upper block */
	block = get_block_by_order (zone, order + 1);
	if (block == NULL)
		return NULL;
	/* use first block and insert the rest block into list */
	remains = get_frame (get_frame_index (block) + (1 << order));
	list_add (&remains->chain, &zone->free_area[order].free_list);
	zone->free_area[order].nr_free++;

	return block;
}

struct page *alloc_pages (struct zone *zone, int num)
{
	/* this function allocates num pages from zone */
	struct page *leader, *cur, *next = NULL;
	int order, leader_index;
	int i;

	order = get_order_page (num);
	/* only get the leader block */
	leader = get_block_by_order (zone, order);
	if (!leader)
		return NULL;
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

void *free_pages (struct zone *zone, struct page *page)
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

static void *kmalloc_large (size_t size, gfp_t flags)
{
	struct zone *zone;
	struct page *page;
	int order;

	assert (flags & GFP_KERNEL);

	order = get_order_size (size);

	zone = mm_zone_get (ZONE_NORMAL);
	page = alloc_pages (zone, 1 << order);
	if (!page)
		return NULL;

	return page->virtual;
}

static void kfree_large (const void *ptr)
{
	struct zone *zone;
	uint32_t frame_index;

	frame_index = (uint32_t) get_physical_address ((void *) ptr) >> PAGE_SHIFT;
	zone = mm_zone_get (ZONE_NORMAL);

	free_pages (zone, get_frame (frame_index));
}

void *kmalloc (size_t size, gfp_t flags)
{
	if (size <= PAGE_SIZE / 2)
		return kmalloc_compact (size, flags);
	return kmalloc_large (size, flags);
}

void kfree (const void *ptr)
{
	if (kmalloc_is_compact (ptr))
	{
		kfree_compact (ptr);
	}
	else
	{
		kfree_large (ptr);
	}
}

size_t ksize (const void *ptr)
{
	struct list_head *head;
	uint32_t frame_index;
	size_t size;
	int i;

	size = ksize_compact (ptr);
	if (size != 0)
	{
		return size;
	}

	frame_index = (uint32_t) get_physical_address ((void *) ptr) >> PAGE_SHIFT;
	head = &get_frame (frame_index)->chain;
	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return PAGE_SIZE * i;
}
