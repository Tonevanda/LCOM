#include "draw.h"

uint8_t *main_frame_buffer;
uint8_t *drawing_frame_buffer;
uint8_t *title_screen_backround_buffer;
uint8_t *game_board_backround_buffer;
uint32_t frame_buffer_size;

extern uint8_t rtc_data[6];
extern struct tm initialTime;
extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern Sprite *mouse;
extern Sprite *XIco;
extern Sprite *frame;
extern Sprite *boat_down;
extern Sprite *boat_up;
extern Sprite *boat_middle_vert;
extern Sprite *boat_left;
extern Sprite *boat_right;
extern Sprite *boat_middle_hor;
extern Sprite *arrow;
extern Sprite *ProbedHole;
extern Sprite *redX;
extern Sprite *whiteX;
extern Sprite *vicotory;
extern Sprite *number0;
extern Sprite *number1;
extern Sprite *number2;
extern Sprite *number3;
extern Sprite *number4;
extern Sprite *number5;
extern Sprite *number6;
extern Sprite *number7;
extern Sprite *number8;
extern Sprite *number9;
extern Sprite *colon;
extern Sprite *your;
extern Sprite *opponent;
extern Sprite *board_name;
extern Sprite *lose;
extern Sprite *selected_start;
extern Sprite *exit_selected;

int selected=0;
int original_board_x=402; 
int original_board_y=272;
int board_index=0;
int current_boat;
bool isKeyboard=false;
bool vert=false;
bool playerBoardVisible=false;
uint16_t modes;
struct slot player_board[66];
struct slot enemy_board[66];
struct gameInfo game;
time_t endTime;
bool victory=false;



int set_frame_buffers(uint16_t mode) {
    modes=mode;
    if (set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    drawing_frame_buffer =(uint8_t *) malloc(frame_buffer_size);
    title_screen_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    game_board_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    return 0;
}

void swap_buffers() {
    memcpy(main_frame_buffer, drawing_frame_buffer, frame_buffer_size);
}

// A função recebe um objeto Sprite proveniente de um XPM e mostra-o nas coordenadas (x, y)
// Usa as cores dinamicamente alocadas na altura da construção
// A função ignora a cor transparente do XPM para não modificar o fundo quando não é preciso
int draw_sprite_xpm(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == 0xFFFFFE) {
            continue;
            }
        else if (draw_pixel(x + w, y + h, current_color, drawing_frame_buffer) != 0) return 1;
      }
    }
    return 0; 
}

int draw_number_xpm(int number, int x, int y){
    switch (number)
    {
    case 0:
        if(draw_sprite_xpm(number0,x,y)) return 1;
        break;
    case 1:
        if(draw_sprite_xpm(number1,x,y)) return 1;
        break;
    case 2:
        if(draw_sprite_xpm(number2,x,y)) return 1;
        break;
    case 3:
        if(draw_sprite_xpm(number3,x,y)) return 1;
        break;
    case 4:
        if(draw_sprite_xpm(number4,x,y)) return 1;
        break;
    case 5:
        if(draw_sprite_xpm(number5,x,y)) return 1;
        break;
    case 6:
        if(draw_sprite_xpm(number6,x,y)) return 1;
        break;
    case 7:
        if(draw_sprite_xpm(number7,x,y)) return 1;
        break;
    case 8:
        if(draw_sprite_xpm(number8,x,y)) return 1;
        break;
    case 9:
        if(draw_sprite_xpm(number9,x,y)) return 1;
        break;
    }
    return 0;
}

int prepare_backround(Sprite *sprite, int x, int y,uint8_t* buffer) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == 0xFFFFFE) {
            continue;
            }
        else if (draw_pixel(x + w, y + h, current_color, buffer) != 0) return 1;
      }
    }
    return 0; 
}

void draw_title_screen(){
    memcpy(drawing_frame_buffer,title_screen_backround_buffer,frame_buffer_size);
    draw_title_selection();
    draw_rtc();
    draw_mouse();
}

void draw_placement(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_player_board_icons();
    draw_reticle();
    draw_selected();
    draw_rtc();
    draw_round_time();
    draw_mouse(); 
}

