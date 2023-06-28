ASM			= nasm
CC			= gcc
CFLAGS		= -m32 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

LD			= ld

RM			= rm -rf

ARCH		= arch/i386/boot/boot.asm arch/i386/segment.c
DRIVERS		= drivers/tty/keyboard.c drivers/tty/wrapper.c drivers/tty/getty.c
INIT		= init/main.c
KERNEL		= kernel/printk/printk.c
LIB			= lib/vga.c lib/string.c

SRCS		= $(ARCH) $(DRIVERS) $(INIT) $(KERNEL) $(LIB)
INCS		= -Iinclude -Iarch/i386/include

OBJS		= $(patsubst %.asm,%.o,$(SRCS)) $(patsubst %.c,%.o,$(SRCS))
OBJS		:= $(filter %.o,$(OBJS))

BIN			= kfs
ISO			= kfs.iso

LINKER_CONF	= linker.ld

all: $(OBJS) $(BIN) pack run

%.o: %.asm
	@$(ASM) -f elf32 $< -o $@
	@echo "ASM\t" $@

%.o: %.c
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@echo "CC\t" $@

$(BIN):
	@$(LD) -m elf_i386 -T $(LINKER_CONF) -o $(BIN) $(OBJS)
	@echo "LD\t" $(OBJS)

pack:
	@grub-file --is-x86-multiboot $(BIN)
	@mkdir -p iso/boot/grub
	@cp $(BIN) iso/boot
	@cp grub.cfg iso/boot/grub
	@grub-mkrescue -o $(ISO) iso
	@$(RM) $(BIN) iso

run:
	@qemu-system-i386 -cdrom $(ISO)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN) $(ISO)

re: fclean all
