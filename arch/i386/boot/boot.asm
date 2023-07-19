; signature
MMAGIC equ 0x1BADB002
MFLAGS equ 0x3
CHECKSUM equ -(MMAGIC + MFLAGS)
; MACRO
VMA_OFFSET equ 0xC0000000
STACK_SIZE equ 16384

PAGE_SIZE equ 4096
PAGE_COUNT equ 1024 * 1024
KERNEL_NORMAL_PAGE_COUNT equ 229376
KERNEL_NORMAL_DIR_COUNT equ 224


; multiboot header
section .header
align 4
	dd MMAGIC
	dd MFLAGS
	dd CHECKSUM


; initdata section
section .initdata

global _init_page_directory
global _init_page_tables

; page tables
align 4096
_init_page_directory:
	times 1024 dd 0
_init_page_table_low:
	times 1024 dd 0
; mapping up to normal zone (896 MB)
_init_page_tables:
	times KERNEL_NORMAL_PAGE_COUNT dd 0


; inittext section
section .inittext
global start

start:
	cli
	; paging
	call boot_paging
	; jump to virtual
	jmp clean_start

	; something is wrong
	hlt

boot_paging:
	; physical offset
	; flags
	mov eax, 0x03
	; count
	mov ecx, 0

fill_page:
	; mapping
	; lower half kerenl mapping
	mov dword [_init_page_table_low + ecx * 4], eax
	; higher half kerenl mapping
	mov dword [_init_page_tables + ecx * 4], eax
	; move offset
	add eax, PAGE_SIZE
	add ecx, 1

	cmp ecx, 1024
	jne fill_page

fill_page_higher: 
	; mapping
	; higher half kerenl mapping
	mov dword [_init_page_tables + ecx * 4], eax
	; move offset
	add eax, PAGE_SIZE
	add ecx, 1

	cmp ecx, KERNEL_NORMAL_PAGE_COUNT
	jne fill_page_higher

fill_directory:
	mov ecx, 0
	; 0 ~ 4 MB
	mov eax, _init_page_table_low
	or eax, 0x03
	mov dword [_init_page_directory], eax
	; 3 GB ~ 3 GB + 4 MB
	mov eax, _init_page_tables
	or eax, 0x03
	mov dword [_init_page_directory + 768 * 4], eax

fill_directory_higher:
	; 3 GB + ecx * 4 MB ~ 3 GB + (ecx + 1) * 4 MB
	add eax, 4096
	add ecx, 1
	mov dword [_init_page_directory + (ecx + 768) * 4], eax
	
	cmp ecx, KERNEL_NORMAL_DIR_COUNT
	jne fill_directory_higher

enable_paging:
	mov eax, _init_page_directory
	mov cr3, eax
 
	mov eax, cr0
	or eax, 0x80010000
	mov cr0, eax

	ret


; bss section
section .bss

; stack
virtual_stack:
align 16
	resb STACK_SIZE


; text section
section .text

global clean_start
extern kernel_init

clean_start:
	; unmap the identity mapping
	mov dword [_init_page_directory + VMA_OFFSET], 0

	; flush a TLB
	mov eax, cr3
	mov cr3, eax

	; virtual address stack
	mov esp, virtual_stack + STACK_SIZE
	; main kernel
	call kernel_init

	; if a flow exits the kernel
	hlt
