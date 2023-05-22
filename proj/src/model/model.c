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
extern struct slot enemy_board[66];
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
Sprite *boat_down;
Sprite *boat_up;
Sprite *boat_middle_vert;
Sprite *boat_left;
Sprite *boat_right;
Sprite *boat_middle_hor;

int x=0;
int y=0;
int doubles=4;
int triples=3;
int quads=2;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    plus = create_sprite_xpm((xpm_map_t) plus_xpm);
    XIco = create_sprite_xpm((xpm_map_t) X);
    t = create_sprite_xpm((xpm_map_t) title);
    player1 = create_sprite_xpm((xpm_map_t) play1);
    player2 = create_sprite_xpm((xpm_map_t) play2);
    board = create_sprite_xpm((xpm_map_t) bo);
    frame = create_sprite_xpm((xpm_map_t) fra);
    boat_down = create_sprite_xpm((xpm_map_t) down);
    boat_up = create_sprite_xpm((xpm_map_t) up);
    boat_middle_vert = create_sprite_xpm((xpm_map_t) middle_v);
    boat_left = create_sprite_xpm((xpm_map_t) left);
    boat_right = create_sprite_xpm((xpm_map_t) right);
    boat_middle_hor = create_sprite_xpm((xpm_map_t) middle_h);
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
    destroy_sprite(boat_down);
    destroy_sprite(boat_up);
    destroy_sprite(boat_middle_vert);
    destroy_sprite(boat_left);
    destroy_sprite(boat_right);
    destroy_sprite(boat_middle_hor);
}

void update_timer_state() {
    setup_backround();
    switch (state)
    {
    case Title:
        draw_title_screen();
        break;
    case Placement:
        draw_game_screen();
        break;  
    case Atack:
        draw_enemy();
        break;
    case Defend:
        draw_defence();
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
        case Placement:
            getBoardPos();
            update_mouse_actions_player();
            break;
        case Atack:
            getBoardPos();
            update_mouse_actions_Attack();
            break;
        case Defend:
            update_mouse_actions_defend();
            break;      
        }
        byteIndex=0;
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
            state=Placement;
            break;
        case 2:
            printf("player2 state");
            break;
        }
    }
}

void getBoardPos(){
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
}

