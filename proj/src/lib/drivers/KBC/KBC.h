#ifndef _PROJ_KBC_H
#define _PROJ_KBC_H

/**
 * @brief Reads the status from the Keyboard Controller.
 * @param status Pointer to a variable where the read status byte will be stored.
 * @return Non-zero if error occured. 
 */
int readKBCStatus(uint8_t* status);
/**
 * @brief Reads data from the Keyboard Controller output buffer.
 *
 * This function reads data from the output buffer of the keyboard controller using the specified port. It checks the status byte of the Keyboard Controller for errors and verifies if the expected device type (keyboard or mouse) matches the received data.
 * @param port The port to read the data from.
 * @param output Pointer to a variable where the read data will be stored.
 * @param mouse Indicates whether the expected device type is a mouse (true) or keyboard (false).
 * @return Non-zero if error occured. 
 */
int readKBCOutput(uint8_t port, uint8_t *output, bool mouse);
/**
 * @brief Writes a command byte to the Keyboard Controller input buffer.
 *
 * This function writes a command byte to the input buffer of the keyboard controller using the specified port. It checks the status byte of the keyboard controller to ensure that the input buffer is not full. If the input buffer is not full, the command byte is sent to the specified port with sys_outb.
 *
 * @param port The port to write the command byte to.
 * @param commandByte The command byte to be written.
 * @return Non-zero if error occured.
 */
int writeKBCCommand(uint8_t port, uint8_t commandByte);

#endif // _PROJ_KBC_H
