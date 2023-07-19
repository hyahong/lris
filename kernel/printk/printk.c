#include "driver/vga.h"

#include "lris/assert.h"
#include "lris/stdarg.h"
#include "lris/string.h"
#include "lris/printk.h"

static void printk_info_init (struct printk_info *info, const char *format)
{
	int i;

	info->format = format;
	for (i = 0; i < PRINTK_BUFFER_SIZE + 1; i++)
		info->buffer[i] = 0;
	info->size = PRINTK_BUFFER_SIZE;
	info->offset = 0;
	/* flag */
	info->flags = 0;
	info->flag.fill = ' ';
	info->flag.width = 0;
}

static void printk_buffer_write (struct printk_info *info, char *str, int length)
{
	int i;

	/* buffer expansion logic is required */
	if (info->offset + length > info->size)
		return ;
	
	for (i = 0; i < length; i++)
		info->buffer[info->offset + i] = str[i];
	info->offset += length;
}

static void printk_parse_flags (struct printk_info *info)
{
	char *flags = "xXcsd";
	int i;

	i = 0;
	info->format++;
	/* set fill from ' ' to '0' */
	if (info->format[0] == '0')
	{
		info->flag.fill = '0';
		info->format++;
	}
	/* width flag */
	while (i < strlen (info->format) && !strchr (flags, info->format[i]))
		++i;
	if (i != 1)
	{
		info->flags |= PRINTK_FLAG_WIDTH;
		info->flag.width = atoi (info->format);
	}
	info->format += i;
}

static void printk_argument_x (struct printk_info *info, unsigned int arg, uint8_t upper)
{
	char buffer[32];
	int length;
	int i;
	
	length = 0;
	do
	{
		buffer[length++] = (upper ? "0123456789ABCDEF" : "0123456789abcdef")[arg % 16];
	}
	while (arg /= 16);
	/* fill with character by width */
	if (info->flags & PRINTK_FLAG_WIDTH)
	{
		if (info->flag.fill == ' ')
		{
			buffer[length++] = 'x';
			buffer[length++] = '0';
			while (length + 2 < info->flag.width)
				buffer[length++] = ' ';
		}
		else if (info->flag.fill == '0')
		{
			while (length + 2 < info->flag.width)
				buffer[length++] = '0';
			buffer[length++] = 'x';
			buffer[length++] = '0';
		}
	}
	else
	{
		buffer[length++] = 'x';
		buffer[length++] = '0';
	}
	/* reverse */
	for (i = 0; i < length; i++)
		printk_buffer_write (info, &buffer[length - i - 1], 1);
}

static void printk_argument_c (struct printk_info *info, char arg)
{
	printk_buffer_write (info, &arg, 1);
}

static void printk_argument_s (struct printk_info *info, char *arg)
{
	printk_buffer_write (info, arg, strlen (arg));
}

static void printk_argument_d (struct printk_info *info, int arg)
{
	uint8_t minus;
	char buffer[16];
	int length;
	int i;
	
	minus = 0;
	if (arg < 0)
	{
		minus = 1;
		arg *= -1;
	}
	length = 0;
	do
	{
		buffer[length++] = '0' + arg % 10;
	}
	while (arg /= 10);
	if (minus)
		buffer[length++] = '-';

	for (i = 0; i < length; i++)
		printk_buffer_write (info, &buffer[length - i - 1], 1);
}

static void printk_write_argument (struct printk_info *info, va_list *args)
{
	switch (*info->format)
	{
		case 'x':
		case 'X':
			printk_argument_x (info, va_arg (*args, unsigned int), *info->format == 'X');
			break;

		case 'c':
			printk_argument_c (info, va_arg (*args, char));
			break;

		case 's':
			printk_argument_s (info, va_arg (*args, char *));
			break;

		case 'd':
			printk_argument_d (info, va_arg (*args, int));
			break;

		default:
			break;
	}
	++info->format;
}

int printk (const char *format, ...)
{
	struct printk_info info;
	va_list args;
	int i;

	va_start (args, format);
	printk_info_init (&info, format);

	while (*info.format)
	{
		if (*info.format == '%')
		{
			printk_parse_flags (&info);
			printk_write_argument (&info, &args);
		}
		else
		{
			printk_buffer_write (&info, (char *) info.format, 1);
			info.format++;
		}
	}

	va_end (args);

	for (i = 0; i < info.offset; i++)
		vga_draw (info.buffer[i], 1);
	vga_set_cursor ();

	return info.offset;
}

int vprintk (const char *format, va_list args)
{
	struct printk_info info;
	int i;

	printk_info_init (&info, format);

	while (*info.format)
	{
		if (*info.format == '%')
		{
			printk_parse_flags (&info);
			printk_write_argument (&info, &args);
		}
		else
		{
			printk_buffer_write (&info, (char *) info.format, 1);
			info.format++;
		}
	}

	for (i = 0; i < info.offset; i++)
		vga_draw (info.buffer[i], 1);
	vga_set_cursor ();

	return info.offset;
}

void klog (const char *format, ...)
{
	va_list args;

	va_start (args, format);

	printk ("[timestamp] ");
	vprintk (format, args);
	printk ("\n");
	
	va_end (args);
}
