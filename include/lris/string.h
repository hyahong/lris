#ifndef _STRING_H_
# define _STRING_H_

# include "lris/types.h"

unsigned int strlen (const char *str);
char *strcpy (char *dest, const char *origin);
char *strncpy (char *dest, const char *origin, size_t n);
int strcmp (const char *str1, const char *str2);
int strncmp (const char *str1, const char *str2, uint32_t length);
char *strchr (const char *str, int c);
int atoi (const char *str);

void *memset (void *ptr, int value, size_t num);

#endif
