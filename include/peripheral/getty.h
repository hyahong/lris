#ifndef _GETTY_H_
# define _GETTY_H_

# include "lris/types.h"

# define GETTY_COMMAND_MAX 64

# define DUMP_WIDTH 16

typedef struct getty_struct getty_t;
struct getty_struct
{
	uint32_t offset;
	uint32_t size;
	char buffer[GETTY_COMMAND_MAX + 1];
};

void wrapper_hook (char character);

void getty_command (char *command[]);
void getty_parse (void);
void getty_init (void);

#endif
