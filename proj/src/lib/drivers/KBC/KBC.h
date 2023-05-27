#ifndef _PROJ_KBC_H
#define _PROJ_KBC_H

int readKBCStatus(uint8_t* status);
int readKBCOutput(uint8_t port, uint8_t *output, bool mouse);
int writeKBCCommand(uint8_t port, uint8_t commandByte);

#endif // _PROJ_KBC_H
