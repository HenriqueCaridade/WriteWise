#ifndef _PROJ_MOUSE_H
#define _PROJ_MOUSE_H

uint8_t irqSetMouse;
struct packet mousePacket;

/**
 * @brief Subscribes mouse interrupts.
 *
 * This function subscribes to mouse interrupts by setting the IRQ policy for the mouse interrupt line. The function sets the irqSetMouse flag to indicate that the mouse interrupt is subscribed. The IRQ policy is set to re-enable the interrupts and exclusive. The hook ID is updated with the assigned value by the system.
 *
 * @return Non-zero if error occured.
 */
int (mouse_subscribe_int)();
/**
 * @brief Unsubscribes mouse interrupts.
 *
 * This function unsubscribes mouse interrupts by removing the IRQ policy of the mouse interrupt line. The function retrieves the hook ID associated with the mouse interrupt and passes it to sys_irqrmpolicy to remove the policy.
 * @return Non-zero if error occured.
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Processes mouse synchronization bytes.
 *
 * This function processes mouse synchronization bytes received from the mouse controller. It checks the byte index and the value of the current byte to determine whether it is the first byte or a subsequent byte. If it is the first byte, it is stored in the mouseBytes array at index 0. If it is a subsequent byte, it is stored in the `mouseBytes` array at the current byte index. The byte index is incremented accordingly.
 */
void (mouse_sync_bytes)();
/**
 * @brief Converts mouse bytes to a mouse packet.
 *
 * This function converts the mouse bytes stored in the `mouseBytes` array into a mouse packet. It assigns the corresponding bytes from `mouseBytes` to the `mousePacket` structure's `bytes` array. Additionally, it extracts specific bits from the first byte of `mouseBytes` and assigns them to the relevant fields of `mousePacket` for button status, overflow flags, and delta values for the x and y axes.
 */
void (mouse_bytes_to_packet)();
/**
 * @brief Mouse interrupt handler.
 *
 * This function is the interrupt handler for the mouse. It reads the byte from the mouse and performs the necessary actions based on the received data. If a complete packet is received, it converts the bytes to a mouse packet structure. 
 *
 * @return 0 if the byte was read successfully but a complete packet is not yet received, 1 if a complete packet is received and processed successfully, or an error code indicating the failure.
 */
int (mouse_int_h)();

/**
 * @brief Writes a command to the mouse.
 *
 * This function writes a command to the mouse by sending the command byte to the Keyboard Controller. It waits for the mouse response and checks if the response is an acknowledgement (ACK). The function retries the write operation a maximum number of attempts if the acknowledgement is not received.
 *
 * @param command The command byte to be sent to the mouse.
 * @return 0 if the command was written and acknowledged successfully, or 1 if an error occurred during the write operation.
 */
int (mouse_write)(uint8_t command);

#endif // _PROJ_MOUSE_H
