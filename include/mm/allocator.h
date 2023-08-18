#ifndef _ALLOCATOR_H_
# define _ALLOCATOR_H_

# include "lris/types.h"

struct zone;

typedef enum gfp_enum gfp_t;
enum gfp_enum
{
	GFP_KERNEL = 1
};

struct page *alloc_pages (struct zone *zone, int num);
void *free_pages (struct zone *zone, struct page *page);

void *kmalloc (size_t size, gfp_t flags);
void kfree (const void *ptr);
size_t ksize (const void *ptr);

#endif
