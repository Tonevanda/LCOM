#include "model.h"

extern uint8_t scancode;
extern uint8_t byteIndex;
SystemState systemState = RUNNING;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;

int timer_interrupts = 0;

Sprite *plus;
Sprite *mouse;
//Sprite *meme;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    plus = create_sprite_xpm((xpm_map_t) plus_xpm);
    //meme = create_sprite_xpm((xpm_map_t) memexd);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(plus);
    //destroy_sprite(meme);
}

void update_timer_state() {
    if (DOUBLE_BUFFER) swap_buffers();
    draw_test();
    timer_interrupts++;
    //debug_timer();
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byteIndex == 3) {
        mouse_sync_mouseInfo();
        //update_buttons_state();
        //draw_new_frame();
        byteIndex = 0;
    }
}

void update_keyboard_state(){
    (kbc_ih)();
    switch (scancode) {
        case Q_KEY:{
            systemState = EXIT;
            break;
        }
        default:
            break;
    }
    //draw_test();
}

void debug_timer(){
    if(timer_interrupts%60==0){
        printf(" %d /n",timer_interrupts/60);
    }
}
