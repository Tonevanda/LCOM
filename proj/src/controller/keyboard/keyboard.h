#ifndef _keyboard_h_
#define _keyboard_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC_Keyboard.h"

#define KEYBOARD_INTERRUPT BIT(1)

/**
 * @brief Subscribes the keyboard interrupts
 * @return 0 if successful
 */
int (keyboard_subscribe_interrupts)();

/**
 * @brief Unsubscribes the keyboard interrupts
 * @return 0 if successful
 */
int (keyboard_unsubscribe_interrupts)();

/**
 * @brief KBC interrupt handler
 */
void (kbc_ih)();

/**
 * @brief Used to restore the keyboard interrupts, in case the keyboard was used in polling mode
 * @return 0 if successful
 */
int (keyboard_restore)();

#endif
