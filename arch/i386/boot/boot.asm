; signature
MMAGIC equ 0x1BADB002
MFLAGS equ 0x3
CHECKSUM equ -(MMAGIC + MFLAGS)
; MACRO
STACK_SIZE equ 16384
VMA_OFFSET equ 0xC0000000
PAGE_STRUCT_SIZE equ 6
PAGE_SIZE equ 4096
PAGE_COUNT equ 1024 * 1024


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
	dd 1024
_init_page_table_low:
	dd 1024
; mapping up to normal zone (896 MB)
_init_page_tables:
	dd 229376


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
	mov eax, 0
	; count
	mov ecx, 0

fill_page:
	; mapping
	mov ebx, eax
	; flags
	or ebx, 0x03
	mov dword [_init_page_table_low + ecx * 4], ebx
	mov dword [_init_page_tables + ecx * 4], ebx
	; move offset
	add eax, PAGE_SIZE
	add ecx, 1

	cmp eax, PAGE_SIZE * 1024
	jne fill_page

fill_directory:
	; 0 ~ 4 MB
	mov ebx, _init_page_table_low
	or ebx, 0x03
	mov dword [_init_page_directory], ebx
	; 3 GB ~ 3 GB + 4 MB
	mov ebx, _init_page_tables
	or ebx, 0x03
	mov dword [_init_page_directory + 768 * 4], ebx

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
