#include "model.h"
#include <time.h>

extern uint8_t scancode;
extern uint8_t byteIndex;
SystemState systemState = RUNNING;
State state = Title;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern uint8_t selected;
int timer_interrupts = 0;
extern uint16_t original_board_x; 
extern uint16_t original_board_y;
extern uint8_t board_index;
extern uint8_t *title_screen_backround_buffer;
extern uint8_t *game_board_backround_buffer;
extern struct slot player_board[66];
extern struct slot enemy_board[66];
extern struct gameInfo game;
extern uint8_t current_boat;
extern bool vert;
extern bool isKeyboard;
extern bool victory;
extern bool playerBoardVisible;
extern uint8_t rtc_data[6];
extern time_t endTime;
struct tm initialTime;
Sprite *mouse;
Sprite *XIco;
Sprite *board;
Sprite *frame;
Sprite *boat_down;
Sprite *boat_up;
Sprite *boat_middle_vert;
Sprite *boat_left;
Sprite *boat_right;
Sprite *boat_middle_hor;
Sprite *arrow;
Sprite *backround;
Sprite *titleName;
Sprite *ProbedHole;
Sprite *gameBackround;
Sprite *redX;
Sprite *whiteX;
Sprite *vicotory;
Sprite *lose;
Sprite *number0;
Sprite *number1;
Sprite *number2;
Sprite *number3;
Sprite *number4;
Sprite *number5;
Sprite *number6;
Sprite *number7;
Sprite *number8;
Sprite *number9;
Sprite *colon;
Sprite *board_name;
Sprite *your;
Sprite *opponent;
Sprite *start;
Sprite *selected_start;
Sprite *exit_idk;
Sprite *exit_selected;
Sprite* sprites[40];

uint8_t x=0;
uint8_t y=0;
uint8_t doubles=2;
uint8_t triples=2;
uint8_t quads=2;
uint8_t aiAttackTries=0;
int aiCounter=0;
uint8_t lastAttackedBoat=1;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    XIco = create_sprite_xpm((xpm_map_t) X);
    board = create_sprite_xpm((xpm_map_t) bo);
    frame = create_sprite_xpm((xpm_map_t) fra);
    boat_down = create_sprite_xpm((xpm_map_t) down);
    boat_up = create_sprite_xpm((xpm_map_t) up);
    boat_middle_vert = create_sprite_xpm((xpm_map_t) middle_v);
    boat_left = create_sprite_xpm((xpm_map_t) left);
    boat_right = create_sprite_xpm((xpm_map_t) right);
    boat_middle_hor = create_sprite_xpm((xpm_map_t) middle_h);
    arrow = create_sprite_xpm((xpm_map_t) arr);
    backround = create_sprite_xpm((xpm_map_t) boatimg);
    titleName = create_sprite_xpm((xpm_map_t) titleCard);
    ProbedHole = create_sprite_xpm((xpm_map_t) hole);
    gameBackround = create_sprite_xpm((xpm_map_t) auga);
    redX = create_sprite_xpm((xpm_map_t) red-x);
    whiteX = create_sprite_xpm((xpm_map_t) white-x);
    vicotory = create_sprite_xpm((xpm_map_t) wi);
    number0 = create_sprite_xpm((xpm_map_t) n0);
    number1 = create_sprite_xpm((xpm_map_t) n1);
    number2 = create_sprite_xpm((xpm_map_t) n2);
    number3 = create_sprite_xpm((xpm_map_t) n3);
    number4 = create_sprite_xpm((xpm_map_t) n4);
    number5 = create_sprite_xpm((xpm_map_t) n5);
    number6 = create_sprite_xpm((xpm_map_t) n6);
    number7 = create_sprite_xpm((xpm_map_t) n7);
    number8 = create_sprite_xpm((xpm_map_t) n8);
    number9 = create_sprite_xpm((xpm_map_t) n9);
    colon = create_sprite_xpm((xpm_map_t) co);
    board_name = create_sprite_xpm((xpm_map_t) boardn);
    your = create_sprite_xpm((xpm_map_t) you);
    opponent = create_sprite_xpm((xpm_map_t) opp);
    start = create_sprite_xpm((xpm_map_t) star);
    selected_start = create_sprite_xpm((xpm_map_t) selec);
    exit_idk = create_sprite_xpm((xpm_map_t) ext);
    exit_selected = create_sprite_xpm((xpm_map_t) exit_s);
    lose = create_sprite_xpm((xpm_map_t) los);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(XIco);
    destroy_sprite(board);
    destroy_sprite(frame);
    destroy_sprite(boat_down);
    destroy_sprite(boat_up);
    destroy_sprite(boat_middle_vert);
    destroy_sprite(boat_left);
    destroy_sprite(boat_right);
    destroy_sprite(boat_middle_hor);
    destroy_sprite(arrow);
    destroy_sprite(backround);
    destroy_sprite(titleName);
    destroy_sprite(ProbedHole);
    destroy_sprite(gameBackround);
    destroy_sprite(redX);
    destroy_sprite(whiteX);
    destroy_sprite(vicotory);
    destroy_sprite(number0);
    destroy_sprite(number1);
    destroy_sprite(number2);
    destroy_sprite(number3);
    destroy_sprite(number4);
    destroy_sprite(number5);
    destroy_sprite(number6);
    destroy_sprite(number7);
    destroy_sprite(number8);
    destroy_sprite(number9);
    destroy_sprite(colon);
    destroy_sprite(board_name);
    destroy_sprite(your);
    destroy_sprite(opponent);
    destroy_sprite(start);
    destroy_sprite(selected_start);
    destroy_sprite(exit_idk);
    destroy_sprite(exit_selected);
    destroy_sprite(lose);
}

