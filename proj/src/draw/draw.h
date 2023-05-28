#ifndef _DRAW_H_
#define _DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include <time.h>
#include "config.h"
#include "controller/graphic/graphic.h"
#include "controller/mouse/mouse.h"
#include "model/sprite.h"


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
};

struct gameInfo{
    int doublesBoatsLeft;
    int triplesBoatsLeft;
    int quadsBoatsLeft;
    int doublesAiBoatsLeft;
    int triplesAiBoatsLeft;
    int quadsAiBoatsLeft;
};

/**
 * @brief Calls the set_frame_buffer method to create de main buffer and allocates memory for all the other buffers
 * @param mode Hex value of the graphics mode
 * @return 0 if successful 
 */
int set_frame_buffers(uint16_t mode);

/**
 * @brief Copies the drawing_frame_buffer contents onto the main_frame_buffer, thus changing what's displayed on screen
 */
void swap_buffers();

/**
 * @brief Given a sprite object, draws its contents onto the drawing_frame_buffer
 * @param sprite Sprite object generated from a xpm
 * @param x Leftmost pixel x coordinate
 * @param y Topmost pixel y coordinate
 * @return 0 if successful
 */
int draw_sprite_xpm(Sprite *sprite, int x, int y);

/**
 * @brief Given a sprite object, draws its contents onto a buffer given in the input. Mostly used to draw onto the other background buffers right at the start and won't be updated after this initial phase
 * @param sprite Sprite object generated from a xpm
 * @param x Leftmost pixel x coordinate
 * @param y Topmost pixel y coordinate
 * @param buffer The buffer that it draws onto
 * @return 0 if successful
 */
int prepare_backround(Sprite *sprite, int x, int y,uint8_t* buffer);

/**
 * @brief Given user input will draw a number sprite onto the drawing_frame_buffer
 * @param number Determines which number sprite to draw 
 * @param x Leftmost pixel x coordinate
 * @param y Topmost pixel y coordinate
 * @return 0 if successful 
 */
int draw_number_xpm(int number, int x, int y);

/**
 * @brief Copies the title_screen_background_buffer onto the drawing_frame_buffer and draws other sprites necessary for the Title state
 */
void draw_title_screen();

/**
 * @brief Copies the game_board_background_buffer onto the drawing_frame_buffer and draws other sprites necessary for the Placement state
 */
void draw_placement();

/**
 * @brief Copies the game_board_background_buffer onto the drawing_frame_buffer and draws other sprites necessary for the Attack state
 */
void draw_attack();

/**
 * @brief Copies the game_board_background_buffer onto the drawing_frame_buffer and draws other sprites necessary for the Defend state
 */
void draw_defend();

/**
 * @brief Copies the game_board_background_buffer onto the drawing_frame_buffer and draws other sprites necessary for the Victory state
 */
void draw_victory();

/**
 * @brief Draws onto the drawing_frame_buffer the highlighted portions of the title screen
 */
void draw_title_selection();

/**
 * @brief Draws a reticle onto the selected board slot
 */
void draw_reticle();

/**
 * @brief Draws the current_boat that follows the mouse position or the selected reticle icon in the case of keyboard input
 */
void draw_selected();

/**
 * @brief Draws the mouse sprite on its current position
 */
void draw_mouse();

/**
 * @brief Draws the current time in the bottom left corner
 */
void draw_rtc();

/**
 * @brief Draws the elapsed time since the start of the round in the bottom of the screen
 */
void draw_round_time();

/**
 * @brief Draws the sprites that need to be visible when the player board is visible
 */
void draw_player_board_icons();
/**
 * @brief Draws the sprites that need to be visible when the Opponent board is visible
 */
void draw_AI_board_icons();
/**
 * @brief Decides which boat sprite to draw depending on the current slot pos enum
 * @param x X slot coordinate starting from the left (value goes from 1 to 8)
 * @param y Y slot coordinate starting from the top (value goes from 1 to 8)
 * @param pos Enum that represents witch boat portion this is 
 */
void pos_switch(int x, int y, enum part pos);

/**
 * @brief Draws on a board slot (determined by the input x and y coordinates) the inputted Sprite. This is only used for icons related to the board and the progression of the game.
 * @param x X slot coordinate starting from the left (value goes from 1 to 8)
 * @param y Y slot coordinate starting from the top (value goes from 1 to 8)
 * @param sprite Sprite that is drawn
 */
void draw_on_board(int x,int y,Sprite *sprite);

#endif
