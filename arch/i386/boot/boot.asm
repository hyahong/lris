; signature
MMAGIC equ 0x1BADB002
MFLAGS equ 0x0
CHECKSUM equ -(MMAGIC + MFLAGS)

; multiboot header
section .header
align 4			; 4 bytes alignment
	dd MMAGIC
	dd MFLAGS
	dd CHECKSUM

; bss section
section .bss
align 16		; 16 bytes alignment
	resb 16384	; 16 KiB
stack_top:

; text section
section .text
global start		; export this section
extern kernel_init

start:
	mov esp, stack_top
	
	call kernel_init

	cli		; clear an interrupt flag

	hlt		; stop