void draw_attack(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    if (!playerBoardVisible){
        draw_AI_board_icons();
    }
    else{
        draw_player_board_icons();
    }
    draw_reticle();
    draw_rtc();
    draw_round_time();
    draw_mouse();
}

void draw_defend(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_player_board_icons();
    draw_reticle();
    draw_rtc();
    draw_round_time();
    draw_sprite_xpm(arrow, mode_info.XResolution-48, 0);
    draw_mouse();
}

void draw_victory(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_rtc();
    draw_end_time();
    if (victory)
    {
        draw_AI_board_icons();
        draw_sprite_xpm(vicotory, 465, 312);
    }
    else{
        draw_player_board_icons();
        draw_sprite_xpm(lose, 472, 315);
    }
    draw_mouse(); 
}

void draw_title_selection(){    
    if (selected==1){
        draw_sprite_xpm(selected_start, 420, 398);
    }
    else if (selected==2){
        draw_sprite_xpm(exit_selected, 471, 500);
    }
}

void draw_reticle(){
    if (board_index!=0){
        int x = (board_index-1)%8;
        int y = (board_index-1)/8;
        draw_sprite_xpm(frame, original_board_x+((x)*50), original_board_y+((y)*50));
    }
}

void draw_selected(){
    int x_rel;
    int y_rel;
    if (!isKeyboard){
        x_rel=mouse_info.x;
        y_rel=mouse_info.y;
    }
    else{
        int x = (board_index-1)%8;
        int y = (board_index-1)/8;
        x_rel=original_board_x+((x)*50);
        y_rel=original_board_y+((y)*50);
    }
    switch (current_boat)
    {
    case 2:
        if (vert){
            draw_sprite_xpm(boat_up, x_rel, y_rel);
            draw_sprite_xpm(boat_down, x_rel, y_rel+50);
        }
        else{
            draw_sprite_xpm(boat_left, x_rel, y_rel);
            draw_sprite_xpm(boat_right, x_rel+50, y_rel);
        }
        break;
    case 3:
        if (vert){
            draw_sprite_xpm(boat_up, x_rel, y_rel);
            draw_sprite_xpm(boat_middle_vert, x_rel, y_rel+50);
            draw_sprite_xpm(boat_down, x_rel, y_rel+100);
        }
        else{
            draw_sprite_xpm(boat_left, x_rel, y_rel);
            draw_sprite_xpm(boat_middle_hor, x_rel+50, y_rel);
            draw_sprite_xpm(boat_right, x_rel+100, y_rel);
        }
        break;
    case 4:
        if (vert){
            draw_sprite_xpm(boat_up, x_rel, y_rel);
            draw_sprite_xpm(boat_middle_vert, x_rel, y_rel+50);
            draw_sprite_xpm(boat_middle_vert, x_rel, y_rel+100);
            draw_sprite_xpm(boat_down, x_rel, y_rel+150);
        }
        else{
            draw_sprite_xpm(boat_left, x_rel, y_rel);
            draw_sprite_xpm(boat_middle_hor, x_rel+50, y_rel);
            draw_sprite_xpm(boat_middle_hor, x_rel+100, y_rel);
            draw_sprite_xpm(boat_right, x_rel+150, y_rel);
        }
        break;
    default:
        break;
    }
}

void draw_mouse() {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
}

void draw_rtc(){
    
    int x = 200;
    int y = 800;

    int seconds = rtc_data[0];
    int digit;
    for(int i = 0;i < 2;i++){ //SECONDS
        digit = seconds % 10;
        draw_number_xpm(digit, x, y);
        seconds = seconds / 10;
        x -= 24;
    }
    draw_sprite_xpm(colon,x+5,y);
    x-=25;
    int minutes = rtc_data[1];
    for(int i = 0;i < 2;i++){ //MINUTES
        digit = minutes % 10;
        draw_number_xpm(digit, x, y);
        minutes = minutes / 10;
        x-=24;
    }
    draw_sprite_xpm(colon,x+5,y);
    x-=25;
    int hours = (rtc_data[2] + 1) % 24;
    for(int i = 0;i < 2;i++){ //HOURS
        digit = hours % 10;
        draw_number_xpm(digit, x, y);
        hours = hours / 10;
        x-=24;
    }
}

