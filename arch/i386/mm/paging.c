#include "cpu/paging.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "lris/assert.h"

/* frames */
struct page mm_frames[PAGE_DIRECTORY_COUNT * PAGE_TABLE_COUNT];
/* kernel page maps */
pgtable_t mm_pgtable;

static uint32_t *paging_get_vm (void *p, offset_t off)
{
	return (uint32_t *) (p + off);
}

static void paging_pgtable_init (void)
{
	uint32_t *base;
	int i;

	mm_pgtable.offset.dir = VMA_OFFSET;
	mm_pgtable.offset.table = VMA_OFFSET;
	mm_pgtable.page_directory = (page_directory_t *) &_init_page_directory;
	/* clear the tables */
	/* maps each directory to tables */
	base = paging_get_vm (mm_pgtable.page_directory, mm_pgtable.offset.dir);
	for (i = 0; i < NORMAL_ZONE_PAGE_COUNT / 1024; i++)
		base[i + 768] = ((uint32_t) &_init_page_tables + i * 4 * PAGE_TABLE_COUNT) | PG_WRITABLE | PG_PRESENT;

	klog ("Initialized mapping %d pages for paging", NORMAL_ZONE_PAGE_COUNT);
}

static void paging_frame_init (void)
{
	int mapped;
	int i;

	/* used in kernel mapping */
	mapped = (((uint32_t) &_kernel_end - VMA_OFFSET) >> 12) + 1;
	for (i = 0; i < mapped; i++)
	{
		mm_frames[i].flags = PG_reserved;
		mm_frames[i]._refcount = 1;
		mm_frames[i].virtual = (void *) ((uint32_t) VMA_OFFSET + i * PAGE_SIZE);
		LIST_HEAD_INIT (mm_frames[i].chain);
	}
	klog ("%d frames are in use", i);
	/* clear unused frames state */
	for (; i < PAGE_DIRECTORY_COUNT * PAGE_TABLE_COUNT; i++)
		memset (&mm_frames[i], 0, sizeof (struct page));

	klog ("Initialized %d frames for paging", PAGE_DIRECTORY_COUNT * PAGE_TABLE_COUNT);
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
	paging_pgtable_init ();
	paging_frame_init ();
}
