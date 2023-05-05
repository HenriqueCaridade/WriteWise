#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include <i8042.h>
#include <keyboard.h>

int hookIdKeyboard = 1;
uint32_t counter_KB = 0;
uint8_t scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == 0) return 1;
  *bit_no = BIT(hookIdKeyboard);
  if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdKeyboard)) return 1;
  return 0;
}

int (keyboard_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hookIdKeyboard)) return 1;
  return 0;
}

int (read_KBC_output)(uint8_t port, uint8_t *output) {
  uint8_t status;
  uint8_t i = 10;

  while (i--) {
    if (util_sys_inb(KBC_STATUS_REG, &status)) return 1;

    if (status & BIT(0)){
      if (util_sys_inb(port, output)) return 1;
      if (status & BIT(7)) return 1;
      if (status & BIT(6)) return 1;
      return 0;
    } 
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
}

void (kbc_ih)() {
  if (read_KBC_output(KBC_OUT_CMD, &scancode)) printf("ERROR: Could not read scancode!\n");
}

int (keyboard_restore)(){
  if (sys_outb(0x64, 0x20)) return 1;
  uint8_t config;
  if (util_sys_inb(0x60, &config)) return 1;
  config |= 1;
  if (sys_outb(0x64, 0x60)) return 1;
  return sys_outb(0x60, config);
}
