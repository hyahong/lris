#include "linux/string.h"

unsigned int strlen (char *str)
{
	unsigned int length;

	length = 0;
	while (*(str + length))
		++length;

	return length;
}