void setup_backround(){
    prepare_backround(backround, 0, 0,title_screen_backround_buffer);
    prepare_backround(start, 420, 398,title_screen_backround_buffer);
    prepare_backround(exit_idk, 471, 500,title_screen_backround_buffer);
    prepare_backround(titleName, 231, 100,title_screen_backround_buffer);
    prepare_backround(gameBackround, 0, 0,game_board_backround_buffer);
    prepare_backround(XIco, 0, 0,game_board_backround_buffer);
    prepare_backround(board, 350, 220,game_board_backround_buffer);
    prepare_backround(boat_left, 70, 260,game_board_backround_buffer);
    prepare_backround(boat_right, 120, 260,game_board_backround_buffer);
    prepare_backround(boat_left, 70, 360,game_board_backround_buffer);
    prepare_backround(boat_middle_hor, 120, 360,game_board_backround_buffer);
    prepare_backround(boat_right, 170, 360,game_board_backround_buffer);
    prepare_backround(boat_left, 70, 460,game_board_backround_buffer);
    prepare_backround(boat_middle_hor, 120, 460,game_board_backround_buffer);
    prepare_backround(boat_middle_hor, 170, 460,game_board_backround_buffer);
    prepare_backround(boat_right, 220, 460,game_board_backround_buffer);
}


void update_timer_state() {
    //setup_backround();
    switch (state)
    {
    case Title:
        draw_title_screen();
        break;
    case Placement:
        draw_placement();
        break;  
    case Attack:
        draw_attack();
        break;
    case Defend:
        //ai_actions();
        //ACCUALLY IMPORTANTE DESCOMENTA DEPOIS !!!!!!!!!!!!!!!!!!!!!!!!!!!
        if(aiCounter%(GAME_FPS)==0){
          ai_actions();
        }
        draw_defend();
        aiCounter++;
        break;
    case Victory:
        draw_victory();
        break;
    }
    swap_buffers();
    timer_interrupts++;
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    isKeyboard=false;
    if (byteIndex == 3) {
        mouse_sync_mouseInfo();
        switch (state)
        {
        case Title:
            update_mouse_actions_title();
            break;
        case Placement:
            getBoardPos();
            update_mouse_actions_placement();
            break;
        case Attack:
            getBoardPos();
            update_mouse_actions_attack();
            break;
        case Defend:
            update_mouse_actions_defend();
            break;  
        case Victory:
            update_mouse_actions_victory();
            break;    
        }
        byteIndex=0;
    }
}

void update_mouse_actions_title() {
    if(mouse_info.x>420 && mouse_info.x<676 && mouse_info.y>398 && mouse_info.y<498){
        selected=1;
    }
    else if(mouse_info.x>471 && mouse_info.x<681 && mouse_info.y>500 && mouse_info.y<575){
        selected=2;
    }
    else{
        selected=0;
    }
    if(mouse_info.left_click){
        menu_actions();
    }
}

