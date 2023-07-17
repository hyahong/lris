#include "cpu/segment.h"
#include "cpu/page.h"

#define TEXT_SECTION __attribute__((section(".inittext")))
#define DATA_SECTION __attribute__((section(".initdata")))
#define ALIGN_ATTR __attribute__ ((aligned (4096)))

static page_directory_t DATA_SECTION temp_directory[1024] ALIGN_ATTR;
static page_table_t DATA_SECTION temp_table_low[1024] ALIGN_ATTR;
static page_table_t DATA_SECTION temp_table_high[1024 * 10] ALIGN_ATTR;

void static TEXT_SECTION paging_enable_temp (void)
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
		: "r"(temp_directory)
	);
}

void static TEXT_SECTION paging_init_temp (void)
{
	int i;

	for (i = 0; i < 1024; i++) /* 0G + 4MB*/
		((int *) temp_table_low)[i] = (i * PAGE_SIZE) | PG_WRITABLE | PG_PRESENT;
	for (i = 0; i < 1024 * 10; i++) /* 3G + 4MB */
		((int *) temp_table_high)[i] = (i * PAGE_SIZE) | PG_WRITABLE | PG_PRESENT;

	((int *) temp_directory)[0] = (uint32_t) temp_table_low | PG_WRITABLE | PG_PRESENT;
	for (i = 0; i < 10; i++)
		((int *) temp_directory)[i + 768] = ((uint32_t) temp_table_high + i * 1024) | PG_WRITABLE | PG_PRESENT;
}

void TEXT_SECTION main (void)
{
	segment_init ();

	paging_init_temp ();
	paging_enable_temp ();

	segment_paging_flush ();
}
