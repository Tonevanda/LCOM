#ifndef _timer_h_
#define _timer_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

#define TIMER_INTERRUPT BIT(0)

/**
 * @brief Sets the timer frequency
 * @param timer Represents the desired timer to configure
 * @param freq Representes the desired frequency
 * @return 0 if successful
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes the interrupts for the timer
 * @return 0 if successful
 */
int (timer_subscribe_interrupts)();

/**
 * @brief Unsubscribes the interrupts for the timer
 * @return 0 if successful
 */
int (timer_unsubscribe_interrupts)();

/**
 * @brief Reads the configuration of the timer and stores it in the st parameter
 * @param timer Represents which timer to read the configuration from
 * @param st Represents the variable which will store the configuration read from the timer 
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
