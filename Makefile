TARGET		= i386

ASM			= nasm
CC			= gcc
CFLAGS		= -m32 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

LD			= ld
LDFLAGS		= -m elf_i386

RM			= rm -rf

ARCH		= arch/$(TARGET)/boot/boot.asm arch/$(TARGET)/segment.c arch/$(TARGET)/vga.c \
			  arch/$(TARGET)/driver/keyboard.c

DRIVERS		= drivers/tty/wrapper.c drivers/tty/getty.c
INIT		= init/main.c
KERNEL		= kernel/printk/printk.c
LIB			= lib/string.c

SRCS		= $(ARCH) $(DRIVERS) $(INIT) $(KERNEL) $(LIB)
INCS		= -Iinclude -Iarch/$(TARGET)/include

OBJS		= $(patsubst %.asm,%.o,$(SRCS)) $(patsubst %.c,%.o,$(SRCS))
OBJS		:= $(filter %.o,$(OBJS))

BIN			= kfs
ISO			= kfs.iso

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
	@$(RM) $(BIN) iso

run:
	@qemu-system-i386 -cdrom $(ISO)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN) $(ISO)

re: fclean all
