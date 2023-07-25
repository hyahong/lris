#ifndef _ALLOCATOR_H_
# define _ALLOCATOR_H_

struct zone;

typedef enum gfp_enum gfp_t;
enum gfp_enum
{
	GFP_KERNEL = 1
};

struct page *alloc_page (struct zone *zone, int num);
void *free_page (struct zone *zone, struct page *page);

void *kmalloc (size_t size, gfp_t flags);
void kfree (const void *ptr);

#endif
