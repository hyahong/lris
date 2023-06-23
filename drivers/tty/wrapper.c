#include "vga.h"
#include "linux/keyboard.h"
#include "linux/wrapper.h"

struct plain_field plain_maps[] = {
	[KEY_1] = {PLAIN_FLAG_SHIFT, '!', '1'},
	[KEY_2] = {PLAIN_FLAG_SHIFT, '@', '2'},
	[KEY_3] = {PLAIN_FLAG_SHIFT, '#', '3'},
	[KEY_4] = {PLAIN_FLAG_SHIFT, '$', '4'},
	[KEY_5] = {PLAIN_FLAG_SHIFT, '%', '5'},
	[KEY_6] = {PLAIN_FLAG_SHIFT, '^', '6'},
	[KEY_7] = {PLAIN_FLAG_SHIFT, '&', '7'},
	[KEY_8] = {PLAIN_FLAG_SHIFT, '*', '8'},
	[KEY_9] = {PLAIN_FLAG_SHIFT, '(', '9'},
	[KEY_0] = {PLAIN_FLAG_SHIFT, ')', '0'},
	[KEY_MINUS] = {PLAIN_FLAG_SHIFT, '_', '-'},
	[KEY_EQUAL] = {PLAIN_FLAG_SHIFT, '+', '='},
	
	[KEY_Q] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'Q', 'q'},
	[KEY_W] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'W', 'w'},
	[KEY_E] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'E', 'e'},
	[KEY_R] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'R', 'r'},
	[KEY_T] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'T', 't'},
	[KEY_Y] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'Y', 'y'},
	[KEY_U] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'U', 'u'},
	[KEY_I] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'I', 'i'},
	[KEY_O] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'O', 'o'},
	[KEY_P] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'P', 'p'},
		
	[KEY_LEFTBRACE] = {PLAIN_FLAG_SHIFT, '{', '['},
	[KEY_RIGHTBRACE] = {PLAIN_FLAG_SHIFT, '}', ']'},
	[KEY_ENTER] = {0, 0, '\n'},

	[KEY_A] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'A', 'a'},
	[KEY_S] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'S', 's'},
	[KEY_D] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'D', 'd'},
	[KEY_F] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'F', 'f'},
	[KEY_G] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'G', 'g'},
	[KEY_H] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'H', 'h'},
	[KEY_J] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'J', 'j'},
	[KEY_K] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'K', 'k'},
	[KEY_L] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'L', 'l'},

	[KEY_SEMICOLON] = {PLAIN_FLAG_SHIFT, ':', ';'},
	[KEY_APOSTROPHE] = {PLAIN_FLAG_SHIFT, '"', '\''},
	[KEY_GRAVE] = {PLAIN_FLAG_SHIFT, '~', '`'},
	[KEY_BACKSLASH] = {PLAIN_FLAG_SHIFT, '|', '\\'},

	[KEY_Z] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'Z', 'z'},
	[KEY_X] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'X', 'x'},
	[KEY_C] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'C', 'c'},
	[KEY_V] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'V', 'v'},
	[KEY_B] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'B', 'b'},
	[KEY_N] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'N', 'n'},
	[KEY_M] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, 'M', 'm'},

	[KEY_COMMA] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, '<', ','},
	[KEY_DOT] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, '>', '.'},
	[KEY_SLASH] = {PLAIN_FLAG_SHIFT | PLAIN_FLAG_CAPSLOCK, '?', '/'},
	[KEY_KPASTERISK] = {0, 0, '*'},
	[KEY_SPACE] = {0, 0, ' '},

	[KEY_KP7] = {0, 0, '7'},
	[KEY_KP8] = {0, 0, '8'},
	[KEY_KP9] = {0, 0, '9'},

	[KEY_KPMINUS] = {0, 0, '-'},

	[KEY_KP4] = {0, 0, '4'},
	[KEY_KP5] = {0, 0, '5'},
	[KEY_KP6] = {0, 0, '6'},

	[KEY_KPPLUS] = {0, 0, '+'},

	[KEY_KP1] = {0, 0, '1'},
	[KEY_KP2] = {0, 0, '2'},
	[KEY_KP3] = {0, 0, '3'},
	[KEY_KP0] = {0, 0, '0'},

	[KEY_KPDOT] = {0, 0, '.'},
};

void keyboard_wrapper (uint32_t code)
{
	struct plain_field *plain;
	uint8_t character;

	plain = &plain_maps[code];
	character = plain->lower;
	if (plain->upper && (
			(k_state.shift && (plain->flag & PLAIN_FLAG_SHIFT)) ||
			((k_state.ledstate & KEY_LED_CAPSLOCK) && (plain->flag & PLAIN_FLAG_CAPSLOCK))
		))
		character = plain->upper;
	if (!character)
		return ;
	vga_draw (character, 1);
	vga_set_cursor ();
}
