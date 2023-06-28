#ifndef _STRING_H_
# define _STRING_H_

unsigned int strlen (const char *str);
int strcmp (const char *str1, const char *str2);
int strncmp (const char *str1, const char *str2, uint32_t length);
char *strchr (const char *str, int c);
int atoi (const char *str);

#endif
