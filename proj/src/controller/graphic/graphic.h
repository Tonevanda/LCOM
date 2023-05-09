#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <lcom/lcf.h>

// estrutura de dados que contém informação sobre o modo gráfico
vbe_mode_info_t mode_info; 

int (set_graphic_mode)(uint16_t submode);

uint32_t (pixel_index)(uint16_t x, uint16_t y);

int (set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);
int (draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer);
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer);

int (normalize_color)(uint32_t color, uint32_t *new_color);

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t (R)(uint32_t first);
uint32_t (G)(uint32_t first);
uint32_t (B)(uint32_t first);
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y,uint8_t *buffer);
#endif
