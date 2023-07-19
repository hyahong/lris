OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(start)

SECTIONS
{
	/* virtual memory address offset (higher half) */
	VMA_OFFSET = 0xC0000000;
	/* conventional address for kernel */
	. = 0x100000;
	
	.header				: { *(.header) }
	.inittext			: { *(.inittext) }
	.initdata			: { *(.initdata) }

	/* to map a kernel virtual address, uses 0xC0000000 */
	. += VMA_OFFSET;

	.text ALIGN(4K)		: AT (ADDR (.text) - VMA_OFFSET)
	{
		*(.text)
	}
	
	.rodata ALIGN(4K)	: AT (ADDR (.rodata) - VMA_OFFSET)
	{
		*(.rodata)
	}

	.data ALIGN(4K)		: AT (ADDR (.data) - VMA_OFFSET)
	{
		*(.data)
	}

	.bss ALIGN(4K)		: AT (ADDR (.bss) - VMA_OFFSET)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}
