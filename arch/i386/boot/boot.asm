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
	mov dword [page_table_low - VMA_OFFSET + ecx * 4], ebx
	mov dword [page_tables - VMA_OFFSET + ecx * 4], ebx
	; move offset
	add eax, PAGE_SIZE
	add ecx, 1

	cmp eax, PAGE_SIZE * 1024
	jne fill_page

fill_directory:
	; 0 ~ 4 MB
	mov ebx, page_table_low - VMA_OFFSET
	or ebx, 0x03
	mov dword [page_directory - VMA_OFFSET], ebx
	; 3 GB ~ 3 GB + 4 MB
	mov ebx, page_tables - VMA_OFFSET
	or ebx, 0x03
	mov dword [page_directory - VMA_OFFSET + 768 * 4], ebx

enable_paging:
	mov eax, page_directory - VMA_OFFSET
	mov cr3, eax
 
	mov eax, cr0
	or eax, 0x80010000
	mov cr0, eax

	ret


; bss section
section .bss

global page_directory
global page_tables

; stack
virtual_stack:
align 16
	resb STACK_SIZE
; page tables
align 4096
page_directory:
	resd 1024
page_table_low:
	resd 1024
; mapping up to normal zone (896 MB)
page_tables:
	resd 229376
; frames
mm_frames:
	resb PAGE_STRUCT_SIZE * PAGE_COUNT

; text section
section .text

global clean_start
extern kernel_init

clean_start:
	; unmap the identity mapping
	mov dword [page_directory], 0

	; flush a TLB
	mov eax, cr3
	mov cr3, eax

	; virtual address stack
	mov esp, virtual_stack + STACK_SIZE
	; main kernel
	call kernel_init

	; if a flow exits the kernel
	hlt
