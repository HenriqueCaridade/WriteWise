#ifndef _PROJ_KEYBOARD_H
#define _PROJ_KEYBOARD_H

#define LEFT_SHIFT_SCANCODE     0x2A
#define LEFT_CTRL_SCANCODE      0x1D
#define LEFT_ALT_SCANCODE       0x38
#define SPACE_SCANCODE          0x39
#define RIGHT_SHIFT_SCANCODE    0x36
#define CAPSLOCK_SCANCODE       0X3A
#define BACKSPACE_SCANCODE      0x0E
#define ENTER_SCANCODE          0x1C
#define KEY_SCANCODE_1          0x02
#define KEY_SCANCODE_0          0x0B
#define KEY_SCANCODE_Q          0x10
#define KEY_SCANCODE_P          0x19
#define KEY_SCANCODE_A          0x1E
#define KEY_SCANCODE_L          0x26
#define KEY_SCANCODE_Z          0x2C
#define KEY_SCANCODE_M          0X32
#define KEY_SCANCODE_DASH       0x35

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
