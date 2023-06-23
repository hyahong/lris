#ifndef _KEYBOARD_H_
# define _KEYBOARD_H_

# include "linux/key-code.h"

# define KEYBOARD_KEYMAP_MAX 128
# define KEY_NULL 0
# define KEY_PRESS 1

# define STATUS_REGISTER_INPUT 1

# define KEY_RELEASE_OFFSET 0x80

# define KEY_LED_SCROLLLOCK 0x1
# define KEY_LED_NUMBERLOCK 0x2
# define KEY_LED_CAPSLOCK 0x4

struct key_state
{
	uint8_t key_maps[KEYBOARD_KEYMAP_MAX];
	int shift;
	int ledstate;
};

extern struct key_state k_state;

void keyboard_init (void);
void keyboard_handler (void);

#endif
