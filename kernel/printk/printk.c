#include "linux/stdarg.h"

int printk (const char *format, ...)
{
	va_list args;

	va_start (args, format);
	
	va_end (args);

	return 0;
}
