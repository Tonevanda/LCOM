#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC_Keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphic/graphic.h"
//#include "controller/rtc/rtc.h"

#include "draw/draw.h"
#include "model/sprite.h"
#include "config.h"

#include "xpms/plus.xpm"
#include "xpms/mouse.xpm"
#include "xpms/x.xpm"
#include "xpms/title.xpm"
#include "xpms/player1.xpm"
#include "xpms/player2.xpm"

typedef enum {
    RUNNING,
    EXIT,
}SystemState;

typedef enum{
  Title,
  Player,
  AI,
}State;

void setup_sprites();
void destroy_sprites();
void destroy_sprites();
void update_timer_state();
void update_mouse_state();
void update_keyboard_state();
void debug_timer();
void setup_backround();
void update_mouse_actions_title();
#endif
