#ifndef _kbc_keyboard_h_
#define _kbc_keyboard_h_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (read_KBC_Keyboard_status)(uint8_t* status);

int (read_KBC_Keyboard_output)(uint8_t port, uint8_t *output);

int (write_KBC_Keyboard_command)(uint8_t port, uint8_t commandByte);

#endif