void menu_actions(){
    switch (selected){
        case 1:
            printf("player1 state");
            state=Placement;
            initialTime.tm_sec = rtc_data[0];
            initialTime.tm_min = rtc_data[1];
            initialTime.tm_hour = rtc_data[2];
            lastAttackedBoat=(rand() % (64 - 1 + 1)) + 1;
            game.doublesBoatsLeft=doubles;
            game.triplesBoatsLeft=triples;
            game.quadsBoatsLeft=quads;
            game.doublesAiBoatsLeft=doubles;
            game.triplesAiBoatsLeft=triples;
            game.quadsAiBoatsLeft=quads;
            memset(player_board,0,sizeof player_board);
            memset(enemy_board,0,sizeof enemy_board);
            break;
        case 2:
            systemState = EXIT;
            break;
    }
}
void getBoardPos(){
    if(mouse_info.x>402 && mouse_info.x<801 && mouse_info.y>272 && mouse_info.y<671){
        y=((mouse_info.y-original_board_y)/50)+1;
        x=((mouse_info.x-original_board_x)/50)+1;
        board_index=((y-1)*8)+x;
    }
    else{board_index=0;}
}

void update_mouse_actions_placement() {
    if(mouse_info.left_click){
        //printf("x: %d | y: %d",mouse_info.x,mouse_info.y);
       
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }

        
        if(mouse_info.x>70 && mouse_info.x<270 && mouse_info.y>260 && mouse_info.y<510){
            if(mouse_info.x<170 && mouse_info.y<310 && game.doublesBoatsLeft!=0){
                //double
                current_boat=2;
                printf("Selected double");
            }
            else if(mouse_info.x<220 && mouse_info.y>360 && mouse_info.y<410 && game.triplesBoatsLeft!=0){
                current_boat=3;
                printf("Selected triple");
                //triple
            }
            else if(mouse_info.y>460 && game.quadsBoatsLeft!=0){
                current_boat=4;
                printf("Selected quad");
                //quad
            }
        }
        else{
            placeBoat();
        }        
    }
    if(mouse_info.right_click){
        vert=!vert;
    }
}

void update_mouse_actions_attack(){
    //printf("nuts");
    if(mouse_info.left_click){
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }
        ////printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
        if(board_index!=0 && !playerBoardVisible){
            attack(enemy_board);
        }
    }
    else if(mouse_info.right_click){
        playerBoardVisible=!playerBoardVisible;
    }
}

void update_mouse_actions_defend(){
    if(mouse_info.left_click){
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }
        else if(aiAttackTries==4 && mouse_info.x>mode_info.XResolution-48 && mouse_info.y<40){
            state=Attack;
        }
        ////printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
    }
}

void update_mouse_actions_victory(){
    if(mouse_info.left_click){
        if(mouse_info.x<45 && mouse_info.y<45){
            state=Title;
        }
    }
}

