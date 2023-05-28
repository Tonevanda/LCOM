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



//boats
#include "xpms/boat_down_vertical.xpm"
#include "xpms/boat_left_horizontal.xpm"
#include "xpms/boat_middle_horizontal.xpm"
#include "xpms/boat_middle_vertical.xpm"
#include "xpms/boat_right_horizontal.xpm"
#include "xpms/boat_up_vertical.xpm"

//backrounds/complex xpms
#include "xpms/board.xpm"
#include "xpms/backround.xpm"
#include "xpms/win.xpm"
#include "xpms/auga.xpm"

//icons
#include "xpms/mouse.xpm"
#include "xpms/x.xpm"
#include "xpms/arrow.xpm"
#include "xpms/probed.xpm"
#include "xpms/white-x.xpm"
#include "xpms/red-x.xpm"
#include "xpms/colon.xpm"
#include "xpms/frame.xpm"

//numbers
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

//words
#include "xpms/word-Title.xpm"
#include "xpms/word-opp.xpm"
#include "xpms/word-your.xpm"
#include "xpms/word-board.xpm"
#include "xpms/word-start.xpm"
#include "xpms/word-start_selected.xpm"
#include "xpms/word-exit.xpm"
#include "xpms/word-exit_selected.xpm"

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
