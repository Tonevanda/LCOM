#include "model.h"

extern uint8_t scancode;
extern uint8_t byteIndex;
SystemState systemState = RUNNING;
State state = Title;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern int selected;
int timer_interrupts = 0;
extern int original_board_x; 
extern int original_board_y;
extern int board_index;
extern uint8_t *title_screen_backround_buffer;
extern uint8_t *game_board_backround_buffer;
extern struct slot player_board[66];
extern int current_boat;
extern bool vert;

Sprite *plus;
Sprite *mouse;
Sprite *XIco;
Sprite *t;
Sprite *player1;
Sprite *player2;
Sprite *board;
Sprite *frame;
int x=0;
int y=0;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    plus = create_sprite_xpm((xpm_map_t) plus_xpm);
    XIco = create_sprite_xpm((xpm_map_t) X);
    t = create_sprite_xpm((xpm_map_t) title);
    player1 = create_sprite_xpm((xpm_map_t) play1);
    player2 = create_sprite_xpm((xpm_map_t) play2);
    board = create_sprite_xpm((xpm_map_t) bo);
    frame = create_sprite_xpm((xpm_map_t) fra);
}

void setup_backround(){
    prepare_backround(XIco, 0, 0,title_screen_backround_buffer);
    prepare_backround(t, 300, 300,title_screen_backround_buffer);
    draw_rectangle(0,0,mode_info.XResolution,mode_info.YResolution,0x0000FF,game_board_backround_buffer);
    prepare_backround(XIco, 0, 0,game_board_backround_buffer);
    prepare_backround(board, 350, 220,game_board_backround_buffer);
    draw_rectangle(100,100,10,10,0xFF0000,game_board_backround_buffer);
    draw_rectangle(100,120,10,10,0xFF0000,game_board_backround_buffer);
    draw_rectangle(100,140,10,10,0xFF0000,game_board_backround_buffer);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(plus);
    destroy_sprite(XIco);
    destroy_sprite(t);
    destroy_sprite(player1);
    destroy_sprite(player2);
    destroy_sprite(board);
    destroy_sprite(frame);
}

void update_timer_state() {
    //setup_backround();
    switch (state)
    {
    case Title:
        draw_title_screen();
        break;
    case Player:
        draw_game_screen();
        break;
    case AI:
        //update_mouse_actions_AI();
        break;        
    default:
        break;
    }
    swap_buffers();
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
            update_mouse_actions_player();
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

void update_mouse_actions_player() {
    if(mouse_info.x>402 && mouse_info.x<801 && mouse_info.y>272 && mouse_info.y<671){
        //322
        if(mouse_info.y<original_board_y+50){
            y=1;
        }
        else if (mouse_info.y<original_board_y+100){
            y=2;
        }
        else if (mouse_info.y<original_board_y+150){
            y=3;
        }
        else if (mouse_info.y<original_board_y+200){
            y=4;
        }
        else if (mouse_info.y<original_board_y+250){
            y=5;
        }
        else if (mouse_info.y<original_board_y+300){
            y=6;
        }
        else if (mouse_info.y<original_board_y+350){
            y=7;
        }
        else if (mouse_info.y<original_board_y+400){
            y=8;
        }
        if(mouse_info.x<original_board_x+50){
            x=1;
        }
        else if (mouse_info.x<original_board_x+100){
            x=2;
        }
        else if (mouse_info.x<original_board_x+150){
            x=3;
        }
        else if (mouse_info.x<original_board_x+200){
            x=4;
        }
        else if (mouse_info.x<original_board_x+250){
            x=5;
        }
        else if (mouse_info.x<original_board_x+300){
            x=6;
        }
        else if (mouse_info.x<original_board_x+350){
            x=7;
        }
        else if (mouse_info.x<original_board_x+400){
            x=8;
        }
        board_index=((y-1)*8)+x;
    }
    else{
        board_index=0;
    }
    if(mouse_info.left_click){
        //printf("x: %d | y: %d",mouse_info.x,mouse_info.y);
       
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }

        if(mouse_info.x>100 && mouse_info.x<110 && mouse_info.y>100 && mouse_info.y<150){
            if(mouse_info.y<110){
                //double
                current_boat=2;
                printf("Selected double");
            }
            else if(mouse_info.y>120 && mouse_info.y<130){
                current_boat=3;
                printf("Selected triple");
                //triple
            }
            else if(mouse_info.y>140){
                current_boat=4;
                printf("Selected quad");
                //quad
            }
            else{
                current_boat=0;
            }
        }
        else{
            printf(" board_index: %d | current_boat: %d ",board_index,current_boat);
            if(board_index!=0){
                switch (current_boat)
                {
                case 2:
                    if(vert){
                        if(y<8){
                            printf("placed double");
                            player_board[board_index].hasBoat=true;
                            player_board[board_index].probed=false;
                            player_board[board_index].len=current_boat;
                            player_board[board_index].vert=vert;
                            player_board[board_index+8].hasBoat=true;
                            player_board[board_index+8].probed=false;
                            player_board[board_index+8].len=current_boat;
                            player_board[board_index+8].vert=vert;
                        }
                    }
                    else{
                        if(x<8){
                            printf("placed double");
                            player_board[board_index].hasBoat=true;
                            player_board[board_index].probed=false;
                            player_board[board_index].len=current_boat;
                            player_board[board_index].vert=vert;
                            player_board[board_index+1].hasBoat=true;
                            player_board[board_index+1].probed=false;
                            player_board[board_index+1].len=current_boat;
                            player_board[board_index+1].vert=vert;
                        }
                    }
                    
                    break;
                case 3:
                    if(vert){
                        if(y<7){
                            printf("placed double");
                            player_board[board_index].hasBoat=true;
                            player_board[board_index].probed=false;
                            player_board[board_index].len=current_boat;
                            player_board[board_index].vert=vert;
                            player_board[board_index+8].hasBoat=true;
                            player_board[board_index+8].probed=false;
                            player_board[board_index+8].len=current_boat;
                            player_board[board_index+8].vert=vert;
                            player_board[board_index+16].hasBoat=true;
                            player_board[board_index+16].probed=false;
                            player_board[board_index+16].len=current_boat;
                            player_board[board_index+16].vert=vert;
                        }
                    }
                    else{
                        if(x<7){
                            printf("placed double");
                            player_board[board_index].hasBoat=true;
                            player_board[board_index].probed=false;
                            player_board[board_index].len=current_boat;
                            player_board[board_index].vert=vert;
                            player_board[board_index+1].hasBoat=true;
                            player_board[board_index+1].probed=false;
                            player_board[board_index+1].len=current_boat;
                            player_board[board_index+1].vert=vert;
                            player_board[board_index+2].hasBoat=true;
                            player_board[board_index+2].probed=false;
                            player_board[board_index+2].len=current_boat;
                            player_board[board_index+2].vert=vert;
                        }
                    }
                    break;
                case 4:
                    /* code */
                    break;
                default:
                    break;
                }
            }
            
            
        }        
    }
    if(mouse_info.right_click){
        if (vert){
            vert=false;
        }
        else{
            vert=true;
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