void placeBoat(){
    ////printf(" board_index: %d | current_boat: %d ",board_index,current_boat);
    //printf(" board_index: %d | x: %d | y: %d ",board_index,(board_index-1)%8,(board_index-1)/8);
    if(board_index==0){
        current_boat=0;
    }
    else if(!player_board[board_index].hasBoat){
        switch (current_boat)
        {
        case 2:
            if(vert){
                if(y<8 && !player_board[board_index+8].hasBoat){
                    //printf(" placed double ");
                    addBoat(player_board,board_index,up_p);
                    addBoat(player_board,board_index+8,down_p);
                    game.doublesBoatsLeft--;
                }
            }
            else{
                if(x<8 && !player_board[board_index+1].hasBoat){
                    //printf(" placed double ");
                    addBoat(player_board,board_index,left_p);
                    addBoat(player_board,board_index+1,right_p);
                    game.doublesBoatsLeft--;
                }
            }
            if(game.doublesBoatsLeft==0){
                current_boat=0;
            }
            break;
        case 3:
            if(vert){
                if(y<7 && !player_board[board_index+8].hasBoat && !player_board[board_index+16].hasBoat){
                    //printf(" placed triple ");
                    addBoat(player_board,board_index,up_p);
                    addBoat(player_board,board_index+8,middle_vertical_p);
                    addBoat(player_board,board_index+16,down_p);
                    game.triplesBoatsLeft--;
                }
            }
            else{
                if(x<7 && !player_board[board_index+1].hasBoat && !player_board[board_index+2].hasBoat){
                    //printf(" placed triple ");
                    addBoat(player_board,board_index,left_p);
                    addBoat(player_board,board_index+1,middle_horizontal_p);
                    addBoat(player_board,board_index+2,right_p);
                    game.triplesBoatsLeft--;
                }
            }
            if(game.triplesBoatsLeft==0){
                current_boat=0;
            }
            break;
        case 4:
            if(vert){
                if(y<6 && !player_board[board_index+8].hasBoat && !player_board[board_index+16].hasBoat && !player_board[board_index+24].hasBoat){
                    //printf(" placed quad ");
                    addBoat(player_board,board_index,up_p);
                    addBoat(player_board,board_index+8,middle_vertical_p);
                    addBoat(player_board,board_index+16,middle_vertical_p);
                    addBoat(player_board,board_index+24,down_p);
                    game.quadsBoatsLeft--;
                }
            }
            else{
                if(x<6 && !player_board[board_index+1].hasBoat && !player_board[board_index+2].hasBoat && !player_board[board_index+3].hasBoat){
                    //printf(" placed quad ");
                    addBoat(player_board,board_index,left_p);
                    addBoat(player_board,board_index+1,middle_horizontal_p);
                    addBoat(player_board,board_index+2,middle_horizontal_p);
                    addBoat(player_board,board_index+3,right_p);
                    game.quadsBoatsLeft--;
                }
            }
            if(game.quadsBoatsLeft==0){
                current_boat=0;
            }
            break;
        default:
            break;
        }
    }
    if(game.quadsBoatsLeft==0 && game.triplesBoatsLeft==0 && game.doublesBoatsLeft==0){
        game.quadsBoatsLeft=quads;
        game.triplesBoatsLeft=triples;
        game.doublesBoatsLeft=doubles;
        placeAiBoats();
        state=Attack;
        //printf("                           test                         ");
    }
}

