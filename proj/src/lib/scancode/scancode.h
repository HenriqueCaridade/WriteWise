#ifndef _PROJ_SCANCODE_LIST_H
#define _PROJ_SCANCODE_LIST_H

#include <stdint.h>
#include "../drivers/KBC/keyboard.h"

#define BACKSPACE_SCANCODE      0x0E
#define ENTER_SCANCODE          0x1C

#define ROW1_KEY_AMMOUT         11
#define KEY_SCANCODE_1          0x02
#define KEY_SCANCODE_2          0x03
#define KEY_SCANCODE_3          0x04
#define KEY_SCANCODE_4          0x05
#define KEY_SCANCODE_5          0x06
#define KEY_SCANCODE_6          0x07
#define KEY_SCANCODE_7          0x08
#define KEY_SCANCODE_8          0x09
#define KEY_SCANCODE_9          0x0A
#define KEY_SCANCODE_0          0x0B
#define KEY_SCANCODE_APOSTROPHE 0x0C
#define KEY_SCANCODE_ROW1_FIRST KEY_SCANCODE_1
#define KEY_SCANCODE_ROW1_LAST  KEY_SCANCODE_APOSTROPHE

#define ROW2_KEY_AMMOUT         10
#define KEY_SCANCODE_Q          0x10
#define KEY_SCANCODE_W          0x11
#define KEY_SCANCODE_E          0x12
#define KEY_SCANCODE_R          0x13
#define KEY_SCANCODE_T          0x14
#define KEY_SCANCODE_Y          0x15
#define KEY_SCANCODE_U          0x16
#define KEY_SCANCODE_I          0x17
#define KEY_SCANCODE_O          0x18
#define KEY_SCANCODE_P          0x19
#define KEY_SCANCODE_ROW2_FIRST KEY_SCANCODE_Q
#define KEY_SCANCODE_ROW2_LAST  KEY_SCANCODE_P

#define ROW3_KEY_AMMOUT         9
#define KEY_SCANCODE_A          0x1E
#define KEY_SCANCODE_S          0x1F
#define KEY_SCANCODE_D          0x20
#define KEY_SCANCODE_F          0x21
#define KEY_SCANCODE_G          0x22
#define KEY_SCANCODE_H          0x23
#define KEY_SCANCODE_J          0x24
#define KEY_SCANCODE_K          0x25
#define KEY_SCANCODE_L          0x26
#define KEY_SCANCODE_ROW3_FIRST KEY_SCANCODE_A
#define KEY_SCANCODE_ROW3_LAST  KEY_SCANCODE_L

#define ROW4_KEY_AMMOUT         10
#define KEY_SCANCODE_Z          0x2C
#define KEY_SCANCODE_X          0x2D
#define KEY_SCANCODE_C          0x2E
#define KEY_SCANCODE_V          0x2F
#define KEY_SCANCODE_B          0x30
#define KEY_SCANCODE_N          0x31
#define KEY_SCANCODE_M          0X32
#define KEY_SCANCODE_COMMA      0X33
#define KEY_SCANCODE_PERIOD     0X34
#define KEY_SCANCODE_DASH       0x35
#define KEY_SCANCODE_ROW4_FIRST KEY_SCANCODE_Z
#define KEY_SCANCODE_ROW4_LAST  KEY_SCANCODE_DASH

/**
 * @struct keyboard_t
 * @brief Struct that holds each of the key's corresponding letter
 * 
 */
typedef struct {
    uint8_t row1[ROW1_KEY_AMMOUT];
    uint8_t row2[ROW2_KEY_AMMOUT];
    uint8_t row3[ROW3_KEY_AMMOUT];
    uint8_t row4[ROW4_KEY_AMMOUT];
    uint8_t row1Shift[ROW1_KEY_AMMOUT];
    uint8_t row2Shift[ROW2_KEY_AMMOUT];
    uint8_t row3Shift[ROW3_KEY_AMMOUT];
    uint8_t row4Shift[ROW4_KEY_AMMOUT];
} keyboard_t;
const keyboard_t portugueseKeyboard;
keyboard_t currentKeyboard;

typedef enum {
    none,
    portuguese
} keyboard_layout_t;
keyboard_layout_t currentKeyboardLayout;

/**
 * @brief Sets the keyboard layout.
 *
 * This function sets the keyboard layout based on the provided keyboardLayout parameter.
 *
 * @param keyboardLayout The keyboard layout to be set.
 * @return 0 if the keyboard layout was set successfully, otherwise 1 if an error occurred or an unsupported layout was provided.
 */
int setKeyboardLayout(keyboard_layout_t keyboardLayout);

/**
 * @brief Retrieves a character from a given makecode.
 *
 * This function returns the corresponding character based on the provided makecode parameter and the current keyboard layout.
 *
 * @param makecode The makecode to retrieve the character from.
 * @return The corresponding character if found, ' ' (space) if makecode matches SPACE_SCANCODE, '\n' (newline) if makecode matches ENTER_SCANCODE, or 0 if no matching character is found or the current keyboard layout is set to none.
 */
char getCharFromMakecode(uint8_t makecode);

#endif // _PROJ_SCANCODE_LIST_H
