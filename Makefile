TARGET		= i386

ASM			= nasm
CC			= gcc
CFLAGS		= -m32 -fno-builtin -fno-exceptions \
			  -fno-stack-protector -nostdlib -nodefaultlibs \
			  -ffreestanding \
			  -g3

LD			= ld
LDFLAGS		= -m elf_i386

RM			= rm -rf

ARCH		= arch/$(TARGET)/boot/boot.asm \
			  arch/$(TARGET)/interrupt.c \
			  arch/$(TARGET)/mm/segment.c arch/$(TARGET)/mm/paging.c \
			  arch/$(TARGET)/driver/keyboard.c arch/$(TARGET)/driver/vga.c

UNITTEST	= unittest/main.c unittest/list.c

DRIVERS		= drivers/tty/wrapper.c drivers/tty/getty.c
INIT		= init/main.c
KERNEL		= kernel/printk/printk.c kernel/assert.c kernel/list.c
LIB			= lib/string.c
MM			= mm/memory.c mm/zone.c mm/allocator.c

SRCS		= $(ARCH) $(DRIVERS) $(INIT) $(KERNEL) $(LIB) $(MM) $(UNITTEST)
INCS		= -Iinclude -Iarch/$(TARGET)/include

OBJS		= $(patsubst %.asm,%.o,$(SRCS)) $(patsubst %.c,%.o,$(SRCS))
OBJS		:= $(filter %.o,$(OBJS))

BIN			= lris
ISO			= lris.iso

LINKER_CONF	= arch/$(TARGET)/boot/setup.ld

all: $(OBJS) $(BIN) pack run

%.o: %.asm
	@$(ASM) -f elf32 $< -o $@
	@echo "ASM\t" $@

%.o: %.c
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@echo "CC\t" $@

$(BIN):
	@$(LD) $(LDFLAGS) -T $(LINKER_CONF) -o $(BIN) $(OBJS)
	@echo "LD\t" $(OBJS)

pack:
	@grub-file --is-x86-multiboot $(BIN)
	@mkdir -p iso/boot/grub
	@cp $(BIN) iso/boot
	@cp grub.cfg iso/boot/grub
	@grub-mkrescue -o $(ISO) iso
#@$(RM) $(BIN) iso

run:
	@qemu-system-i386 -smp 1 -m 4G -cdrom $(ISO) -s 

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN) $(ISO)

re: fclean all
