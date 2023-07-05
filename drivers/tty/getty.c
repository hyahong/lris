#include "port.h"
#include "vga.h"

#include "peripheral/getty.h"
#include "lris/string.h"
#include "lris/printk.h"

getty_t sys_getty;

void wrapper_hook (char character)
{
	char filter[] = { 0x9 };
	int i;

	/* throw */
	if (character == '\n')
	{
		vga_draw (character, 1);
		vga_set_cursor ();
		getty_parse ();
		return ;
	}
	
	/* ignore */
	if (sys_getty.offset == sys_getty.size)
		return ;
	for (i = 0; i < 1; i++)
		if (character == filter[i])
			return ;

	/* if input is a backspace */
	if (character == 0x8)
	{
		if (sys_getty.offset > 0)
		{
			sys_getty.buffer[--sys_getty.offset] = NULL;
			vga_backspace ();
			vga_set_cursor ();
		}
		return ;
	}
	/* if input is not a backspace */
	sys_getty.buffer[sys_getty.offset++] = character;
	
	vga_draw (character, 1);
	vga_set_cursor ();
}

/*
 * command execute area
 */
void getty_pdump (char *address)
{
	const unsigned char *addr;
	char *hex = "0123456789ABCDEF";
	int quotient, remainder;
	int i, j;

	addr = (const unsigned char *) atoi (address);
	/* format */
	printk ("P:Address    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	for (i = 0; i < 8; i++)
	{
		/* field */
		printk ("%010x:  ", (unsigned int) &addr[i * DUMP_WIDTH]);
		/* hex */
		for (j = 0; j < DUMP_WIDTH; j++)
		{
			remainder = addr[i * DUMP_WIDTH + j] % 16;
			quotient = addr[i * DUMP_WIDTH + j] / 16;
			vga_draw (hex[quotient], 1);
			vga_draw (hex[remainder], 1);
			vga_draw (' ', 1);
		}
		/* margin */
		vga_draw (' ', 1);
		vga_draw (' ', 1);
		/* ascii */
		for (j = 0; j < DUMP_WIDTH; j++)
		{
			if ((char) addr[i * DUMP_WIDTH + j] < 0x20 ||
					(char) addr[i * DUMP_WIDTH + j] == 0x7F)
				vga_draw ('.', 1);
			else
				vga_draw (addr[i * DUMP_WIDTH + j], 1);
		}
		/* draw */
		vga_draw ('\n', 1);
	}
	vga_draw ('\n', 1);
	vga_set_cursor ();
}

void getty_command (char *command[])
{
	if (!strcmp (command[0], "dump"))
	{
		getty_pdump (command[1]);
	}
	else if (!strcmp (command[0], "shutdown"))
	{
		outw (0x604, 0x2000);
	}
}

void getty_parse (void)
{
	char *command[8] = { 0, };
	char *cursor;
	int i, j;
	
	/*
	 * trim
	 */
	/* count the number of space in front of */
	for (i = 0; i < sys_getty.offset; i++)
		if (sys_getty.buffer[i] != ' ')
			break;
	/* there is no meaningful command */
	if (i == sys_getty.offset)
		goto end;
	for (j = 0; j < sys_getty.offset; j++)
		sys_getty.buffer[j] = sys_getty.buffer[j + i];
	for (i = sys_getty.offset - 1; i >= 0; i--)
	{
		if (sys_getty.buffer[i] != ' ')
		{
			sys_getty.buffer[i + 1] = NULL;
			break;
		}
	}

	/* command parse */
	j = 0;
	cursor = sys_getty.buffer;
	while (*cursor)
	{
		for (i = 0; i < strlen (cursor); i++)
			if (cursor[i] == ' ')
				break;
		cursor[i] = NULL;
		/* store the splited command */
		if (strlen (cursor))
			command[j++] = cursor;
		cursor = cursor + i + 1;
	}
	/* execute the command */
	getty_command (command);

end:
	/* flush the buffer */
	sys_getty.offset = 0;
	sys_getty.size = GETTY_COMMAND_MAX;
	for (i = 0; i < GETTY_COMMAND_MAX + 1; i++)
		sys_getty.buffer[i] = NULL;
	/* TODO: require modification */
	printk ("$> ");
}

void getty_init (void)
{
	int i;

	sys_getty.offset = 0;
	sys_getty.size = GETTY_COMMAND_MAX;
	for (i = 0; i < GETTY_COMMAND_MAX + 1; i++)
		sys_getty.buffer[i] = NULL;
	/* TODO: require modification */
	printk ("$> ");
}

