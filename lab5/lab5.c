// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "keyboard.h"
#include "i8042.h"
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if (set_graphic_mode(mode)) return 1;
    sleep(delay);
    if (vg_exit()) return 1;
    return 0;
}

int (wait_ESC)() {
    uint8_t irq_set_keyboard;
    int ipc_status;
    message msg;

    if (keyboard_subscribe_int(&irq_set_keyboard)) return 1;
    
    while (scancode != BREAK_ESC){
        if (driver_receive(ANY, &msg, &ipc_status)) return 1;
        if (is_ipc_notify(ipc_status)){
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_keyboard) kbc_ih();
                break;
            }
        }
    }

    return keyboard_unsubscribe_int();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
    if (set_frame_buffer(mode)) return 1;
    if (set_graphic_mode(mode)) return 1;
    if (vg_draw_rectangle(x, y, width, height, normalize_color(color))) { vg_exit(); return 1;}
    if (wait_ESC()) { vg_exit(); return 1; }
    if (vg_exit()) return 1;
    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    if (no_rectangles == 0) return 1;
    if (set_frame_buffer(mode)) return 1;
    if (set_graphic_mode(mode)) return 1;
    first = normalize_color(first);
    const uint16_t SCREEN_WIDTH = getWidth();
    const uint16_t SCREEN_HEIGHT = getHeight();
    const uint16_t width = SCREEN_WIDTH / no_rectangles;
    const uint16_t height = SCREEN_HEIGHT / no_rectangles;
    for (uint8_t col = 0; (col + 1) * width <= SCREEN_WIDTH; col++){
        for (uint8_t row = 0; (row + 1) * height <= SCREEN_HEIGHT; row++) {
            uint32_t color;
            if (mode == VBE_768p_INDEXED) {
                color = indexed_mode(col, row, step, first, no_rectangles);
            } else {
                uint32_t red, green, blue;
                red   = Red(col, step, first);
                green = Green(row, step, first);
                blue  = Blue(col, row, step, first);
                color = direct_mode(red, green, blue);
            }
            if (vg_draw_rectangle(col * width, row * height, width, height, color)) { vg_exit(); return 1;}
        }
    }
    if (wait_ESC()) { vg_exit(); return 1; }
    if (vg_exit()) return 1;
    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    if (set_frame_buffer(VBE_768p_INDEXED)) return 1;
    if (set_graphic_mode(VBE_768p_INDEXED)) return 1;
    if (print_xpm(xpm, x, y)) { vg_exit(); return 1;}
    if (wait_ESC()) { vg_exit(); return 1; }
    if (vg_exit()) return 1;
    return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
    uint8_t irq_set_keyboard;
    uint8_t irq_set_timer;
    int ipc_status;
    message msg;

    enum {up, down, left, right} direction;
    if (xi == xf) direction = yi < yf ? down : up;
    else if (yi == yf) direction = xi < xf ? right : left;
    else return 1;

    enum {perFrame, frames} movementType;
    uint16_t movementAmount;
    if (speed > 0) { movementType = perFrame; movementAmount = speed; }
    else if (speed < 0) { movementType = frames; movementAmount = -speed; }
    else return 1;


    if (timer_subscribe_int(&irq_set_timer)) return 1;
    if (keyboard_subscribe_int(&irq_set_keyboard)) return 1;
    if (timer_set_frequency(0, fr_rate)) return 1;
    
    if (set_frame_buffer(VBE_768p_INDEXED)) return 1;
    if (set_graphic_mode(VBE_768p_INDEXED)) return 1;
    
    // First Frame
    if (print_xpm(xpm, xi, yi)) { vg_exit(); return 1; }
    uint8_t frameCount = 0;

    while (scancode != BREAK_ESC){
        if (driver_receive(ANY, &msg, &ipc_status)) { vg_exit(); return 1; }
        if (is_ipc_notify(ipc_status)){
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_keyboard) kbc_ih();
                if (msg.m_notify.interrupts & irq_set_timer){
                    // Erase Last Image
                    if (vg_draw_rectangle(xi, yi, 100, 100, 0)) { vg_exit(); return 1; }
                    // Update Values
                    switch (movementType) {
                    case perFrame:
                        switch (direction) {
                        case up:    yi -= movementAmount; if (yi < yf) yi = yf; break;
                        case down:  yi += movementAmount; if (yi > yf) yi = yf; break;
                        case left:  xi -= movementAmount; if (xi < xf) xi = xf; break;
                        case right: xi += movementAmount; if (xi > xf) xi = xf; break;
                        } break;
                    case frames:
                        if (++frameCount == movementAmount) {
                            frameCount = 0;
                            switch (direction) {
                            case up:    if (--yi < yf) yi = yf; break;
                            case down:  if (++yi > yf) yi = yf; break;
                            case left:  if (--xi < xf) xi = xf; break;
                            case right: if (++xi > xf) xi = xf; break;
                            }
                        }
                    }
                    // Draw Image
                    if (print_xpm(xpm, xi, yi)) { vg_exit(); return 1; }
                }
            }
        }
    }

    if (vg_exit()) return 1;
    if (keyboard_unsubscribe_int()) return 1;
    if (timer_unsubscribe_int()) return 1;
    return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
