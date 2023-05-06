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
    if (read_KBC_output(KBC_OUT_CMD, &scancode, false)) printf("ERROR: Could not read scancode!\n");
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
    if (sys_outb(0x64, 0x20)) return 1;
    uint8_t config;
    if (util_sys_inb(0x60, &config)) return 1;
    config |= 1;
    if (sys_outb(0x64, 0x60)) return 1;
    return sys_outb(0x60, config);
}
