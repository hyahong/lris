#ifndef _STDARG_H_
# define _STDARG_H_

typedef char * va_list;

#define __va_rounded_size(TYPE) \
	((sizeof (TYPE) <= 4) ? 4 : sizeof (TYPE))

#define va_start(AP, LAST) \
	(AP = ((char *) &LAST) + __va_rounded_size (LAST))

#define va_arg(AP, TYPE) \
	(AP += __va_rounded_size (TYPE), \
	 *((TYPE *) (AP - __va_rounded_size (TYPE))))

#define va_end(AP)

#endif
