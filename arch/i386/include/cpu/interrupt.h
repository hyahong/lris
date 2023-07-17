#ifndef _CPU_INTERRUPT_
# define _CPU_INTERRUPT_

typedef struct idt_entry_struct idt_entry_t;
struct idt_entry_struct
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t _unused;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;
struct idt_ptr_struct
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void interrupt_init (void);

#endif
