#ifndef _PROJ_TIMER_H
#define _PROJ_TIMER_H

#include <stdbool.h>
#include <stdint.h>

uint8_t irqSetTimer;

int (timer_subscribe_interrupt)();
uint64_t(timer_get_elapsed_count)();

#endif // _PROJ_TIMER_H
