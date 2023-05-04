#ifndef _sprite_h_
#define _sprite_h_

#include "controller/graphic/graphic.h"

typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
    uint32_t color;
    uint8_t pressed;
} Sprite; 

Sprite *create_sprite_xpm(xpm_map_t sprite);
void destroy_sprite(Sprite *sprite);

#endif
