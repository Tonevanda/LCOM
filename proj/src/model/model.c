#include "model.h"

extern uint8_t scancode;
extern uint8_t byteIndex;
SystemState systemState = RUNNING;
State state = Title;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern int selected;
int timer_interrupts = 0;



Sprite *plus;
Sprite *mouse;
Sprite *XIco;
Sprite *t;
Sprite *player1;
Sprite *player2;


void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    plus = create_sprite_xpm((xpm_map_t) plus_xpm);
    XIco = create_sprite_xpm((xpm_map_t) X);
    t = create_sprite_xpm((xpm_map_t) title);
    player1 = create_sprite_xpm((xpm_map_t) play1);
    player2 = create_sprite_xpm((xpm_map_t) play2);
}

void setup_backround(){
    prepare_backround(XIco, 0, 0);
    prepare_backround(t, 300, 300);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(plus);
    destroy_sprite(XIco);
    destroy_sprite(t);
    destroy_sprite(player1);
    destroy_sprite(player2);
}

void update_timer_state() {
    //setup_backround();
    draw_test();
    if (DOUBLE_BUFFER) swap_buffers();
    timer_interrupts++;
    //debug_timer();
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byteIndex == 3) {
        mouse_sync_mouseInfo();
        //draw_new_frame();
        switch (state)
        {
        case Title:
            update_mouse_actions_title();
            break;
        case Player:
            //update_mouse_actions_player();
            break;
        case AI:
            //update_mouse_actions_AI();
            break;        
        default:
            break;
        }
        byteIndex = 0;
    }
}

void update_mouse_actions_title() {
    if(mouse_info.x>300 && mouse_info.x<459){
        if(mouse_info.y>300 && mouse_info.y<336){
        selected=1;
        }
        if(mouse_info.y>336 && mouse_info.y<372){
        selected=2;
        }
    }
    else if(mouse_info.x<45 && mouse_info.y<45){
        //systemState = EXIT; 
        selected=3;
        //printf("exit");  
    }
    else{
        selected=0;
    }
    if(mouse_info.left_click){
        switch (selected)
        {
        case 3:
            systemState = EXIT;
            break;
        case 1:
            printf("player1 state");
            state=Player;
            break;
        case 2:
            printf("player2 state");
            break;
        }
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
