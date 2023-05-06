#ifndef _PROJ_KEYBOARD_H
#define _PROJ_KEYBOARD_H

uint8_t irqSetKeyboard;
uint8_t scancode;
bool isScancodeTwoBytes;

int (keyboard_subscribe_int)();
int (keyboard_unsubscribe_int)();
int (keyboard_ih)();
int (keyboard_restore)();

#endif // _PROJ_KEYBOARD_H