void update_mouse_actions_player() {
    if(mouse_info.left_click){
        //printf("x: %d | y: %d",mouse_info.x,mouse_info.y);
       
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }

        if(mouse_info.x>100 && mouse_info.x<110 && mouse_info.y>100 && mouse_info.y<150){
            if(mouse_info.y<110 && doubles!=0){
                //double
                current_boat=2;
                printf("Selected double");
            }
            else if(mouse_info.y>120 && mouse_info.y<130 && triples!=0){
                current_boat=3;
                printf("Selected triple");
                //triple
            }
            else if(mouse_info.y>140 && quads!=0){
                current_boat=4;
                printf("Selected quad");
                //quad
            }
            else{
                current_boat=0;
            }
        }
        else{
            //printf(" board_index: %d | current_boat: %d ",board_index,current_boat);
            printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
            if(board_index!=0 && !player_board[board_index].hasBoat){
                switch (current_boat)
                {
                case 2:
                    if(vert){
                        if(y<8 && !player_board[board_index+8].hasBoat){
                            printf(" placed double ");
                            addBoat(board_index,4);
                            addBoat(board_index+8,6);
                            doubles--;
                        }
                    }
                    else{
                        if(x<8 && !player_board[board_index+1].hasBoat){
                            printf(" placed double ");
                            addBoat(board_index,1);
                            addBoat(board_index+1,3);
                            doubles--;
                        }
                    }
                    if(doubles==0){
                        current_boat=0;
                    }
                    break;
                case 3:
                    if(vert){
                        if(y<7 && !player_board[board_index+8].hasBoat && !player_board[board_index+16].hasBoat){
                            printf(" placed triple ");
                            addBoat(board_index,4);
                            addBoat(board_index+8,5);
                            addBoat(board_index+16,6);
                            triples--;
                        }
                    }
                    else{
                        if(x<7 && !player_board[board_index+1].hasBoat && !player_board[board_index+2].hasBoat){
                            printf(" placed triple ");
                            addBoat(board_index,1);
                            addBoat(board_index+1,2);
                            addBoat(board_index+2,3);
                            triples--;
                        }
                    }
                    if(triples==0){
                        current_boat=0;
                    }
                    break;
                case 4:
                    if(vert){
                        if(y<6 && !player_board[board_index+8].hasBoat && !player_board[board_index+16].hasBoat && !player_board[board_index+24].hasBoat){
                            printf(" placed quad ");
                            addBoat(board_index,4);
                            addBoat(board_index+8,5);
                            addBoat(board_index+16,5);
                            addBoat(board_index+24,6);
                            quads--;
                        }
                    }
                    else{
                        if(x<6 && !player_board[board_index+1].hasBoat && !player_board[board_index+2].hasBoat && !player_board[board_index+3].hasBoat){
                            printf(" placed quad ");
                            addBoat(board_index,1);
                            addBoat(board_index+1,2);
                            addBoat(board_index+2,2);
                            addBoat(board_index+3,3);
                            quads--;
                        }
                    }
                    if(quads==0){
                        current_boat=0;
                    }
                    break;
                default:
                    break;
                }
            }
            if(quads==0 && triples==0 && doubles==0){
                quads=2;
                triples=3;
                doubles=4;
                aiBoats();
                state=Atack;
                printf("                           test                         ");
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

void update_mouse_actions_Attack(){
    //printf("nuts");
    if(mouse_info.left_click){
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }
        //printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
        if(board_index!=0){
            attack(enemy_board);
        }
    }
}

void update_mouse_actions_defend(){
    if(mouse_info.left_click){
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Atack;
        }
        //printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
    }
}

void attack(struct slot atackee[66]){
    if(!atackee[board_index].probed){
        atackee[board_index].probed=true;
        if(atackee[board_index].hasBoat){
            printf("found %d ",atackee[board_index].len);
            switch (atackee[board_index].len)
            {
            case 2:
                switch (atackee[board_index].pos)
                {
                case 1:
                    if(atackee[board_index+1].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                    }
                    break;
                case 3:
                    if(atackee[board_index-1].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index-1].sinked=true;
                    }
                    break;
                case 4:
                    if(atackee[board_index+8].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                    }
                    break;
                case 6:
                    if(atackee[board_index-8].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index-8].sinked=true;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 3:
                switch (atackee[board_index].pos)
                {
                case 1:
                    if(atackee[board_index+1].probed && atackee[board_index+2].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                        atackee[board_index+2].sinked=true;
                    }
                    break;
                case 2:
                    if(atackee[board_index+1].probed && atackee[board_index-1].probed){
                        printf("sinked");
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                    }
                    break;
                case 3:
                    if(atackee[board_index-1].probed && atackee[board_index-2].probed){
                        printf("sinked");
                        atackee[board_index-2].sinked=true;
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        
                    }
                    break;
                case 4:
                    if(atackee[board_index+8].probed && atackee[board_index+16].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                        atackee[board_index+16].sinked=true;
                    }
                    break;
                case 5:
                    if(atackee[board_index+8].probed && atackee[board_index-8].probed){
                        printf("sinked");
                        atackee[board_index-8].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                    }
                    break;
                case 6:
                    if(atackee[board_index-8].probed && atackee[board_index-16].probed){
                        printf("sinked");
                        atackee[board_index-16].sinked=true;
                        atackee[board_index-8].sinked=true;
                        atackee[board_index].sinked=true;
                        
                    }
                    break;
                default:
                    break;
                }
                break;
            case 4:
                switch (atackee[board_index].pos)
                {
                case 1:
                    if(atackee[board_index+1].probed && atackee[board_index+2].probed && atackee[board_index+3].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                        atackee[board_index+2].sinked=true;
                        atackee[board_index+3].sinked=true;
                    }
                    break;
                case 2:
                    if(atackee[board_index+1].probed && atackee[board_index-1].probed){
                        if(atackee[board_index-1].pos==1 && atackee[board_index+2].probed){
                            atackee[board_index+2].sinked=true;
                        }
                        else if (atackee[board_index+1].pos==3 && atackee[board_index-2].probed){
                            atackee[board_index-2].sinked=true;
                        }
                        
                        printf("sinked");
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                    }
                    break;
                case 3:
                    if(atackee[board_index-1].probed && atackee[board_index-2].probed && atackee[board_index-3].probed){
                        printf("sinked");
                        atackee[board_index-3].sinked=true;
                        atackee[board_index-2].sinked=true;
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        
                    }
                    break;
                case 4:
                    if(atackee[board_index+8].probed && atackee[board_index+16].probed && atackee[board_index+24].probed){
                        printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                        atackee[board_index+16].sinked=true;
                        atackee[board_index+24].sinked=true;
                    }
                    break;
                case 5:
                    if(atackee[board_index+8].probed && atackee[board_index-8].probed){
                        if(atackee[board_index-8].pos==4 && atackee[board_index+24].probed){
                            atackee[board_index+24].sinked=true;
                        }
                        else if (atackee[board_index+8].pos==3 && atackee[board_index-24].probed){
                            atackee[board_index-24].sinked=true;
                        }
                        printf("sinked");
                        atackee[board_index-8].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                    }
                    break;
                case 6:
                    if(atackee[board_index-8].probed && atackee[board_index-16].probed && atackee[board_index-24].probed){
                        printf("sinked");
                        atackee[board_index-24].sinked=true;
                        atackee[board_index-16].sinked=true;
                        atackee[board_index-8].sinked=true;
                        atackee[board_index].sinked=true;
            
                    }
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        else {
            if(state==Atack){
                printf("             ai time now                      ");
                state=Defend;
                /*
                y=(rand() % (5 - 1 + 1)) + 1;
                x=(rand() % (8 - 1 + 1)) + 1;
                board_index=((y-1)*8)+x;
                */
                do
                {
                    do
                    {
                        y=(rand() % (5 - 1 + 1)) + 1;
                        x=(rand() % (8 - 1 + 1)) + 1;
                        board_index=((y-1)*8)+x;
                        printf(" board_index: %d",board_index);
                    } while (player_board[board_index].probed);
                    attack(player_board);
                } while (player_board[board_index].hasBoat);
            }
        }
    }
}

void aiBoats(){
    printf("                                                   ai turn:             ");
    srand(time(NULL));
    current_boat=4;
    while(quads!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (5 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat && !enemy_board[board_index+16].hasBoat && !enemy_board[board_index+24].hasBoat){
                printf(" placed quad ");
                addEnemyBoat(board_index,4);
                addEnemyBoat(board_index+8,5);
                addEnemyBoat(board_index+16,5);
                addEnemyBoat(board_index+24,6);
                quads--;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (5 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat && !enemy_board[board_index+2].hasBoat && !enemy_board[board_index+3].hasBoat){
                printf(" placed quad ");
                addEnemyBoat(board_index,1);
                addEnemyBoat(board_index+1,2);
                addEnemyBoat(board_index+2,2);
                addEnemyBoat(board_index+3,3);
                quads--;
            }
        }
    }
    current_boat=3;
    while(triples!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (6 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat && !enemy_board[board_index+16].hasBoat){
                printf(" placed triple ");
                addEnemyBoat(board_index,4);
                addEnemyBoat(board_index+8,5);
                addEnemyBoat(board_index+16,6);
                triples--;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (6 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat && !enemy_board[board_index+2].hasBoat){
                printf(" placed triple ");
                addEnemyBoat(board_index,1);
                addEnemyBoat(board_index+1,2);
                addEnemyBoat(board_index+2,3);
                triples--;
            }
            if(doubles==0){
                current_boat=0;
            }
        }
    }
    current_boat=2;
    while(doubles!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (7 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat){
                printf(" placed double ");
                addEnemyBoat(board_index,4);
                addEnemyBoat(board_index+8,6);
                doubles--;
            }
            if(doubles==0){
                current_boat=0;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (7 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat){
                printf(" placed double ");
                addEnemyBoat(board_index,1);
                addEnemyBoat(board_index+1,3);
                doubles--;
            }
            if(doubles==0){
                current_boat=0;
            }
        }
    }
}

void addBoat(int board_index,int pos){
    player_board[board_index].hasBoat=true;
    player_board[board_index].probed=false;
    player_board[board_index].sinked=false;
    player_board[board_index].len=current_boat;
    player_board[board_index].pos=pos;
}
void addEnemyBoat(int board_index,int pos){
    enemy_board[board_index].hasBoat=true;
    enemy_board[board_index].probed=false;
    enemy_board[board_index].sinked=false;
    enemy_board[board_index].len=current_boat;
    enemy_board[board_index].pos=pos;
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
