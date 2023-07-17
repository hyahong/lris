/*
 * To perform memory initialization first, this code is located in
 * .inittext SECTION and .initdata SECTION.
 */

#include "cpu/page.h"

#include "lris/string.h"
#include "lris/assert.h"

#define TEXT_SECTION __attribute__((section(".inittext")))
#define DATA_SECTION __attribute__((section(".initdata")))

/* directory and table must be aligned for 4K */
uint32_t page_directory[1024]
__attribute__ ((aligned (4096)));

page_table_t page_table0[1024]
__attribute__ ((aligned (4096)));

page_table_t page_table1[1024]
__attribute__ ((aligned (4096)));

static void page_table_init (void)
{
	page_table_t *pg_tlb;
	page_directory_t *pg_dir;
	int zone_start, zone_end;
	int i;

	/* mpp zone of DMA, NORMAL of kernel to virtual */
	zone_start = 0;
	zone_end = 1 * 1024;
	
	/* kernel */
	/*
	for (i = 0; i < 256 * 2; i++)
	{
		pg_tlb = &page_table0[i];
		pg_tlb->_ignored = 0;

		pg_tlb->p = PG_PRESENT;
		pg_tlb->rw = PG_RDWR;
		pg_tlb->us = PG_USER;
		pg_tlb->pwt = PG_WRITEBACK;
		pg_tlb->pcd = PG_CACHE;
		pg_tlb->a = 0;
		pg_tlb->d = 0;
		pg_tlb->pat = 0;
		pg_tlb->g = PG_NOTGLOBAL;
		pg_tlb->physical = i;
	}
	for (i = 0; i < 256 * 3; i++)
	{//786432
		pg_tlb = &page_table1[i];
		pg_tlb->_ignored = 0;

		pg_tlb->p = PG_PRESENT;
		pg_tlb->rw = PG_RDWR;
		pg_tlb->us = PG_USER;
		pg_tlb->pwt = PG_WRITEBACK;
		pg_tlb->pcd = PG_CACHE;
		pg_tlb->a = 0;
		pg_tlb->d = 0;
		pg_tlb->pat = 0;
		pg_tlb->g = PG_NOTGLOBAL;
		pg_tlb->physical = 256 + i;
	}
	*/

	/* give the address of each table to directory list */
	/* kernel */
	page_directory[0] = ((uint32_t) page_table0) | 0x7;
	page_directory[768] = ((uint32_t) page_table1) | 0x7;	
}

static void page_enable (void)
{
	asm volatile (
		".intel_syntax			\n\t"
		/* page directory address to cr3 */
		"mov %%cr3, %0			\n\t"
		/* paging enable */
		"mov %%eax, %%cr0		\n\t"
		"or %%eax, 0x80000000	\n\t"
		"mov %%cr0, %%eax		\n\t"
		".att_syntax			\n\t"
		: /* output */
		: "r"(page_directory)
	);
}

void page_init (void)
{
	mm_zone_init ();
	page_table_init ();
	page_enable ();
}
