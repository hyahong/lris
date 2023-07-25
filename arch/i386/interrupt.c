#include "cpu/segment.h"
#include "cpu/interrupt.h"

#include "lris/string.h"
#include "lris/printk.h"

idt_entry_t idt_entry[256];
idt_ptr_t idt_ptr;

static void interrupt_register (void)
{
	idt_ptr.base = (uint32_t) idt_entry;
	idt_ptr.limit = 32 * sizeof (idt_entry_t) - 1;

	asm volatile (
		".intel_syntax  \n\t"
		"mov %%eax, %0	\n\t"
		"lidt [%%eax]	\n\t"
		".att_syntax    \n\t"
		: /* output */
		: "r"(&idt_ptr)
	);
}

#include "lris/printk.h"
void irq14 (int error_code)
{
	int cr2;

	printk ("\nwhat... \n");

	asm volatile (
		".intel_syntax		\n\t"
		"mov %0, %%cr2		\n\t"
		".att_syntax		\n\t"
		: "=r"(cr2)
		:
		: "memory"
	);
	printk ("\n%x\n", cr2);

	asm volatile ("hlt");
}

void irq0 (void)
{
	printk ("\nsomething is wrong...\n");
	asm volatile ("hlt");
}

void interrupt_init (void)
{
	int i;

	memset (idt_entry, 0, sizeof (idt_entry_t) * 256);

	for (i = 0; i < 32; i++)
	{
		idt_entry[i].offset_low = ((uint32_t) irq0) & 0xFFFF;
		idt_entry[i].selector = SEG_SELECTOR_KERNEL_CODE;
		idt_entry[i].flags = 0b11101110;// 1110_1110;
		idt_entry[i].offset_high = ((uint32_t) irq0) >> 16;
	}
	idt_entry[14].offset_low = ((uint32_t) irq14) & 0xFFFF;
	idt_entry[14].selector = SEG_SELECTOR_KERNEL_CODE;
	idt_entry[14].flags = 0b11101110;// 1110_1110;
	idt_entry[14].offset_high = ((uint32_t) irq14) >> 16;

	interrupt_register ();

	klog ("Initialized interrupt routine");
}
