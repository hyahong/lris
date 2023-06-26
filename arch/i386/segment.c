#ifdef i386
# include "cpu/interface.h"
#else
# error "this build target is not supported."
#endif

struct gdt_entry *gdt;

static void gdt_set_entry (uint16_t index, uint32_t base, uint32_t limit)
{
	struct gdt_entry *entry;

	entry = &gdt[index];

	entry->limit_low = limit & 0xFFFF;
	entry->limit_high = (limit >> 16) & 0xF;
	entry->base_low = base & 0xFFFFFF;
	entry->base_high = (base >> 24) & 0xFF;

	entry->access = 0;
	entry->flag = 0;
}

void segment_init (void)
{
	gdt = (struct gdt_entry *) GDT_BASE_ADDRESS;

	gdt_set_entry (0, 0, 0);

	gdt_set_entry (GDT_ENTRY_KERNEL_CS, 0x12345678, 0x42424);
}
