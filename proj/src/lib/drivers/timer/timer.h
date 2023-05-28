#ifndef _PROJ_TIMER_H
#define _PROJ_TIMER_H

#include <stdbool.h>
#include <stdint.h>

uint8_t irqSetTimer;

/**
 * @brief Subscribes to Timer interrupts.
 *
 * This function subscribes to Timer interrupts by using `sys_irqsetpolicy` function.
 * 
 * @return Non-zero if error occured.
 * */
int (timer_subscribe_interrupt)();
/**
 * @brief Gets the elapsed count of the Timer.
 *
 * @return The elapsed count of the Timer.
 */
uint64_t (timer_get_elapsed_count)();

#endif // _PROJ_TIMER_H
