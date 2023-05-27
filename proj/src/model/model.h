#ifndef _MODEL_H_
#define _MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC_Keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphic/graphic.h"
#include "controller/rtc/rtc.h"

#include "draw/draw.h"
#include "model/sprite.h"
#include "config.h"

#include "xpms/plus.xpm"
#include "xpms/mouse.xpm"
#include "xpms/x.xpm"
#include "xpms/title.xpm"
#include "xpms/player1.xpm"
#include "xpms/player2.xpm"
#include "xpms/board.xpm"
#include "xpms/frame.xpm"
#include "xpms/down_vertical_boat.xpm"
#include "xpms/left_horizontal_boat.xpm"
#include "xpms/middle_horizontal_boat.xpm"
#include "xpms/middle_vertical_boat.xpm"
#include "xpms/right_horizontal_boat.xpm"
#include "xpms/up_vertical_boat.xpm"
#include "xpms/arrow.xpm"
#include "xpms/backround.xpm"
#include "xpms/titlecard.xpm"
#include "xpms/probed.xpm"
#include "xpms/white-x.xpm"
#include "xpms/red-x.xpm"
#include "xpms/win.xpm"
//FALTA ADICIONAR XPMS DOS NUMEROS PARA RTC
#include "xpms/n0.xpm"
#include "xpms/n1.xpm"
#include "xpms/n2.xpm"
#include "xpms/n3.xpm"
#include "xpms/n4.xpm"
#include "xpms/n5.xpm"
#include "xpms/n6.xpm"
#include "xpms/n7.xpm"
#include "xpms/n8.xpm"
#include "xpms/n9.xpm"
#include "xpms/colon.xpm"
#include "xpms/auga.xpm"
#include "xpms/opp.xpm"
#include "xpms/your.xpm"
#include "xpms/boardn.xpm"
#include "xpms/start.xpm"
#include "xpms/select.xpm"
#include "xpms/exit.xpm"
#include "xpms/exit_selected.xpm"

typedef enum {
  RUNNING,
  EXIT,
}SystemState;

typedef enum{
  Title,
  Placement,
  Defend,
  Attack,
  Victory,
}State;

void setup_sprites();
void destroy_sprites();
void setup_backround();

void update_timer_state();
void update_mouse_state();
void update_keyboard_state();


void update_mouse_actions_title();
void update_mouse_actions_placement();
void update_mouse_actions_Attack();
void update_mouse_actions_defend();

void update_keyboard_actions_title();
void update_keyboard_actions_placement();
void update_keyboard_actions_attack();
void update_keyboard_actions_defend();


void menu_actions();
void getBoardPos();
void getKeyboadPos();

void placeBoat();

void addBoat(struct slot board[66],int board_index,enum part);

void attack(struct slot atackee[66]);
void aiBoats();

void ai_actions();
#endif
