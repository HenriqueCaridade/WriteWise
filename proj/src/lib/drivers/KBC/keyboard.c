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

int (keyboard_subscribe_int)() {
    irqSetKeyboard = BIT(hookIdKeyboard);
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdKeyboard);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&hookIdKeyboard);
}

int (keyboard_ih)() {
    if (read_KBC_output(KBC_OUT_CMD, &scancode, false)) {
        printf("ERROR: Could not read scancode!\n");
        isTwoBytes = false;
        isScancodeTwoBytes = false;
        return 0;
    }
    if (scancode == TWO_BYTES){
        isTwoBytes = true;
        return 0;
    } else {
        isScancodeTwoBytes = isTwoBytes;
        isTwoBytes = false;
        return 1;
    }
}

int (keyboard_restore)(){
    if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD)) return 1;
    uint8_t config;
    if (read_KBC_output(KBC_OUT_CMD, &config, false)) return 1;
    config |= ENABLE_INT;
    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD)) return 1;
    return write_KBC_command(KBC_WRITE_CMD, config);
}
