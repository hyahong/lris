#ifndef _LIST_H_
# define _LIST_H_

# include "lris/types.h"

# define list_for_each(ptr, head) \
	for (ptr = (head)->next; ptr; ptr = ptr->next)

# define list_entry(ptr, type, member) container_of(ptr, type, member)

# define list_for_each_entry(ptr, head, member) \
	for (ptr = list_entry((head)->next, typeof (*ptr), member); &ptr->member; \
		ptr = list_entry(ptr->member.next, typeof (*ptr), member))

void list_add (struct list_head *new, struct list_head *head);
void list_del (struct list_head *entry);

#endif
