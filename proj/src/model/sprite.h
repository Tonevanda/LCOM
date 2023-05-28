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

/**
 * @brief Loads the xpm passed as parameter and transforms it into a sprite
 * @param sprite Represents the xpm to be transformed into a sprite
 * @return Sprite if successful, NULL otherwise
 */
Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief Frees the space occupied by the sprite passed as parameter
 * @param sprite Represents the sprite to free space
 */
void destroy_sprite(Sprite *sprite);

#endif
