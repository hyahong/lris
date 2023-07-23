#ifndef _TYPES_H_
# define _TYPES_H_

# define NULL 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef unsigned int size_t;
typedef int ssize_t;

# define offsetof(type, member) ((size_t) &((type *) 0)->member)
# define container_of(ptr, type, member) \
		((type *) ((char *) ptr - offsetof(type, member)))

/* list */
struct list_head
{
	struct list_head *next;
	struct list_head *prev;
};

# define LIST_HEAD(name) struct list_head name;
# define LIST_HEAD_INIT(head) (head.next = head.prev = NULL, head)

#endif
