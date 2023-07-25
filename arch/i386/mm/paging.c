#include "cpu/paging.h"

#include "lris/string.h"
#include "lris/printk.h"
#include "lris/assert.h"

/* frames */
static struct page mm_frames[PAGE_DIRECTORY_COUNT * PAGE_TABLE_COUNT];
/* kernel page maps */
static pgtable_t kernel_pgtable;
/* current page table */
static pgtable_t *mm_pgtable;

static void paging_pgtable_init (void)
{
	uint32_t *base;
	int i;

	/* set the initial page table */
	mm_pgtable = &kernel_pgtable;
	/* init */
	mm_pgtable->page_directory = (page_directory_t *) ((uint32_t) &_init_page_directory + VMA_OFFSET);
	/* clear the tables */
	/* maps each directory to tables */
	base = (uint32_t *) mm_pgtable->page_directory;
	for (i = 0; i < NORMAL_ZONE_PAGE_COUNT / 1024; i++)
		base[i + 768] = ((uint32_t) &_init_page_tables + i * 4 * PAGE_TABLE_COUNT) | PG_WRITABLE | PG_PRESENT;

	klog ("Initialized mapping %d pages for paging", NORMAL_ZONE_PAGE_COUNT);
}

static void paging_frame_init (void)
{
	int mapped;
	int i;

	/* used in kernel mapping */
	mapped = (((uint32_t) &_kernel_end - VMA_OFFSET) >> PAGE_SHIFT) + 1;
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

uint32_t get_frame_index (const struct page *page)
{
	return (uint32_t) (page - mm_frames);
}

struct page *get_frame (int a)
{
	return &mm_frames[a];
}

/* this function shouldn't be called for virtual address 0xC0000000 */
void *get_physical_address (void *address)
{
	uint32_t dir_offset, table_offset;
	uint32_t *dir_base, *table_base;

	dir_offset = ((uint32_t) address) >> (PAGE_SHIFT + 10);
	table_offset = (((uint32_t) address) >> PAGE_SHIFT) & 0x3FF;

	/* directory */
	dir_base = (uint32_t *) &mm_pgtable->page_directory[dir_offset];
	if (!*dir_base)
		return 0;

	/* table */
	table_base = (uint32_t *) (uint32_t) ((*dir_base & 0xFFFFF000) + VMA_OFFSET);
	
	return (void *) table_base[table_offset];
}

struct page *set_page_address (struct page *page, void *address)
{
	uint32_t dir_offset, table_offset;
	uint32_t *dir_base, *table_base;

	if (page->flags && !(page->flags & PG_shared))
		/* already mapped and unable to remap */
		panic ("page is mapped redundantly");

	page->virtual = address;

	dir_offset = ((uint32_t) address) >> (PAGE_SHIFT + 10);
	table_offset = (((uint32_t) address) >> PAGE_SHIFT) & 0x3FF;

	/* directory */
	dir_base = (uint32_t *) &mm_pgtable->page_directory[dir_offset];
	if (!*dir_base)
	{
		/* need table creation logic */
		/* mapping */
		panic ("wrong in this point");
	}

	/* table */
	table_base = (uint32_t *) (uint32_t) ((*dir_base & 0xFFFFF000) + VMA_OFFSET);
	table_base[table_offset] = get_frame_index (page) * PAGE_SIZE | PG_WRITABLE | PG_PRESENT;

	return page;
}

struct page *get_reserved_page (void)
{
	static int index = 0;

	/* get the reserved pages in normal zone */
	for (; index < NORMAL_ZONE_PAGE_COUNT; index++)
	{
		if (mm_frames[index]._refcount == 0)
		{
			set_page_address (&mm_frames[index], (void *) VMA_OFFSET + PAGE_SIZE * index);
			mm_frames[index]._refcount = 1;

			return &mm_frames[index++];
		}
	}
	return NULL;
}

void paging_init (void)
{
	paging_pgtable_init ();
	paging_frame_init ();
}
