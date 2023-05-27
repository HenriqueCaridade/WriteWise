#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"
#include "i8042.h"
#include "keyboard.h"

int hookIdKeyboard = 1;
uint32_t counter_KB = 0;
bool isTwoBytes = false;
bool isShiftPressed = false;
bool isCtrlPressed = false;
bool isAltPressed = false;
bool isCapsLockActive = false;

int (keyboard_subscribe_int)() {
    irqSetKeyboard = BIT(hookIdKeyboard);
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdKeyboard);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&hookIdKeyboard);
}

int (keyboard_ih)() {
    if (readKBCOutput(KBC_OUT_CMD, &scancode, false)) {
        printf("ERROR: Could not read scancode!\n");
        isTwoBytes = false;
        isScancodeTwoBytes = false;
        return 0;
    }
    if (scancode == TWO_BYTES){
        isTwoBytes = true;
        return 0;
    } else {
        switch (scancode) {
        case LEFT_SHIFT_SCANCODE: case RIGHT_SHIFT_SCANCODE:
            isShiftPressed = true; return 0;
        case LEFT_SHIFT_SCANCODE | BREAK_CODE: case RIGHT_SHIFT_SCANCODE | BREAK_CODE:
            isShiftPressed = false; return 0;
        case LEFT_CTRL_SCANCODE:
            isCtrlPressed = true; return 0;
        case LEFT_CTRL_SCANCODE | BREAK_CODE:
            isCtrlPressed = false; return 0;
        case LEFT_ALT_SCANCODE:
            isAltPressed = true; return 0;
        case LEFT_ALT_SCANCODE | BREAK_CODE:
            isAltPressed = false; return 0;
        case CAPSLOCK_SCANCODE:
            isCapsLockActive = !isCapsLockActive; return 0;
        default: break;
        }
        isScancodeTwoBytes = isTwoBytes;
        isTwoBytes = false;
        return 1;
    }
}

int (keyboard_restore)(){
    if (writeKBCCommand(KBC_IN_CMD, KBC_READ_CMD)) return 1;
    uint8_t config;
    if (readKBCOutput(KBC_OUT_CMD, &config, false)) return 1;
    config |= ENABLE_INT;
    if (writeKBCCommand(KBC_IN_CMD, KBC_WRITE_CMD)) return 1;
    return writeKBCCommand(KBC_WRITE_CMD, config);
}
