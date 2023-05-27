#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

#define TIMER_FREQ 1193182
#define TIMER0_IRQ 0

#define TIMER_0    0x40
#define TIMER_1    0x41
#define TIMER_2    0x42
#define TIMER_CTRL 0x43

#define TIMER_RB_CMD (BIT(7) | BIT(6))

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

#endif // _LCOM_I8254_H 
