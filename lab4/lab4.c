// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include <lcom/timer.h>
#include "mouse.h"
#include "KBC.h" 
#include "i8042.h"

extern int counter;
extern uint8_t byteIndex;
extern struct packet mousePacket;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  uint8_t irq_set_mouse;
  int ipc_status;
  message msg;

  if (mouse_subscribe_int(&irq_set_mouse)) return 1;
  if (mouse_write(ENABLE_DATA_REPORT)) return 1;

  while (cnt) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed.");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification 11:*/
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_mouse) {
            // Mouse Interrupt
            mouse_ih();
            mouse_sync_bytes();
            if (byteIndex == 3) { // Packet está completo
              mouse_bytes_to_packet();
              mouse_print_packet(&mousePacket);
              byteIndex = 0;
              cnt--;
            }
          }
          break;
      }
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) return 1;
  if (mouse_unsubscribe_int()) return 1;
  return 0;
}


int (mouse_test_async)(uint8_t idle_time) {
  uint8_t irq_set_mouse;
  uint8_t irq_set_timer;
  int ipc_status;
  message msg;

  const int counterIdleTime = idle_time * 60;

  if (timer_subscribe_int(&irq_set_timer)) return 1;
  if (mouse_subscribe_int(&irq_set_mouse)) return 1;
  if (mouse_write(ENABLE_DATA_REPORT)) return 1;

  bool running = true;
  while (running) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed.");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification 11:*/
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_timer) {
            // Timer Interrupt
            timer_int_handler();
            if (counterIdleTime == counter) {
              running = false;
              break;
            }
          }
          if (msg.m_notify.interrupts & irq_set_mouse) {
            // Mouse Interrupt
            counter = 0; // Reset counter.
            mouse_ih();
            mouse_sync_bytes();
            if (byteIndex == 3) { // Packet está completo
              mouse_bytes_to_packet();
              mouse_print_packet(&mousePacket);
              byteIndex = 0;
            }
          }
          break;
      }
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) return 1;
  if (mouse_unsubscribe_int()) return 1;
  if (timer_unsubscribe_int()) return 1;
  return 0;
}

enum state {
  S, U, V, D, E
};

int16_t diffX;
int16_t diffY;

void (mouse_state_update)(enum state* currentState, uint8_t x_len, uint8_t tolerance){
  switch (*currentState) {
    case S: {
      if (mousePacket.lb && !mousePacket.mb && !mousePacket.rb) {
        if (mousePacket.delta_x + tolerance >= 0 && mousePacket.delta_y + tolerance >= 0) {
          *currentState = U;
          diffX = mousePacket.delta_x;
          diffY = mousePacket.delta_y;
        }
      }
    } break;
    case U: {
      if (!mousePacket.mb && !mousePacket.rb){
        if (mousePacket.lb) {
          if (mousePacket.delta_x + tolerance >= 0 && mousePacket.delta_y + tolerance >= 0) {
            diffX += mousePacket.delta_x;
            diffY += mousePacket.delta_y;
          } else *currentState = S;
        } else if (diffX >= x_len && diffY >= diffX) {
          *currentState = V;
        } else *currentState = S;
      } else *currentState = S;
    } break;
    case V: {
      if (!mousePacket.lb && !mousePacket.mb) {
        if (abs(mousePacket.delta_x) <= tolerance || abs(mousePacket.delta_y) <= tolerance) {
          if (mousePacket.rb){
            *currentState = D;
            diffX = mousePacket.delta_x;
            diffY = -mousePacket.delta_y;
          }
        } else *currentState = S;
      } else *currentState = S;
    } break;
    case D: {
      if (!mousePacket.lb && !mousePacket.mb){
        if (mousePacket.rb) {
          if (mousePacket.delta_x + tolerance >= 0 && tolerance - mousePacket.delta_y >= 0) {
            diffX += mousePacket.delta_x;
            diffY -= mousePacket.delta_y;
          } else *currentState = S;
        } else if (diffX >= x_len && diffY >= diffX) {
          *currentState = E;
        } else *currentState = S;
      } else *currentState = S;
    } break;
    case E: break;
  }
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t irq_set_mouse;
  int ipc_status;
  message msg;

  if (mouse_subscribe_int(&irq_set_mouse)) return 1;
  if (mouse_write(ENABLE_DATA_REPORT)) return 1;

  enum state currentState = S;
  while (currentState != E) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed.");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification 11:*/
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_mouse) {
            // Mouse Interrupt
            mouse_ih();
            mouse_sync_bytes();
            if (byteIndex == 3) { // Packet está completo
              mouse_bytes_to_packet();
              mouse_state_update(&currentState, x_len, tolerance);
              mouse_print_packet(&mousePacket);
              byteIndex = 0;
            }
          }
          break;
      }
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) return 1;
  if (mouse_unsubscribe_int()) return 1;
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    while (cnt) {
        if (mouse_write(MOUSE_READ_DATA)) return 1;
        mouse_ih();
        mouse_sync_bytes();
        if (byteIndex == 3) {
            mouse_bytes_to_packet();
            mouse_print_packet(&mousePacket);
            byteIndex = 0;
            cnt--;
            tickdelay(micros_to_ticks(period * 1000));
        }
    }

    if (mouse_write(ENABLE_STREAM_MODE)) return 1;
    if (mouse_write(DISABLE_DATA_REPORT)) return 1;
  
    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD)) return 1;
    if (write_KBC_command(KBC_WRITE_CMD, minix_get_dflt_kbc_cmd_byte())) return 1;  
    return 0;
}
