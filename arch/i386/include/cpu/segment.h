#ifndef _CPU_SEGMENT_H_
# define _CPU_SEGMENT_H_

# include "lris/types.h"

# define GDT_BASE_ADDRESS 0xC0000800

# define GDT_ENTRY_KERNEL_CS	1
# define GDT_ENTRY_KERNEL_DS	2
# define GDT_ENTRY_KERNEL_SS	3

# define GDT_ENTRY_USER_CS		4
# define GDT_ENTRY_USER_DS		5
# define GDT_ENTRY_USER_SS		6

# define SEGMENT_PRESENT(x) ((x) << 7)
# define SEGMENT_PRIVILEGE(x) ((x) << 5)
# define SEGMENT_TYPE(x) ((x) << 4)
# define SEGMENT_EXEC(x) ((x) << 3)
# define SEGMENT_DIRECTION(x) ((x) << 2)
# define SEGMENT_CONFORM(x) ((x) << 2)
# define SEGMENT_RW(x) ((x) << 1)

# define SEGMENT_GRANULARITY(x) ((x) << 3)
# define SEGMENT_SIZE(x) ((x) << 2)
# define SEGMENT_LONG(x) ((x) << 1)

# define SEG_SELECTOR_INDEX(x) ((x) << 3)
# define SEG_SELECTOR_INDICATOR(x) ((x) << 2)
# define SEG_SELECTOR_RPL(x) (x)

/*
 * P DPL S E DC RW A
 */
#define GDT_KERNEL_CODE_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(0) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(1) | SEGMENT_CONFORM(0) | \
								SEGMENT_RW(1)

#define GDT_KERNEL_DATA_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(0) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(0) | SEGMENT_DIRECTION(0) | \
								SEGMENT_RW(1)

#define GDT_KERNEL_STACK_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(0) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(0) | SEGMENT_DIRECTION(1) | \
								SEGMENT_RW(1)

#define GDT_USER_CODE_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(3) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(1) | SEGMENT_CONFORM(1) | \
								SEGMENT_RW(1)

#define GDT_USER_DATA_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(3) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(0) | SEGMENT_DIRECTION(0) | \
								SEGMENT_RW(1)

#define GDT_USER_STACK_ACCESS	SEGMENT_PRESENT(1) | SEGMENT_PRIVILEGE(3) | \
								SEGMENT_TYPE(1) | SEGMENT_EXEC(0) | SEGMENT_DIRECTION(1) | \
								SEGMENT_RW(1)

/*
 * G DB L Reserved
 */
#define GDT_KERNEL_FLAG			SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1) | \
								SEGMENT_LONG(0)

#define GDT_USER_FLAG			SEGMENT_GRANULARITY(1) | SEGMENT_SIZE(1) | \
								SEGMENT_LONG(0)

/*
 * INDEX TI RPL 
 */
#define SEG_SELECTOR_KERNEL_CODE	SEG_SELECTOR_INDEX(GDT_ENTRY_KERNEL_CS) | \
									SEG_SELECTOR_INDICATOR(0) | SEG_SELECTOR_RPL(0)

#define SEG_SELECTOR_KERNEL_DATA	SEG_SELECTOR_INDEX(GDT_ENTRY_KERNEL_DS) | \
									SEG_SELECTOR_INDICATOR(0) | SEG_SELECTOR_RPL(0)

#define SEG_SELECTOR_KERNEL_STACK	SEG_SELECTOR_INDEX(GDT_ENTRY_KERNEL_SS) | \
									SEG_SELECTOR_INDICATOR(0) | SEG_SELECTOR_RPL(0)

/* segmentation */
typedef struct gdt_entry_struct gdt_entry_t;
struct gdt_entry_struct
{
	uint32_t limit_low	:16;
	uint32_t base_low	:24;
	uint32_t access		:8;
	uint32_t limit_high	:4;
	uint32_t flag		:4;
	uint32_t base_high	:8;
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;
struct gdt_ptr_struct
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void segment_init (void);

#endif
