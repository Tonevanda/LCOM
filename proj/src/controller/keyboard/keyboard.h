#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

#define KEYBOARD_MASK BIT(1)

int (keyboard_subscribe_interrupts)();

int (keyboard_unsubscribe_interrupts)();

void (kbc_ih)();

int (keyboard_restore)();

#endif
