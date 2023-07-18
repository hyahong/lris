#include "cpu/paging.h"

#include "lris/string.h"
#include "lris/assert.h"

/*
 * This function maps the kernel area (in physical address, 0~896 MB)
 * to virtual address (0xC0000000 ~ )
 */
static void paging_init_mapping (void)
{
	int i;

	/* first 1024 pages have already been mapped */
	/* table mapping */
	for (i = 1024; i < NORMAL_ZONE_PAGE_COUNT; i++)
		((int *) &page_tables)[i] = (PAGE_SIZE * i) | PG_WRITABLE | PG_PRESENT;
	/* directory mapping */
	for (i = 1; i < NORMAL_ZONE_PAGE_COUNT / 1024; i++)
		((int *) &page_directory)[i + 768] = ((int) &page_tables - VMA_OFFSET + i * 4096) | PG_WRITABLE | PG_PRESENT;

	/* do not need to flush a TLB */
}

void paging_init (void)
{
	paging_init_mapping ();
}
