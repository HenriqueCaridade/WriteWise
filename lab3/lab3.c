#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <lcom/timer.h>
#include <keyboard.h>
#include <i8054.h>

extern int counter;
extern uint8_t scancode;
extern uint32_t counter_KB;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t irq_set_keyboard;
  int ipc_status;
  message msg;
  bool isTwoBytes = false;

  if (keyboard_subscribe_int(&irq_set_keyboard)) return 1;

  while (scancode != BREAK_ESC) { /* You may want to use a different condition */
    /* Get a request message. */
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed.");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification 11:*/
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            /*
            Two Bytes:
                Interrupt: 0xE0 - o próximo é two_byte
                Interrupt: 0xXX - data

            One Byte:
                Interrupt: 0xXX - data
            */
            kbc_ih();

            // kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
            // BreakCode: 0xE0 0x00
            // MakeCode: 0xXX
            // BreakCode: 0xE0 0x00
            // BreakCode: 0xXX
            // Errado!
            
            if (scancode == TWO_BYTES){
              isTwoBytes = true;
            } else {
              if(isTwoBytes){
                uint8_t bytes[2] = {TWO_BYTES, scancode}; // 0xE0 0xXX
                kbd_print_scancode(!(scancode & MAKE_CODE), 2, bytes);
              } else {
                kbd_print_scancode(!(scancode & MAKE_CODE), 1, &scancode);
              }
              isTwoBytes = false;
            }
            // MakeCode: 0xE0 0xXX
            // BreakCode: 0xE0 0xXX
          }
          break;
      }
    }
  }
  if (keyboard_unsubscribe_int()) return 1;
  if (kbd_print_no_sysinb(counter_KB)) return 1;
  return 0;
}

int(kbd_test_poll)() {
  bool isTwoBytes = false;
  while(scancode != BREAK_ESC){
    if ((read_KBC_output(KBC_OUT_CMD, &scancode))) continue;
    if (scancode == TWO_BYTES){
      isTwoBytes = true;
    } else {
      if(isTwoBytes){
        uint8_t bytes[2] = {TWO_BYTES, scancode};
        kbd_print_scancode(!(scancode & MAKE_CODE), 2, bytes);
      } else {
        kbd_print_scancode(!(scancode & MAKE_CODE), 1, &scancode);
      }
      isTwoBytes = false;
    }
  }
  if (kbd_print_no_sysinb(counter_KB)) return 1;
  return keyboard_restore();
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t irq_set_timer;
  uint8_t irq_set_keyboard;
  int ipc_status;
  message msg;
  bool isTwoBytes = false;
  uint8_t timer = n;

  if (timer_subscribe_int(&irq_set_timer)) return 1;
  if (keyboard_subscribe_int(&irq_set_keyboard)) return 1;

  while (timer && scancode != BREAK_ESC) { /* You may want to use a different condition */
    /* Get a request message. */
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed.");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification 11:*/
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            kbc_ih();
            if (scancode == TWO_BYTES){
              isTwoBytes = true;
            } else {
              if(isTwoBytes){
                uint8_t bytes[2] = {TWO_BYTES, scancode};
                kbd_print_scancode(!(scancode & MAKE_CODE), 2, bytes);
              } else {
                kbd_print_scancode(!(scancode & MAKE_CODE), 1, &scancode);
              }
              isTwoBytes = false;
            }
            timer = n;
            counter = 0;
          }
          if (msg.m_notify.interrupts & irq_set_timer){
            timer_int_handler();
            if(counter % 60 == 0){
              timer--;
              counter = 0;
            }
          }
          break;
      }
    }
  }
  if (keyboard_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  if (kbd_print_no_sysinb(counter_KB)) return 1;
  return 0;
}
