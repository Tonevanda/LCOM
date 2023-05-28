#ifndef _graphic_h
#define _graphic_h

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t mode_info; 

/**
 * @brief Sets graphic mode
 * @param submode hex value of the graphic mode
 * @return 0 if successful
 */
int (set_graphic_mode)(uint16_t submode);

/**
 * @brief Creates the main frame buffer and allocates VRAM to it
 * @param mode hex value of the graphic mode
 * @param frame_buffer This buffer will be assigned to physical memory in this method.
 * @return 0 if successful
 */
int (set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer);

/**
 * @brief Draws a pixel of a certain color onto a frame buffer
 * @param x Pixel X coordinate 
 * @param y Pixel Y coordinate
 * @param color Color hex value. Will be interpreted differently depending on the color encoding
 * @param frame_buffer The frame buffer that will be drawn onto
 * @return 0 if successful
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);

/**
 * @brief Draws a horizontal line of a certain color onto a frame buffer
 * @param x Leftmost pixel X coordinate
 * @param y Line Y coordinate
 * @param color Color hex value. Will be interpreted differently depending on the color encoding
 * @param frame_buffer The frame buffer that will be drawn onto
 * @return 0 if successful
 */
int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color,uint8_t* frame_buffer);

/**
 * @brief Draws a rectangle of a certain color onto a frame buffer
 * @param x Leftmost X pixel
 * @param y Top Y pixel
 * @param color Color hex value. Will be interpreted differently depending on the color encoding
 * @param frame_buffer The frame buffer that will be drawn onto
 * @return 0 if successful
 * 
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer);

/**
 * @brief Draws a xpm onto a frame buffer
 * @param xpm The xpm to draw
 * @param x Leftmost X pixel
 * @param y Top Y pixel
 * @param frame_buffer The frame buffer that will be drawn onto
 * @return 0 if successful
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y,uint8_t *buffer);

#endif
