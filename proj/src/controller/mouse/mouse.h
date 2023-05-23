#ifndef _mouse_h_
#define _mouse_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC_Mouse.h"

#define MOUSE_INTERRUPT BIT(2)

typedef struct {
    uint8_t right_click;
    uint8_t left_click;
    int16_t x;
    int16_t y;
} MouseInfo;   

int (mouse_subscribe_int)();

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (mouse_sync_bytes)();

void (mouse_sync_mouseInfo)();

int (mouse_write)(uint8_t command);

#endif
