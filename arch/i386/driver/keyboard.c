#include "port.h"
#include "driver/keyboard.h"

#include "lris/printk.h"
#include "peripheral/wrapper.h"

struct key_state k_state;

void keyboard_init (void)
{
	int i;

	for (i = 0; i < KEYBOARD_KEYMAP_MAX; i++)
		k_state.key_maps[i] = KEY_NULL; 
	k_state.shift = 0;
	k_state.ledstate = 0;

	klog ("Initialized keyboard setting");
}

static void keyboard_press (uint32_t code)
{
	k_state.key_maps[code] = KEY_PRESS;

	if (code == KEY_CAPSLOCK)
		k_state.ledstate = k_state.ledstate ^ KEY_LED_CAPSLOCK;
	/* press handler */
	keyboard_wrapper (code);
}

static void keyboard_release (uint32_t code)
{
	k_state.key_maps[code] = KEY_NULL;

	/* release handler */
}

static void keyboard_shortcut (void)
{
}

void keyboard_handler (void)
{
	uint32_t code;

	if (!(inb (IO_STATUS_REGISTER) & STATUS_REGISTER_INPUT))
		return ;

	code = inb (IO_DATA_PORT);
	
	/*
	 * keyboard event hook
	 */
	if (code < KEY_RELEASE_OFFSET)
		keyboard_press (code);
	else
		keyboard_release (code - KEY_RELEASE_OFFSET);
	
	/* special */
	k_state.shift = k_state.key_maps[KEY_LEFTSHIFT] | k_state.key_maps[KEY_RIGHTSHIFT];
}
