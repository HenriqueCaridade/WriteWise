
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
#define INFO_NREADY     (INFO_BITS | 0x9)
#define INFO_READY_ACK  (INFO_BITS | 0xA)
#define INFO_NREADY_ACK (INFO_BITS | 0xB)
#define INFO_WON        (INFO_BITS | 0xC)
#define INFO_EXIT       (INFO_BITS | 0xD)

#define NORMAL_BIT_MAX  (INFO_BITS - 1)

uint8_t irqSetSerialPort;
uint8_t serialPortByte;
uint8_t receivingSerialPort;
uint64_t waitingAckStartTime;
bool waitingAckSerialPort;

/**
 * @brief Subscribes to interrupts from the serial port.
 *
 * This function subscribes interrupts from the serial port specified by COM1_IRQ. It sets the IRQ_REENABLE and IRQ_EXCLUSIVE flags in the sys_irqsetpolicy function to enable and exclusively allocate the interrupt line for this driver.
 *
 * @return  Non-zero if error occured.
 */
int serial_port_subscribe_int();
/**
 * @brief Unsubscribes from interrupts of the serial port.
 *
 * This function unsubscribes from interrupts of the serial port that was previously subscribed using `serial_port_subscribe_int`. It releases the interrupt line allocated for this driver.
 *
 * @return 0 upon success, or a non-zero value if the unsubscription fails.
 */
int serial_port_unsubscribe_int();

/**
 * @brief Initializes the serial port.
 *
 * This function initializes the serial port by clearing the Interrupt Enable Register (IER) and enabling received data interrupts.
 *
 * @return 0 if the initialization is successful, 1 if there is an error during the register read/write operations.
 */
int serial_port_init();
/**
 * @brief Interrupt handler for the serial port.
 *
 * This function is the interrupt handler for the serial port. It reads the Interrupt Identification Register (IIR) to determine the type of interrupt and performs the necessary actions accordingly.
 *
 * @return 0 if a byte was successfully read, 1 if an error occurred during the read operation, or if no interrupt is pending or no data is available.
 */
int serial_port_ih();
/**
 * @brief Retrieves the status of the serial port.
 *
 * This function retrieves the status of the serial port by reading the Line Status Register (LSR) and storing the result in the specified status variable. It uses util_sys_inb  to perform the register read operation. 
 * @param status Pointer to a variable where the serial port status will be stored.
 * @return 0 if the status is successfully retrieved, or 1 if there is an error during the register read operation.
 */
int serial_port_get_status(uint8_t* status);
/**
 * @brief Sends a byte of data through the serial port.
 *
 * This function sends a byte of data through the serial port by writing it to the Transmitter Holding Register (THR). It first checks the status of the serial port to ensure that the THR is empty before writing the data. If the THR is not empty, the function waits for a certain amount of time and retries the operation.
 *
 * @param byte The byte of data to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendByte      (uint8_t byteValue);
/**
 * @brief Sends an unsigned long value through the serial port.
 *
 * This function sends an unsigned long value through the serial port. It first sends an indicator byte (INFO_ULONG) to indicate the type of data being sent. Then, it sends each byte of the long value sequentially.
 *
 * @param longValue The unsigned long value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendULong     (uint64_t longValue);
/**
 * @brief Sends a long value through the serial port.
 *
 * This function sends a long value through the serial port. It first sends an indicator byte (INFO_LONG) to indicate the type of data being sent. Then, it sends each byte of the long value sequentially. The function uses the serialPortSendByte function to send each byte. 
 *
 * @param longValue The long value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendLong      (int64_t longValue);
/**
 * @brief Sends an unsigned integer value through the serial port.
 *
 * This function sends an unsigned integer value through the serial port. It first sends an indicator byte (INFO_UINT) to indicate the type of data being sent. Then, it sends each byte of the unsigned integer value sequentially. The function uses the serialPortSendByte function to send each byte. 
 * @param intValue The unsigned integer value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendUInt      (uint32_t intValue);
/**
 * @brief Sends a signed integer value through the serial port.
 *
 * This function sends a signed integer value through the serial port. It first sends an indicator byte (INFO_INT) to indicate the type of data being sent. Then, it sends each byte of the signed integer value sequentially. The function uses the serialPortSendByte function to send each byte. 
 *
 * @param intValue The signed integer value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendInt       (int32_t intValue);
/**
 * @brief Sends a floating-point value through the serial port.
 *
 * This function sends a floating-point value through the serial port. It first sends an indicator byte (INFO_FLOAT) to indicate the type of data being sent. Then, it sends each byte of the floating-point value sequentially. The function uses the serialPortSendByte function to send each byte. 
 * @param floatValue The floating-point value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendFloat     (float floatValue);
/**
 * @brief Sends a double-precision floating-point value through the serial port.
 *
 * This function sends a double-precision floating-point value through the serial port. It first sends an indicator byte (INFO_DOUBLE) to indicate the type of data being sent. Then, it sends each byte of the double-precision value sequentially. 
 *
 * @param doubleValue The double-precision floating-point value to be sent through the serial port.
 * @return Non-zero if error occured.
 */
