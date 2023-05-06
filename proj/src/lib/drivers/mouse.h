#ifndef _PROJ_MOUSE_H
#define _PROJ_MOUSE_H

uint8_t irqSetMouse;

int (mouse_subscribe_int)();
int (mouse_unsubscribe_int)();

void (mouse_sync_bytes)();
void (mouse_bytes_to_packet)();

int (mouse_int_h)();

int (mouse_write)(uint8_t command);

#endif // _PROJ_MOUSE_H
