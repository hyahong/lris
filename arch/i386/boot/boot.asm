; signature
MMAGIC equ 0x1BADB002
MFLAGS equ 0x0
CHECKSUM equ -(MMAGIC + MFLAGS)

; multiboot header
section .header
align 4
	dd MMAGIC
	dd MFLAGS
	dd CHECKSUM

; initdata section
section .initdata
	times 256 db 0
physical_stack:

; inittext section
section .inittext
global start
extern main 

start:
	cli
	; call stack
	mov esp, physical_stack 
	; entering into memory init
	call main
	; jump to virtual
	jmp clean_start

	; something is wrong
	hlt

; bss section
section .bss
align 16		; 16 bytes alignment
	resb 16384	; 16 KiB
virtual_stack:

; text section
section .text
global clean_start
extern kernel_init

clean_start:
	; virtual address stack
	mov esp, virtual_stack
	; main kernel
	call kernel_init

	; if a flow exits the kernel
	hlt
