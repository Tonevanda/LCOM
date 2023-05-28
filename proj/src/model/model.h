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
#include "xpms/los.xpm"

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

/**
 * @brief Loads all xpms onto their respective sprite objects 
 */
void setup_sprites();
/**
 * @brief Destroys all sprite objects to clear memory
 */
void destroy_sprites();
/**
 * @brief Draws the static sprites onto their necessary background buffers
 */
void setup_backround();

/**
 * @brief Handles timer interruptions according to the current state. All the draw functions are called from here and also the method for the AI to attack
 */
void update_timer_state();

/**
 * @brief Handles mouse interruptions according to the current state. Calls functions to handle the mouse scancodes sent by the user's mouse
 */
void update_mouse_state();

/**
 * @brief Handles keyboard interruptions according to the current state. Calls functions to handle the keyboard scancodes sent by the user's keyboard
 */
void update_keyboard_state();

/**
 * @brief Handles mouse packets while on the Title state 
 */
void update_mouse_actions_title();
/**
 * @brief Handles mouse packets while on the Placement state 
 */
void update_mouse_actions_placement();
/**
 * @brief Handles mouse packets while on the Attack state 
 */
void update_mouse_actions_attack();
/**
 * @brief Handles mouse packets while on the Defend state 
 */
void update_mouse_actions_defend();

/**
 * @brief Handles mouse scancodes while on the Victory state 
 */
void update_mouse_actions_victory();

/**
 * @brief Handles keyboard scancodes while on the Title state 
 */
void update_keyboard_actions_title();

/**
 * @brief Handles keyboard scancodes while on the Placement state 
 */
void update_keyboard_actions_placement();

/**
 * @brief Handles keyboard scancodes while on the Attack state 
 */
void update_keyboard_actions_attack();

/**
 * @brief Handles keyboard scancodes while on the Defend state 
 */
void update_keyboard_actions_defend();

/**
 * @brief Handles keyboard scancodes while on the Victory state 
 */
void update_keyboard_actions_victory();

/**
 * @brief Chooses main menu options depending on user input
 */
void menu_actions();

/**
 * @brief Updates global board_index variable with mouse inputs
 */
void getBoardPos();

/**
 * @brief Updates global board_index variable with keyboard inputs
 */
void getKeyboadPos();

/**
 * @brief Places boats in the player_board given the current board_index,current_boat and the vert value
 */
void placeBoat();

/**
 * @brief Places all the boats in the enemy_board randomly 
 */
void placeAiBoats();

/**
 * @brief Updates the player_board's and enemy_board's individual slot structs to reflect on placed boats. It's called in the placeBoat and placeAiBoats methods
 * @param board The board where the boat part is added
 * @param board_index The boat index where the boat part is added
 * @param part The part itself to add
 */
void addBoat(struct slot board[66],uint8_t board_index,enum part);

/**
 * @brief Updates the player_board's and enemy_board's individual slot structs to reflect on attacks.
 * @param atackee Board of who is atacked (player_board/enemy_board)
 */
void attack(struct slot atackee[66]);

/**
 * @brief Randomizes board movement and attacks to act as AI
 */
void ai_actions();


#endif
