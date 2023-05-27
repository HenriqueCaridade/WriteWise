
#include "scancode.h"

const keyboard_t portugueseKeyboard = {
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-'},
    {'!', '"', '#', '$', '%', '&', '/', '(', ')', '='},
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_'},
};

keyboard_layout_t currentKeyboardLayout = none;

int setKeyboardLayout(keyboard_layout_t keyboardLayout) {
    switch (keyboardLayout) {
    case portuguese: currentKeyboardLayout = portuguese; currentKeyboard = portugueseKeyboard; break;
    case none:       currentKeyboardLayout = none; break;
    default: return 1;
    }
    return 0;
}

char getCharFromMakecode(uint8_t makecode) {
    if (currentKeyboardLayout == none) return 0;
    if (isShiftPressed) {
        if      (KEY_SCANCODE_ROW1_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW1_LAST) return currentKeyboard.row1Shift[makecode - KEY_SCANCODE_ROW1_FIRST];
        else if (KEY_SCANCODE_ROW2_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW2_LAST) return (isCapsLockActive ? currentKeyboard.row2 : currentKeyboard.row2Shift)[makecode - KEY_SCANCODE_ROW2_FIRST];
        else if (KEY_SCANCODE_ROW3_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW3_LAST) return (isCapsLockActive ? currentKeyboard.row3 : currentKeyboard.row3Shift)[makecode - KEY_SCANCODE_ROW3_FIRST];
        else if (KEY_SCANCODE_ROW4_FIRST <= makecode && makecode <= KEY_SCANCODE_M        ) return (isCapsLockActive ? currentKeyboard.row4 : currentKeyboard.row4Shift)[makecode - KEY_SCANCODE_ROW4_FIRST];
        else if (         KEY_SCANCODE_M <  makecode && makecode <= KEY_SCANCODE_ROW4_LAST) return currentKeyboard.row4Shift[makecode - KEY_SCANCODE_ROW4_FIRST];
    } else {
        if      (KEY_SCANCODE_ROW1_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW1_LAST) return currentKeyboard.row1[makecode - KEY_SCANCODE_ROW1_FIRST];
        else if (KEY_SCANCODE_ROW2_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW2_LAST) return (isCapsLockActive ? currentKeyboard.row2Shift : currentKeyboard.row2)[makecode - KEY_SCANCODE_ROW2_FIRST];
        else if (KEY_SCANCODE_ROW3_FIRST <= makecode && makecode <= KEY_SCANCODE_ROW3_LAST) return (isCapsLockActive ? currentKeyboard.row3Shift : currentKeyboard.row3)[makecode - KEY_SCANCODE_ROW3_FIRST];
        else if (KEY_SCANCODE_ROW4_FIRST <= makecode && makecode <= KEY_SCANCODE_M        ) return (isCapsLockActive ? currentKeyboard.row4Shift : currentKeyboard.row4)[makecode - KEY_SCANCODE_ROW4_FIRST];
        else if (         KEY_SCANCODE_M <  makecode && makecode <= KEY_SCANCODE_ROW4_LAST) return currentKeyboard.row4[makecode - KEY_SCANCODE_ROW4_FIRST];
    }
    if (makecode == SPACE_SCANCODE) return ' ';
    if (makecode == ENTER_SCANCODE) return '\n';
    return 0;
}

