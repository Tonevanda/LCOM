#include "draw.h"

uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint8_t *title_screen_backround_buffer;
uint8_t *game_board_backround_buffer;
uint32_t frame_buffer_size;

extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern Sprite *mouse;
extern Sprite *plus;
extern Sprite *XIco;
extern Sprite *t;
extern Sprite *player1;
extern Sprite *player2;
extern Sprite *frame;
extern Sprite *boat_down;
extern Sprite *boat_up;
extern Sprite *boat_middle_vert;
extern Sprite *boat_left;
extern Sprite *boat_right;
extern Sprite *boat_middle_hor;

int selected=0;
int original_board_x=402; 
int original_board_y=272;
int board_index=0;
int current_boat;
bool isKeyboard=false;
bool vert=false;
struct slot player_board[66];
struct slot enemy_board[66];


int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

    secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    drawing_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    title_screen_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    game_board_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    return 0;
}

void swap_buffers() {memcpy(main_frame_buffer, drawing_frame_buffer, frame_buffer_size);}

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
        //printf(" %d ",current_color);
        if (current_color == 0xFFFFFE) {
            //printf("x: %d ,y: %d ",x+w,y+w);
            //draw_pixel(x + w, y + h, 0xFF0000, drawing_frame_buffer);
            continue;
            }
        else if (draw_pixel(x + w, y + h, current_color, drawing_frame_buffer) != 0) return 1;
      }
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
        //printf(" %d ",current_color);
        if (current_color == 0xFFFFFE) {
            //printf("x: %d ,y: %d ",x+w,y+w);
            //draw_pixel(x + w, y + h, 0xFF0000, drawing_frame_buffer);
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
    draw_mouse();
}

void draw_placement(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_player_board_icons();
    draw_reticle();
    draw_selected();
    draw_mouse(); 
}

void draw_attack(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_AI_board_icons();
    draw_reticle();
    draw_mouse();
}

void draw_defend(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_player_board_icons();
    draw_mouse();
}

void draw_title_selection(){    
    if (selected==1){
        draw_sprite_xpm(player1, 300, 300);
    }
    else if (selected==2){
        draw_sprite_xpm(player2, 300, 336);
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

void draw_player_board_icons(){
    for(int temp=1;temp<66;temp++){
        int x = (temp-1)%8;
        int y = (temp-1)/8;
        if(player_board[temp].hasBoat){
            pos_switch(x,y,player_board[temp].pos);
        }
        if (player_board[temp].probed){
            draw_on_board(x,y,XIco);
        }
    }
}

void draw_AI_board_icons(){
    for(int temp=1;temp<66;temp++){
        int x = (temp-1)%8;
        int y = (temp-1)/8;
        if(enemy_board[temp].hasBoat && enemy_board[temp].sinked){
            pos_switch(x,y,enemy_board[temp].pos);
        }
        if (enemy_board[temp].probed){
            draw_on_board(x,y,XIco);
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
