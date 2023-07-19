#include "cpu/paging.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "lris/assert.h"

page_directory_t *mm_page_directory;
page_table_t *mm_page_tables;

struct page mm_pages[PAGE_DIRECTORY_COUNT * PAGE_TABLE_COUNT];

static void paging_frame_init (void)
{
	int i;

	mm_page_directory = (page_directory_t *) &_init_page_directory;
	mm_page_tables = (page_table_t *) &_init_page_tables;
	
	/* normal zone in physical */
	for (i = 0; i < NORMAL_ZONE_PAGE_COUNT; i++)
	{
		mm_pages[i].flags = PG_reserved;
		mm_pages[i]._refcount = 1;
		mm_pages[i].virtual = VMA_OFFSET + i * PAGE_SIZE;
	}
	/* unused in physical */
	for (i = NORMAL_ZONE_PAGE_COUNT; i < PAGE_COUNT; i++)
	{
		mm_pages[i].flags = 0;
		mm_pages[i]._refcount = 0;
		mm_pages[i].virtual = 0;
	}
}

static void *page_address (const struct page *page)
{
	return 0;
}

static void set_page_address(struct page *page, void *address)
{
}

void paging_init (void)
{
	paging_frame_init ();

	klog ("Initialized %d pages for paging", NORMAL_ZONE_PAGE_COUNT);
}
