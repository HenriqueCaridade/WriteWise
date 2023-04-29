#ifndef _LCOM_I8042H
#define _LCOM_I8042H

#define IRQ_KEYBOARD    1

#define BREAK_ESC       0x81   //0111 0001
#define MAKE_CODE       BIT(7) //1000 0000
#define TWO_BYTES       0xE0   //1110 0000
#define ENABLE_INT      BIT(0) //0000 0001

#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10

#define BREAK_ESC        0x81
#define MAKE             BIT(7)
#define TWO              0XE0

#endif
