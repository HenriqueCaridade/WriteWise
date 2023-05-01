#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "i8254.h"

int counter = 0;
int hookId = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2) return 1;
  if (freq < 19 || freq > TIMER_FREQ) return 1;
  uint16_t initValue = TIMER_FREQ / freq;
  uint8_t conf = 0;
  if (timer_get_conf(timer, &conf) != 0) return 1;
  conf &= 0x0f;
  conf |= 0x30;
  conf |= timer << 6;
  

  uint8_t timerPort = TIMER_0 + timer;
  uint8_t MSB, LSB;
  util_get_LSB (initValue, &LSB);
  util_get_MSB (initValue, &MSB);
  if (sys_outb(TIMER_CTRL, conf)) return 1;
  if (sys_outb(timerPort, LSB)) return 1;
  if (sys_outb(timerPort, MSB)) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == 0) return 1;
  *bit_no = BIT(hookId);

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookId)) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hookId)) return 1;
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t control;
  control = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, control)) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf;

  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      conf.in_mode = (st >> 4) & 3;
      break;
    case tsf_mode:
      conf.count_mode = (st >> 1) & 7;
      if(conf.count_mode >= 7) {
        conf.count_mode ^= 4;
      }
      break;
    case tsf_base:
      conf.bcd = (st & 1);
  }

  timer_print_config(timer, field, conf);
  return 0;
}
