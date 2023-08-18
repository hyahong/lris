#ifndef _CPU_PAGE_H_
# define _CPU_PAGE_H_

# include "lris/types.h"

# define PHYSICAL_MEMORY_SIZE (4ULL * 1024 * 1024 * 1024)
# define PHYSICAL_FRAME_SIZE (4 * 1024)
# define PHYSICAL_FRAME_COUNT (PHYSICAL_MEMORY_SIZE / PHYSICAL_FRAME_SIZE)

# define PAGE_SIZE PHYSICAL_FRAME_SIZE
# define PAGE_SHIFT 12

# define KERNEL_MEMORY_SIZE (1ULL * 1024 * 1024 * 1024)
# define USER_MEMORY_SIZE (PHYSICAL_MEMORY_SIZE - KERNEL_MEMORY_SIZE)

# define KERNEL_PAGE_COUNT (KERNEL_MEMORY_SIZE / PAGE_SIZE)
# define USER_PAGE_COUNT (USER_MEMORY_SIZE / PAGE_SIZE)
# define PAGE_COUNT (KERNEL_PAGE_COUNT + USER_PAGE_COUNT)

# define VMA_OFFSET USER_MEMORY_SIZE
# define NORMAL_ZONE_PAGE_COUNT 229376

# define PAGE_DIRECTORY_COUNT 1024
# define PAGE_TABLE_COUNT 1024

/* P */
# define PG_PRESENT 1

/* RW */
# define PG_RDONLY 0 << 1
# define PG_WRITABLE 1 << 1

/* US */
# define PG_KERNEL 0 << 2
# define PG_USER 1 << 2

/* PWT */
# define PG_WRITEBACK 0 << 3
# define PG_WRITETHROUGH 1 << 3

/* PCD */
# define PG_CACHE 0 << 4
# define PG_NONECACHE 1 << 4

/* G */
# define PG_NOTGLOBAL 0 << 8
# define PG_GLOBAL 1 << 8

/* PS */
# define PG_PAGE 0 << 7
# define PG_BIGPAGE 1 << 7

/* struct page flags */
# define PG_reserved 1 << 0
# define PG_shared 1 << 1

/* paging directory */
typedef struct page_directory page_directory_t;
struct page_directory
{
	uint32_t present	: 1;
	uint32_t rw			: 1;
	uint32_t us			: 1;
	uint32_t pwt		: 1;
	uint32_t pcd		: 1;
	uint32_t a			: 1;
	uint32_t _ignored	: 1;
	uint32_t ps			: 1;
	uint32_t __ignored	: 4;
	uint32_t addr		: 20;
} __attribute__((packed));

/* paging table */
typedef struct page_table page_table_t;
struct page_table
{
	uint32_t present	: 1;
	uint32_t rw			: 1;
	uint32_t us			: 1;
	uint32_t pwt		: 1;
	uint32_t pcd		: 1;
	uint32_t a			: 1;
	uint32_t d			: 1;
	uint32_t pat		: 1;
	uint32_t g			: 1;
	uint32_t _ignored	: 3;
	uint32_t physical	: 20;
} __attribute__((packed));

/* mmpage */
/* whole memory mapping structure */
typedef uint32_t offset_t;
typedef struct pgtable_struct pgtable_t;
struct pgtable_struct
{
	page_directory_t *page_directory;
};

/* frame */
struct page
{
	uint8_t flags;
	uint8_t _refcount;

	void *virtual;

	/* used to ordered list in buddy */
	LIST_HEAD (chain);
} __attribute__((packed));

/* extern */
extern char _kernel_end;

extern char _init_page_directory;
extern char _init_page_tables;

void paging_init (void);

uint32_t get_frame_index (const struct page *page);
struct page *get_frame (int index);

void *get_physical_address (void *address);

struct page *get_reserved_page (void);

#endif
