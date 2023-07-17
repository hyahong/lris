#include "mm/mmzone.h"

/* the offset 'USER_PAGE_COUNT' is added when using zone */
mm_zone_t mem_zone[ZONE_COUNT] = {
	{ "DMA", 0 /* 0 MB */, 4096 /* 16 MB */ },
	{ "NORMAL", 4096 /* 16 MB */, 229376 /* 896 MB */ },
	{ "HIGH", 229376 /* 896 MB */, 262144 /* 1024 MB */ }
};

void mm_zone_init (void)
{
}
