
#ifndef _PROJ_SERIAL_PORT_H
#define _PROJ_SERIAL_PORT_H

#include <lcom/lcf.h>
#include "../timer/timer.h"
#include "UART.h"

#define ACK_TIMEOUT  1

#define INFO_BITS       0xF0
#define INFO_ACK        (INFO_BITS | 0x0)
#define INFO_NACK       (INFO_BITS | 0x1)
#define INFO_ULONG      (INFO_BITS | 0x2)
#define INFO_LONG       (INFO_BITS | 0x3)
#define INFO_UINT       (INFO_BITS | 0x4)
#define INFO_INT        (INFO_BITS | 0x5)
#define INFO_FLOAT      (INFO_BITS | 0x6)
#define INFO_DOUBLE     (INFO_BITS | 0x7)
#define INFO_READY      (INFO_BITS | 0x8)
#define INFO_NREADY     (INFO_BITS | 0x8)
#define INFO_READY_ACK  (INFO_BITS | 0x9)
#define INFO_NREADY_ACK (INFO_BITS | 0xA)

#define NORMAL_BIT_MAX  (INFO_BITS - 1)

uint8_t irqSetSerialPort;
uint8_t serialPortByte;
uint8_t receivingSerialPort;
uint64_t waitingAckStartTime;
bool waitingAckSerialPort;

int serial_port_subscribe_int();
int serial_port_unsubscribe_int();
int serial_port_init();
int serial_port_ih();
int serial_port_get_status(uint8_t* status);

int serialPortSendByte      (uint8_t byteValue);
int serialPortSendULong     (uint64_t longValue);
int serialPortSendLong      (int64_t longValue);
int serialPortSendUInt      (uint32_t intValue);
int serialPortSendInt       (int32_t intValue);
int serialPortSendFloat     (float floatValue);
int serialPortSendDouble    (double doubleValue);
int serialPortReadByte      ();
int serialPortReadULong     (uint64_t* longValue);
int serialPortReadLong      (int64_t* longValue);
int serialPortReadUInt      (uint32_t* intValue);
int serialPortReadInt       (int32_t* intValue);
int serialPortReadFloat     (float* floatValue);
int serialPortReadDouble    (double* doubleValue);
int serialPortSendAck       ();
int serialPortSendReady     ();
int serialPortSendNReady    ();
int serialPortSendReadyAck  ();
int serialPortSendNReadyAck ();
int serialPortSendNack      ();
int serialPortClearFIFO     ();

#endif // _PROJ_SERIAL_PORT_H
