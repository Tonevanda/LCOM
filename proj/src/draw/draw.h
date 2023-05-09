#ifndef _DRAW_H_
#define _DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/graphic/graphic.h"
#include "controller/mouse/mouse.h"
#include "model/sprite.h"
uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;
//#include "controller/rtc/rtc.h"



void swap_buffers();
int set_frame_buffers(uint16_t mode);
int draw_sprite_xpm(Sprite *sprite, int x, int y);
void draw_test();
void test();
#endif
