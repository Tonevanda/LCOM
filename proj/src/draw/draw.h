#ifndef _DRAW_H_
#define _DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/graphic/graphic.h"
#include "controller/mouse/mouse.h"
#include "model/sprite.h"

//#include "controller/rtc/rtc.h"



void swap_buffers();
int set_frame_buffers(uint16_t mode);
int draw_sprite_xpm(Sprite *sprite, int x, int y);
void draw_title_screen();
void draw_mouse();
void draw_x();
void draw_title_selection();
void draw_game_screen();
int prepare_backround(Sprite *sprite, int x, int y,uint8_t* buffer);
#endif
