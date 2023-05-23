#ifndef _keyboard_h_
#define _keyboard_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC_Keyboard.h"

#define KEYBOARD_INTERRUPT BIT(1)

int (keyboard_subscribe_interrupts)();

int (keyboard_unsubscribe_interrupts)();

void (kbc_ih)();

int (keyboard_restore)();

#endif
