#include "cpu/paging.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "lris/assert.h"

page_directory_t *page_directory;
page_table_t *page_tables;

struct page mm_frames[1024 * 1024];

/*
 * This function maps the kernel area (in physical address, 0~896 MB)
 * to virtual address (0xC0000000 ~ )
 */
static void paging_init_mapping (void)
{
	int i;

	page_directory = (page_directory_t *) &_init_page_directory;
	page_tables = (page_table_t *) &_init_page_tables;
	/* first 1024 pages have already been mapped */
	/* table mapping */
	for (i = 1024; i < NORMAL_ZONE_PAGE_COUNT; i++)
		((int *) page_tables)[i] = (PAGE_SIZE * i) | PG_WRITABLE | PG_PRESENT;
	/* directory mapping */
	for (i = 1; i < NORMAL_ZONE_PAGE_COUNT / 1024; i++)
		((int *) page_directory)[i + 768] = ((int) &page_tables - VMA_OFFSET + i * 4096) | PG_WRITABLE | PG_PRESENT;

	/* do not need to flush a TLB */
}

static void *page_address (const struct page *page)
{
	
}

static void set_page_address(struct page *page, void *address)
{
}

void paging_init (void)
{
	paging_init_mapping ();

	klog ("Remapped %d pages for paging", NORMAL_ZONE_PAGE_COUNT);
}
