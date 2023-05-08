#ifndef _PROJ_KBC_H
#define _PROJ_KBC_H

int (read_KBC_status)(uint8_t* status);

int (read_KBC_output)(uint8_t port, uint8_t *output, bool mouse);

int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif // _PROJ_KBC_H