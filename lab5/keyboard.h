#ifndef KEYBOARD_H
#define KEYBOARD_H

int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
int (read_KBC_output)(uint8_t port, uint8_t *output);
void (kbc_ih)();
int (keyboard_restore)();

#endif // KEYBOARD_H
