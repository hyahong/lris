#include "linux/types.h"

void outb (uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1"
					:
					: "a" (val), "Nd" (port)
					: "memory");
}

uint8_t inb (uint16_t port)
{
    uint8_t val;

    asm volatile ("inb %1, %0"
                   : "=a" (val)
                   : "Nd" (port)
                   : "memory");
    return val;
}
