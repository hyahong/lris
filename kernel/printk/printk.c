#include "vga.h"
#include "linux/stdarg.h"
#include "linux/string.h"
#include "linux/printk.h"

static void printk_info_init (struct printk_info *info, const char *format)
{
	info->format = format;
	info->size = PRINTK_BUFFER_SIZE;
	info->offset = 0;
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
	++info->format;
}

static void printk_argument_c (struct printk_info *info, char arg)
{
	printk_buffer_write (info, &arg, 1);
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

static void printk_argument_x (struct printk_info *info, unsigned int arg, uint8_t upper)
{
	char buffer[16];
	int length;
	int i;
	
	length = 0;
	do
	{
		buffer[length++] = (upper ? "0123456789ABCDEF" : "0123456789abcdef")[arg % 16];
	}
	while (arg /= 16);
	buffer[length++] = 'x';
	buffer[length++] = '0';

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

	return 0;
}
