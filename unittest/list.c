#include "unittest.h"

struct node
{
	char contents[15];
	int index;

	LIST_HEAD (chain);
};

struct node base[12];

static void add_and_trav ()
{
	LIST_HEAD (head);
	int i;

	LIST_HEAD_INIT (head);
	for (int i = 0; i < 12; i++)
	{
		strcpy (base[i].contents, "test message");
		base[i].index = i;
		LIST_HEAD_INIT (base[i].chain);
	}
	
	list_add (&base[11].chain, &head);
	list_add (&base[4].chain, &head);
	list_add (&base[6].chain, &head);
	list_add (&base[1].chain, &head);
	list_add (&base[10].chain, &head);

	struct list_head *_head;
	struct node *_node;

	list_for_each (_head, &head)
	{
		_node = list_entry (_head, struct node, chain);

		printk ("%d [%s]\n", _node->index, _node->contents);
	}

	list_for_each_entry (_node, &head, chain)
	{
		printk ("%d [%s]\n", _node->index, _node->contents);
	}

	list_del (&base[6].chain);

	list_for_each_entry (_node, &head, chain)
	{
		printk ("%d [%s]\n", _node->index, _node->contents);
	}
}

void unittest_list ()
{
	printk ("> list\n");

	add_and_trav ();
}