void attack(struct slot atackee[66]){
    if(!atackee[board_index].probed){
        atackee[board_index].probed=true;
        if(atackee[board_index].hasBoat){
            //printf("found %d ",atackee[board_index].len);
            switch (atackee[board_index].len)
            {
            case 2:
                switch (atackee[board_index].pos){
                case left_p:
                    if(atackee[board_index+1].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                    }
                    break;
                case right_p:
                    if(atackee[board_index-1].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index-1].sinked=true;
                    }
                    break;
                case up_p:
                    if(atackee[board_index+8].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                    }
                    break;
                case down_p:
                    if(atackee[board_index-8].probed){
                        //printf("sinked");
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
                case left_p:
                    if(atackee[board_index+1].probed && atackee[board_index+2].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                        atackee[board_index+2].sinked=true;
                    }
                    break;
                case middle_horizontal_p:
                    if(atackee[board_index+1].probed && atackee[board_index-1].probed){
                        //printf("sinked");
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                    }
                    break;
                case right_p:
                    if(atackee[board_index-1].probed && atackee[board_index-2].probed){
                        //printf("sinked");
                        atackee[board_index-2].sinked=true;
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                        
                    }
                    break;
                case up_p:
                    if(atackee[board_index+8].probed && atackee[board_index+16].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                        atackee[board_index+16].sinked=true;
                    }
                    break;
                case middle_vertical_p:
                    if(atackee[board_index+8].probed && atackee[board_index-8].probed){
                        //printf("sinked");
                        atackee[board_index-8].sinked=true;
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                    }
                    break;
                case down_p:
                    if(atackee[board_index-8].probed && atackee[board_index-16].probed){
                        //printf("sinked");
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
                case left_p:
                    if(atackee[board_index+1].probed && atackee[board_index+2].probed && atackee[board_index+3].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+1].sinked=true;
                        atackee[board_index+2].sinked=true;
                        atackee[board_index+3].sinked=true;
                    }
                    break;
                case middle_horizontal_p:
                    if(atackee[board_index+1].probed && atackee[board_index-1].probed){
                        if(atackee[board_index-1].pos==left_p && atackee[board_index+2].probed){
                            atackee[board_index+2].sinked=true;
                            atackee[board_index-1].sinked=true;
                            atackee[board_index].sinked=true;
                            atackee[board_index+1].sinked=true;
                        }
                        else if (atackee[board_index+1].pos==right_p && atackee[board_index-2].probed){
                            atackee[board_index-2].sinked=true;
                            atackee[board_index-1].sinked=true;
                            atackee[board_index].sinked=true;
                            atackee[board_index+1].sinked=true;
                        }
                        //printf("sinked");
                    }
                    break;
                case right_p:
                    if(atackee[board_index-1].probed && atackee[board_index-2].probed && atackee[board_index-3].probed){
                        //printf("sinked");
                        atackee[board_index-3].sinked=true;
                        atackee[board_index-2].sinked=true;
                        atackee[board_index-1].sinked=true;
                        atackee[board_index].sinked=true;
                    }
                    break;
                case up_p:
                    if(atackee[board_index+8].probed && atackee[board_index+16].probed && atackee[board_index+24].probed){
                        //printf("sinked");
                        atackee[board_index].sinked=true;
                        atackee[board_index+8].sinked=true;
                        atackee[board_index+16].sinked=true;
                        atackee[board_index+24].sinked=true;
                    }
                    break;
                case middle_vertical_p:
                    if(atackee[board_index+8].probed && atackee[board_index-8].probed){
                        if(atackee[board_index-8].pos==up_p && atackee[board_index+16].probed){
                            atackee[board_index+16].sinked=true;
                            atackee[board_index-8].sinked=true;
                            atackee[board_index].sinked=true;
                            atackee[board_index+8].sinked=true;
                        }
                        else if (atackee[board_index+8].pos==down_p && atackee[board_index-16].probed){
                            atackee[board_index-16].sinked=true;
                            atackee[board_index-8].sinked=true;
                            atackee[board_index].sinked=true;
                            atackee[board_index+8].sinked=true;
                        }
                        //printf("sinked");
                    }
                    break;
                case down_p:
                    if(atackee[board_index-8].probed && atackee[board_index-16].probed && atackee[board_index-24].probed){
                        //printf("sinked");
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
            if(atackee[board_index].sinked){
                if(state==Attack){
                    switch (atackee[board_index].len){
                        case 2:
                            game.doublesAiBoatsLeft--;
                            //printf("      AI DOUBLE SANK, %d LEFT   ",game.doublesAiBoatsLeft);
                            break;
                        case 3:
                            game.triplesAiBoatsLeft--;
                            //printf("      AI TRIPLE SANK, %d LEFT   ",game.triplesAiBoatsLeft);
                            break;
                        case 4:
                            game.quadsAiBoatsLeft--;
                            //printf("      AI QUAD SANK, %d LEFT   ",game.quadsAiBoatsLeft);
                            break;
                        }
                    if(!game.doublesAiBoatsLeft && !game.triplesAiBoatsLeft && !game.quadsAiBoatsLeft){
                        state = Victory;
                        endTime = rtc_data[0] + rtc_data[1]*60 + rtc_data[2]*3600;
                        victory=true;
                    }
                }
                else{
                    switch (atackee[board_index].len){
                        case 2:
                            game.doublesBoatsLeft--;
                            //printf("      PLAYER DOUBLE SANK, %d LEFT   ",game.doublesBoatsLeft);
                            break;
                        case 3:
                            game.triplesBoatsLeft--;
                            //printf("      PLAYER TRIPLE SANK, %d LEFT   ",game.triplesBoatsLeft);
                            break;
                        case 4:
                            game.quadsBoatsLeft--;
                            //printf("      PLAYER QUAD SANK, %d LEFT   ",game.quadsBoatsLeft);
                            break;
                        }
                    if(!game.quadsBoatsLeft && !game.triplesBoatsLeft && !game.quadsBoatsLeft){
                        state = Victory;
                        endTime = rtc_data[0] + rtc_data[1]*60 + rtc_data[2]*3600;
                    }
                }

            }
        }
        else {
            if(state==Attack){
                //printf("                   ai time now                   ");
                aiAttackTries=(rand() % (3 - 1 + 1)) + 1;
                state=Defend;
            }
        }
    }
}

void ai_actions(){
    //easy ai
    /*
  if(aiAttackTries>0){
        do{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            //printf(" board_index: %d",board_index);
        } while (player_board[board_index].probed);
        aiAttackTries--;
  }
  else{
        attack(player_board);
        if(player_board[board_index].hasBoat){
            aiAttackTries=(rand() % (3 - 1 + 1)) + 1;
        }
        else{
            aiAttackTries=-1;
        }
  }
  */
    //harder ai
    if(aiAttackTries>0 && aiAttackTries!=4){
        uint8_t counter=0;
        do{
            if (counter==10){
                do{
                    y=(rand() % (8 - 1 + 1)) + 1;
                    x=(rand() % (8 - 1 + 1)) + 1;
                    board_index=((y-1)*8)+x;

                    
                } while (player_board[board_index].probed);
                break;
            }
            printf(" board_index: %d \n",lastAttackedBoat);
            uint8_t tempx = (lastAttackedBoat-1)%8 +1;
            uint8_t tempy = (lastAttackedBoat-1)/8 +1;
            printf("  x:%d |y:%d  \n",tempx,tempy);
            uint8_t random = (rand() % (4 - 1 + 1)) + 1;
            switch (random)
            {
            case 1:
                if (tempy>1){
                    tempy--;
                }
                break;
            case 2:
                if (tempy<8){
                    tempy++;
                }
                break;
            case 3:
                if (tempx>1){
                    tempx--;
                }
                break;
            case 4:
                if (tempx<8){
                    tempx++;
                }
                break;
            }
            board_index=((tempy-1)*8)+tempx;
            printf(" board_index: %d \n",board_index);
            counter++;
        } while (player_board[board_index].probed);
        aiAttackTries--;
    }
    else{
        attack(player_board);
        if(player_board[board_index].hasBoat){
            lastAttackedBoat=board_index;
            aiAttackTries=(rand() % (3 - 1 + 1)) + 1;
        }
        else{
            aiAttackTries=4;
        }
    }
}

void placeAiBoats(){
    //printf("                                                   ai turn:                                                     ");
    srand(time(NULL));
    current_boat=4;
    while(game.quadsAiBoatsLeft!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (5 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat && !enemy_board[board_index+16].hasBoat && !enemy_board[board_index+24].hasBoat){
                //printf(" placed quad ");
                addBoat(enemy_board,board_index,up_p);
                addBoat(enemy_board,board_index+8,middle_vertical_p);
                addBoat(enemy_board,board_index+16,middle_vertical_p);
                addBoat(enemy_board,board_index+24,down_p);
                game.quadsAiBoatsLeft--;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (5 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat && !enemy_board[board_index+2].hasBoat && !enemy_board[board_index+3].hasBoat){
                //printf(" placed quad ");
                addBoat(enemy_board,board_index,left_p);
                addBoat(enemy_board,board_index+1,middle_horizontal_p);
                addBoat(enemy_board,board_index+2,middle_horizontal_p);
                addBoat(enemy_board,board_index+3,right_p);
                game.quadsAiBoatsLeft--;
            }
        }
    }
    current_boat=3;
    while(game.triplesAiBoatsLeft!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (6 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat && !enemy_board[board_index+16].hasBoat){
                //printf(" placed triple ");
                addBoat(enemy_board,board_index,up_p);
                addBoat(enemy_board,board_index+8,middle_vertical_p);
                addBoat(enemy_board,board_index+16,down_p);
                game.triplesAiBoatsLeft--;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (6 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat && !enemy_board[board_index+2].hasBoat){
                //printf(" placed triple ");
                addBoat(enemy_board,board_index,left_p);
                addBoat(enemy_board,board_index+1,middle_horizontal_p);
                addBoat(enemy_board,board_index+2,right_p);
                game.triplesAiBoatsLeft--;
            }
        }
    }
    current_boat=2;
    while(game.doublesAiBoatsLeft!=0){
        vert=rand() % 2;
        if(vert){
            y=(rand() % (7 - 1 + 1)) + 1;
            x=(rand() % (8 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+8].hasBoat){
                //printf(" placed double ");
                addBoat(enemy_board,board_index,up_p);
                addBoat(enemy_board,board_index+8,down_p);
                game.doublesAiBoatsLeft--;
            }
        }
        else{
            y=(rand() % (8 - 1 + 1)) + 1;
            x=(rand() % (7 - 1 + 1)) + 1;
            board_index=((y-1)*8)+x;
            if(!enemy_board[board_index].hasBoat && !enemy_board[board_index+1].hasBoat){
                //printf(" placed double ");
                addBoat(enemy_board,board_index,left_p);
                addBoat(enemy_board,board_index+1,right_p);
                game.doublesAiBoatsLeft--;
            }
        }
    }
    game.doublesAiBoatsLeft=doubles;
    game.triplesAiBoatsLeft=triples;
    game.quadsAiBoatsLeft=quads;
    current_boat=0;
}

void addBoat(struct slot board[66],uint8_t board_index,enum part pos){
    board[board_index].hasBoat=true;
    board[board_index].probed=false;
    board[board_index].sinked=false;
    board[board_index].len=current_boat;
    board[board_index].pos=pos;
}

void update_keyboard_state(){
    (kbc_ih)();
    isKeyboard=true;
    switch (scancode) {
        case Q_KEY:{
            systemState = EXIT;
            break;
        }
        default:
            break;
    }
    switch (state){
    case Title:
        update_keyboard_actions_title();
        break;
    case Placement:
        getKeyboadPos();
        update_keyboard_actions_placement();
        break;
    case Attack:
        getKeyboadPos();
        update_keyboard_actions_attack();
        break;
    case Defend:
        update_keyboard_actions_defend();
        break;  
    case Victory:
        update_keyboard_actions_victory();
        break;     	
    default:
        break;
    }
    //draw_test();
}

void update_keyboard_actions_title(){
    switch (scancode)
    {
    case W_KEY:
        if(selected==1){
            selected=2;
        }
        else {
            selected=1;
        }
        break;
    case S_KEY:
        if (selected==2){
            selected=1;
        }
        else{
            selected=2;
        }
        break;
    case E_KEY:
        menu_actions();
        //printf("current pos: %d ",board_index);
        break;
    case ESC_Key:
        systemState = EXIT;
        break;
    default:
        break;
    }
}

void update_keyboard_actions_placement(){
    switch (scancode)
    {
    case R_KEY:
        vert=!vert;
        break;
    case E_KEY:
        placeBoat();
        break;
    case ONE_KEY:
        current_boat=0;
        //printf("deselecting");
        break;
    case TWO_KEY:
        if(game.doublesBoatsLeft!=0){
            current_boat=2;
            //printf("Selected double");
        }
        break;
    case THREE_KEY:
        if(game.triplesBoatsLeft!=0){
            current_boat=3;
            //printf("Selected triple");
        }
        break;
    case FOUR_KEY:
        if(game.quadsBoatsLeft!=0){
            current_boat=4;
            //printf("Selected quad");
        }
        break;
    case ESC_Key:
        state=Title;
        break;
    default:
        break;
    }
} 

void update_keyboard_actions_attack(){
    switch (scancode){
    case ESC_Key:
        state=Title;
        break;
    case E_KEY:
        if(board_index!=0){
            attack(enemy_board);
        }
        break;
    case R_KEY:
        playerBoardVisible=!playerBoardVisible;
        break;
    default:
        break;
    }
}

void update_keyboard_actions_defend(){
    switch (scancode){
    case ESC_Key:
        state=Title;
        break;
    case E_KEY:
        if(aiAttackTries==4){
            state=Attack;
        }
        break;
    default:
        break;
    }
}
void update_keyboard_actions_victory(){
    switch (scancode){
    case ESC_Key:
        state=Title;
        break;
    default:
        break;
    }
}

void getKeyboadPos(){
    switch (scancode)
    {
    case W_KEY:
        if(board_index==0){
            board_index=1;
        }
        else if(y==1){
            y=8;
        }
        else{
            y--;
        }
        break;
    case S_KEY:
        if(board_index==0){
            board_index=1;
        }
        else if(y==8){
            y=1;
        }
        else{
            y++;
        }
        break;
    case A_KEY:
        if(board_index==0){
            board_index=1;
        }
        else if(x==1){
            x=8;
        }
        else{
            x--;
        }
        break;
    case D_KEY:
        if(board_index==0){
            board_index=1;
        }
        else if(x==8){
            x=1;
        }
        else{
            x++;
        }
        break;
    }
    board_index=((y-1)*8)+x;
}