void draw_round_time(){

    int x = 600;
    int y = 800;
    
    time_t initialTimeInSeconds = initialTime.tm_sec + initialTime.tm_min *60 + initialTime.tm_hour * 3600;
    time_t currentTime = rtc_data[0] + rtc_data[1]*60 + rtc_data[2]*3600;
    time_t timeDifference = currentTime - initialTimeInSeconds;
    int seconds = timeDifference % 60;
    int digit;
    for(int i = 0;i < 2;i++){ //SECONDS
        digit = seconds % 10;
        draw_number_xpm(digit, x, y);
        seconds = seconds / 10;
        x -= 24;
    }
    draw_sprite_xpm(colon,x+5,y);
    x-=25;
    int minutes = (timeDifference % 3600) / 60;
    for(int i = 0;i < 2;i++){ //SECONDS
        digit = minutes % 10;
        draw_number_xpm(digit, x, y);
        minutes = minutes / 10;
        x -= 24;
    }
}

void draw_end_time(){

    int x = 600;
    int y = 800;
    
    time_t initialTimeInSeconds = initialTime.tm_sec + initialTime.tm_min *60 + initialTime.tm_hour * 3600;
    time_t timeDifference = endTime - initialTimeInSeconds;
    int seconds = timeDifference % 60;
    int digit;
    for(int i = 0;i < 2;i++){ //SECONDS
        digit = seconds % 10;
        draw_number_xpm(digit, x, y);
        seconds = seconds / 10;
        x -= 24;
    }
    draw_sprite_xpm(colon,x+5,y);
    x-=25;
    int minutes = (timeDifference % 3600) / 60;
    for(int i = 0;i < 2;i++){ //SECONDS
        digit = minutes % 10;
        draw_number_xpm(digit, x, y);
        minutes = minutes / 10;
        x -= 24;
    }
}

void draw_player_board_icons(){
    draw_number_xpm(game.doublesBoatsLeft,40,268);
    draw_number_xpm(game.triplesBoatsLeft,40,368);
    draw_number_xpm(game.quadsBoatsLeft,40,468);
    //284
    //304
    draw_sprite_xpm(your,284,50);
    draw_sprite_xpm(board_name,556,50);
    for(int temp=1;temp<66;temp++){
        int x = (temp-1)%8;
        int y = (temp-1)/8;
        if(player_board[temp].hasBoat){
            pos_switch(x,y,player_board[temp].pos);
            if(player_board[temp].sinked){
                draw_on_board(x,y,redX);
            }
            else if (player_board[temp].probed){
                draw_on_board(x,y,whiteX);
            }
        }
        else if (player_board[temp].probed){
                draw_on_board(x,y,ProbedHole);
            }
        
    }
}

void draw_AI_board_icons(){
    draw_number_xpm(game.doublesAiBoatsLeft,40,268);
    draw_number_xpm(game.triplesAiBoatsLeft,40,368);
    draw_number_xpm(game.quadsAiBoatsLeft,40,468);
    draw_sprite_xpm(opponent,158,50);
    draw_sprite_xpm(board_name,681,50);
    for(int temp=1;temp<66;temp++){
        int x = (temp-1)%8;
        int y = (temp-1)/8;
        if(enemy_board[temp].hasBoat){
            if(enemy_board[temp].sinked){
                pos_switch(x,y,enemy_board[temp].pos);
                draw_on_board(x,y,redX);
            }
            else if (enemy_board[temp].probed){
                draw_on_board(x,y,whiteX);
            }
        }
        else if (enemy_board[temp].probed){
            draw_on_board(x,y,ProbedHole);
        }
    }
}

void pos_switch(int x, int y, enum part pos){
    switch (pos){
    case left_p:
        draw_on_board(x,y,boat_left);
        break;
    case middle_horizontal_p:
        draw_on_board(x,y,boat_middle_hor);
        break;
    case right_p:
        draw_on_board(x,y,boat_right);
        break;
    case up_p:
        draw_on_board(x,y,boat_up);
        break;
    case middle_vertical_p:
        draw_on_board(x,y,boat_middle_vert);
        break;
    case down_p:
        draw_on_board(x,y,boat_down);
    break;
    }
}

void draw_on_board(int x,int y,Sprite *sprite){
    draw_sprite_xpm(sprite, original_board_x+((x)*50), original_board_y+((y)*50));
}