int serialPortSendDouble    (double doubleValue);
/**
 * @brief Reads a byte from the serial port.
 *
 * This function reads a byte from the serial port. It first checks the status of the serial port using the serial_port_get_status function. If there is an error condition it returns 1. It also checks if the received data is ready to be read from the Receiver Buffer Register (RBR). If the data is ready, it reads the byte from the RBR and stores it in the serialPortByte variable. 
 *
 * @return Non-zero if error occured.
 */
int serialPortReadByte      ();
/**
 * @brief Reads an unsigned long (uint64_t) value from the serial port.
 *
 * This function reads an unsigned long (uint64_t) value from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 8 bytes have been received. Once all bytes have been received, the function interprets them as a uint64_t value and stores it in the `longValue` variable.
 * @return Non-zero if error occured.
 */
int serialPortReadULong     (uint64_t* longValue);
/**
 * @brief Reads a long (int64_t) value from the serial port.
 *
 * This function reads a long (int64_t) value from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 8 bytes have been received. Once all bytes have been received, the function interprets them as an int64_t value and stores it in the `longValue` variable.
 * @return Non-zero if error occured.
 */
int serialPortReadLong      (int64_t* longValue);
/**
 * @brief Reads an unsigned integer (uint32_t) value from the serial port.
 *
 * This function reads an unsigned integer (uint32_t) value from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 4 bytes have been received. Once all bytes have been received, the function interprets them as a uint32_t value and stores it in the `intValue` variable. 
 *  @return Non-zero if error occured.
 */
int serialPortReadUInt      (uint32_t* intValue);
/**
 * @brief Reads a signed integer (int32_t) value from the serial port.
 *
 * This function reads a signed integer (int32_t) value from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 4 bytes have been received. Once all bytes have been received, the function interprets them as an int32_t value and stores it in the `intValue` variable. 
 * @return Non-zero if error occured.
 */
int serialPortReadInt       (int32_t* intValue);
/**
 * @brief Reads a floating-point value (float) from the serial port.
 *
 * This function reads a floating-point value (float) from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 4 bytes have been received. Once all bytes have been received, the function interprets them as a float value and stores it in the `floatValue` variable. 
 * @return Non-zero if error occured.
 */
int serialPortReadFloat     (float* floatValue);
/**
 * @brief Reads a double precision value (double) from the serial port.
 *
 * This function reads a double precision value (double) from the serial port. It stores the received bytes in the `bitsSerialPort` array and checks if all 8 bytes have been received. Once all bytes have been received, the function interprets them as a double value and stores it in the `doubleValue` variable. 
 * @return Non-zero if error occured.
 */
int serialPortReadDouble    (double* doubleValue);
/**
 * @brief Sends an acknowledgment (ACK) signal over the serial port.
 *
 * This function sends an acknowledgment (ACK) signal over the serial port. It first clears the FIFO buffer using the `serialPortClearFIFO` function, and then sends the ACK byte using `serialPortSendByte`.
 * @return Non-zero if error occured.
 */
int serialPortSendAck       ();
/**
 * @brief Sends a "ready" signal over the serial port.
 *
 * This function sends a "ready" signal over the serial port. It sets the `waitingAckSerialPort` flag to true and sends the "ready" byte using `serialPortSendByte`.
 *  @return Non-zero if error occured.
 */
int serialPortSendReady     ();
/**
 * @brief Sends a "not ready" signal over the serial port.
 *
 * This function sends a "not ready" signal over the serial port. It sets the `waitingAckSerialPort` flag to true and sends the "not ready" byte using `serialPortSendByte`.
 *  @return Non-zero if error occured.
 */
int serialPortSendNReady    ();
/**
 * @brief Sends a "ready acknowledgment" signal over the serial port.
 *
 * This function sends a "ready acknowledgment" signal over the serial port. It clears the FIFO using the `serialPortClearFIFO` function and sends the "ready acknowledgment" byte using `serialPortSendByte`. 
 * @return Non-zero if error occured.
 */
int serialPortSendReadyAck  ();
/**
 * @brief Sends a "not ready acknowledgment" signal over the serial port.
 *
 * This function sends a "not ready acknowledgment" signal over the serial port. It clears the FIFO using the `serialPortClearFIFO` function and sends the "not ready acknowledgment" byte using `serialPortSendByte`.
 * @return Non-zero if error occured.
 */
int serialPortSendNReadyAck ();
/**
 * @brief Sends a "win" signal over the serial port.
 *
 * This function sends a "win" signal over the serial port by using the `serialPortSendByte` function to send the corresponding byte.
 * @return Non-zero if error occured.
 */
int serialPortSendWon       ();
/**
 * @brief Sends an "exit" signal over the serial port.
 *
 * This function sends an "exit" signal over the serial port by using the `serialPortSendByte` function to send the corresponding byte.
 *  @return Non-zero if error occured.
 */
int serialPortSendExit      ();
/**
 * @brief Sends a "negative acknowledgment" signal over the serial port.
 *
 * This function sends a "negative acknowledgment" signal over the serial port by using the `serialPortSendByte` function to send the corresponding byte. 
 * @return Non-zero if error occured.
 */
int serialPortSendNack      ();
/**
 * @brief Clears the FIFO (First-In-First-Out) of the serial port.
 *
 * This function clears the FIFO of the serial port by writing the clear command to the FIFO Control Register (FCR). It uses sys_outb to perform the write operation. 
 * @return Non-zero if error occured.
 * */
int serialPortClearFIFO     ();

#endif // _PROJ_SERIAL_PORT_H
