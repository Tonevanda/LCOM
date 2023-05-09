#ifndef _timer_h_
#define _timer_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

#define TIMER_MASK BIT(0)

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

int (timer_subscribe_interrupts)();

int (timer_unsubscribe_interrupts)();

int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
