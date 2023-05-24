#ifndef _DRAW_H_
#define _DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/graphic/graphic.h"
#include "controller/mouse/mouse.h"
#include "model/sprite.h"

//#include "controller/rtc/rtc.h"


enum part{
    left_p,
    middle_horizontal_p,
    right_p,
    up_p,
    middle_vertical_p,
    down_p
};

struct slot
{
    bool hasBoat;
    bool probed;
    bool sinked;
    int len;
    enum part pos;
    /*
    1:left
    2:middle horizontal
    3:right
    4:up
    5:middle vertical
    6:down
    */
};

struct gameInfo{
    int doublesBoatsLeft;
    int triplesBoatsLeft;
    int quadsBoatsLeft;
    int doublesAiBoatsLeft;
    int triplesAiBoatsLeft;
    int quadsAiBoatsLeft;
};


int set_frame_buffers(uint16_t mode);
void swap_buffers();
int draw_sprite_xpm(Sprite *sprite, int x, int y);
int prepare_backround(Sprite *sprite, int x, int y,uint8_t* buffer);

void draw_title_screen();
void draw_placement();
void draw_attack();
void draw_defend();
void draw_victory();

void draw_title_selection();
void draw_reticle();
void draw_selected();
void draw_mouse();

void draw_player_board_icons();
void draw_AI_board_icons();
void pos_switch(int x, int y, enum part pos);
void draw_on_board(int x,int y,Sprite *sprite);


#endif
