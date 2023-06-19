#ifndef _PORT_H_
# define _PORT_H_

# include "../linux/types.h"

static inline void outb (uint16_t port, uint8_t val)
{
    asm volatile ("outb %0, %1"
					:
					: "a" (val), "Nd" (port)
					: "memory");
}

static inline uint8_t inb (uint16_t port)
{
    uint8_t val;

    asm volatile ("inb %1, %0"
                   : "=a" (val)
                   : "Nd" (port)
                   : "memory");
    return val;
}

#endif
