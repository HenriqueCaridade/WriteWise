#ifndef _PROJ_KEYBOARD_H
#define _PROJ_KEYBOARD_H

#include <stdbool.h>

#define BREAK_CODE              BIT(7)

#define BREAK_ESC               0x81
#define MAKE_ESC                0x01
#define LEFT_SHIFT_SCANCODE     0x2A
#define LEFT_CTRL_SCANCODE      0x1D
#define LEFT_ALT_SCANCODE       0x38
#define SPACE_SCANCODE          0x39
#define RIGHT_SHIFT_SCANCODE    0x36
#define CAPSLOCK_SCANCODE       0X3A

uint8_t irqSetKeyboard;
uint8_t scancode;
bool isScancodeTwoBytes;
bool isShiftPressed;
bool isCtrlPressed;
bool isAltPressed;
bool isCapsLockActive;
/**
 * @brief Subscribes to the keyboard interrupts.
 *
 * This function uses sys_irqsetpolicy() to set the IRQ policy for the keyboard interrupt line, enabling IRQ re-enabling and exclusive mode. The interrupt request line (IRQ) number 1 is used for the keyboard interrupts.
 *
 * @return Non-zero if error occured.
 */
int (keyboard_subscribe_int)();
/**
 * @brief Unsubscribes from the keyboard interrupts.
 *
 * This function unsubscribes from the keyboard interrupts by removing the IRQ policy associated with the keyboard interrupt line using sys_irqrmpolicy.
 *
 * @return Non-zero if error occured.
 */
int (keyboard_unsubscribe_int)();
/**
 * @brief Keyboard interrupt handler.
 *
 * This function reads the scancode from the keyboard output buffer using readKBCOutput function. If a two-byte scancode is detected, the corresponding flag is set. Otherwise, the scancode is processed to update the state of various keyboard-related flags. The appropriate flags are set or cleared based on the received scancode.
 *
 * @return Non-zero if error occured.
 */
int (keyboard_ih)();

/**
 * @brief Restores the keyboard configuration.
 *
 * This function first sends the read command to obtain the current configuration from the controller. The obtained configuration is then modified to enable interrupts by setting the appropriate flag. Next, the write command is sent to inform the controller that a write operation will follow. Finally, the modified configuration is written back to the controller.
 *
 * @return Non-zero if error occured.
 */
int (keyboard_restore)();

#endif // _PROJ_KEYBOARD_H
