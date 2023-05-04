#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/graphic/graphic.h"
//#include "controller/rtc/rtc.h"
#include "draw/draw.h"
#include "config.h"

typedef enum {
    RUNNING,
    EXIT,
} SystemState;

void setup_sprites();
void destroy_sprites();
void update_timer_state();
void update_mouse_state();

#endif
