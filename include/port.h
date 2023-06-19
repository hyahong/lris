#ifndef _PORT_H_
# define _PORT_H_

void outb (uint16_t port, uint8_t val);
uint8_t inb (uint16_t port);

#endif
