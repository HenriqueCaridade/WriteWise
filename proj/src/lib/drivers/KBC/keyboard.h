#ifndef _PROJ_KEYBOARD_H
#define _PROJ_KEYBOARD_H

#include <stdbool.h>

#define BREAK_CODE              BIT(7)

#define BREAK_ESC               0x81
#define MAKE_ESC                0x01
#define LEFT_SHIFT_SCANCODE     0x2A
#define LEFT_CTRL_SCANCODE      0x1D
#define LEFT_ALT_SCANCODE       0x38
#define SPACE_SCANCODE          0x39
#define RIGHT_SHIFT_SCANCODE    0x36
#define CAPSLOCK_SCANCODE       0X3A

uint8_t irqSetKeyboard;
uint8_t scancode;
bool isScancodeTwoBytes;
bool isShiftPressed;
bool isCtrlPressed;
bool isAltPressed;
bool isCapsLockActive;

int (keyboard_subscribe_int)();
int (keyboard_unsubscribe_int)();
int (keyboard_ih)();
int (keyboard_restore)();

#endif // _PROJ_KEYBOARD_H
