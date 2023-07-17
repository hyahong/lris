#include "lris/types.h"
#include "lris/string.h"

unsigned int strlen (const char *str)
{
	unsigned int length;

	length = 0;
	while (*(str + length))
		++length;

	return length;
}

char *strncpy (char *dest, const char *origin, size_t n)
{
	int i;

	for (i = 0; i < n; i++)
		dest[i] = origin[i];
	return dest;
}

int strcmp (const char *str1, const char *str2)
{
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

int strncmp (const char *str1, const char *str2, uint32_t length)
{
	uint32_t i;

	i = 0;
	while (*str1 && *str2 && *str1 == *str2 && i < length - 1)
	{
		str1++;
		str2++;
		i++;
	}

	return *str1 - *str2;
}

char *strchr (const char *str, int c)
{
	while (*str)
	{
		if (*str == c)
			return (char *) str;
		str++;
	}
	return (char *) -1;
}

static int atoi_hex (const char *str)
{
	char *hex_lower = "0123456789abcdef";
	char *hex_upper = "0123456789ABCDEF";
	char *hex, *offset;
	int result;
	int i, j;
	
	result = 0;
	for (i = 0; (int) strchr (hex_lower, str[i]) >= 0 || (int) strchr (hex_upper, str[i]) >= 0; i++)
	{
		/* raise position */
		result *= 16;
		/* get digit */
		hex = hex_lower;
		offset = strchr (hex, str[i]);
		if ((int) offset < 0)
		{
			hex = hex_upper;
			offset = strchr (hex, str[i]);
		}
		result += offset - hex;
	}
	return result;
}

static int atoi_dec (const char *str)
{
	int result;
	int minus, i;

	minus = 0;
	if (*str == '-')
	{
		minus = 1;
		str++;
	}
	result = 0;
	for (i = 0; (int) strchr ("0123456789", str[i]) >= 0; i++)
	{
		result *= 10;
		result += str[i] - '0';
	}
	if (minus)
		result *= -1;

	return result;
}

int atoi (const char *str)
{
	/* hexadecimal */
	if (!strncmp (str, "0x", 2))
		return atoi_hex (str + 2);
	/* octal */
	else if (!strcmp (str, "0"))
	{
	}
	/* decimal */
	else
		return atoi_dec (str);

	return 0;
}

void *memset (void *ptr, int value, size_t num)
{
	int bound;
	int i;

	bound = num / 4;
	for (i = 0; i < bound; i++)
		((int *) ptr)[i] = value;
	for (i = bound; i < num - bound * 4; i++)
	{
		((char *) ptr)[i] = value & 0xFF;
		value >>= 8;
	}

	return ptr;
}
