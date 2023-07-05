#include "cpu/interface.h"

#include "lris/printk.h"

gdt_entry_t *gdt;
gdt_ptr_t gdt_ptr;

static void gdt_set_entry (uint16_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flag)
{
	gdt_entry_t *entry;

	entry = &gdt[index];

	entry->limit_low = limit & 0xFFFF;
	entry->limit_high = (limit >> 16) & 0xF;
	entry->base_low = base & 0xFFFFFF;
	entry->base_high = (base >> 24) & 0xFF;

	entry->access = access;
	entry->flag = flag;
}

static void segment_flush (void)
{
	gdt_ptr.base = (uint32_t) gdt;
	gdt_ptr.limit = 7 * sizeof (gdt_entry_t) - 1;

	asm volatile (
		".intel_syntax	\n\t"
		"mov %%eax, %0	\n\t"
		"lgdt [%%eax]	\n\t"
		"mov %%eax, %1	\n\t"
		"mov %%ds, %%ax	\n\t"
		"mov %%eax, %2	\n\t"
		"mov %%ss, %%ax	\n\t"
		"jmp 0x08:.flush\n\t"
		".flush:		\n\t"
		".att_syntax	\n\t"
		: /* output */
		: "r"(&gdt_ptr),
		  "r"(SEG_SELECTOR_KERNEL_DATA),
		  "r"(SEG_SELECTOR_KERNEL_STACK)
	);	
}
extern void gdt_flush (uint32_t ptr);
void segment_init (void)
{
	gdt = (gdt_entry_t *) GDT_BASE_ADDRESS;

	/*
	 * Null Segment
	 */
	gdt_set_entry (0, 0, 0, 0, 0);

	/*
	 * Kernel Segment
	 */
	gdt_set_entry (GDT_ENTRY_KERNEL_CS, 0x0, 0xFFFFF, GDT_KERNEL_CODE_ACCESS, GDT_KERNEL_FLAG);
	gdt_set_entry (GDT_ENTRY_KERNEL_DS, 0x0, 0xFFFFF, GDT_KERNEL_DATA_ACCESS, GDT_KERNEL_FLAG);
	gdt_set_entry (GDT_ENTRY_KERNEL_SS, 0x0, 0xFFFFF, GDT_KERNEL_STACK_ACCESS, GDT_KERNEL_FLAG);

	/*
	 * User Segment
	 */
	gdt_set_entry (GDT_ENTRY_USER_CS, 0x0, 0xFFFFF, GDT_USER_CODE_ACCESS, GDT_USER_FLAG);
	gdt_set_entry (GDT_ENTRY_USER_DS, 0x0, 0xFFFFF, GDT_USER_DATA_ACCESS, GDT_USER_FLAG);
	gdt_set_entry (GDT_ENTRY_USER_SS, 0x0, 0xFFFFF, GDT_USER_STACK_ACCESS, GDT_USER_FLAG);

	/*
	 * Load Segment
	 */
	segment_flush ();
}
