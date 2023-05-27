
#ifndef _PROJ_UART_H
#define _PROJ_UART_H

#define WAIT_UART 20000
#define MAX_ATTEMPTS_UART 10

#define COM1_BASE_ADDR  0x3F8
#define COM1_IRQ        4

#define RBR (COM1_BASE_ADDR + 0x00)
#define THR RBR
#define IER (COM1_BASE_ADDR + 0x01)
#define IIR (COM1_BASE_ADDR + 0x02)
#define FCR IIR
#define LCR (COM1_BASE_ADDR + 0x03)
#define MCR (COM1_BASE_ADDR + 0x04)
#define LSR (COM1_BASE_ADDR + 0x05)
#define MSR (COM1_BASE_ADDR + 0x06)
#define SR  (COM1_BASE_ADDR + 0x07)

#define DLL RBR
#define DLM IER

#define LSR_DATA_READY      BIT(0)
#define LSR_OVERRUN_ERROR   BIT(1)
#define LSR_PARITY_ERROR    BIT(2)
#define LSR_FRAMING_ERROR   BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_THR_EMPTY       BIT(5)
#define LSR_THR_N_TSR_EMPTY BIT(6)
#define LSR_FIFO_ERROR      BIT(7)

#define IER_ENABLE_RECEIVED_INT         BIT(0)
#define IER_ENABLE_TRANSMITER_INT       BIT(1)
#define IER_ENABLE_RECEIVER_STATUS_INT  BIT(2)
#define IER_ENABLE_MODEM_STATUS_INT     BIT(3)
#define IER_CLEAR (BIT(4) | BIT(5) | BIT(6) | BIT(7))

#define IIR_NO_INTERRUPT_PENDING    BIT(0)
#define IIR_INTERRUPT_PENDING_INFO  (BIT(1) | BIT(2) | BIT(3))
#define IIR_RECEIVER_STATUS         (BIT(1) | BIT(2))
#define IIR_RECEIVED_DATA_AVAILABLE BIT(2)
#define IIR_CHAR_TIMEOUT_FIFO       (BIT(2) | BIT(3))
#define IIR_THR_EMPTY               BIT(1)
#define IIR_MODEM_STATUS            0

#define FCR_CLEAR (BIT(0) | BIT(1) | BIT(2))

#endif // _PROJ_UART_H
