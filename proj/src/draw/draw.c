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
bool vert=false;
struct slot player_board[66];
struct slot enemy_board[66];

void swap_buffers() {
    //memcpy(main_frame_buffer, title_screen_backround_buffer, frame_buffer_size);
    memcpy(main_frame_buffer, drawing_frame_buffer, frame_buffer_size);
    //if (timer_interrupts % GAME_FREQUENCY == 0) display_real_time();
}

void draw_title_screen(){
    //memset(drawing_frame_buffer,0,frame_buffer_size);
    memcpy(drawing_frame_buffer,title_screen_backround_buffer,frame_buffer_size);

    draw_title_selection();

    draw_mouse();
    //draw_sprite_xpm(plus, mode_info.XResolution/2 - 100, mode_info.YResolution/2 - 100);  
}

void draw_game_screen(){
    //memset(drawing_frame_buffer,0,frame_buffer_size);
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);

    //draw_title_slection();
    draw_boats();
    draw_reticle();
    draw_selected();
    draw_mouse();
     
}

void draw_enemy(){
    memcpy(drawing_frame_buffer,game_board_backround_buffer,frame_buffer_size);
    draw_mouse();
    for(int temp=1;temp<66;temp++){
        if(enemy_board[temp].hasBoat){
            int x = (temp-1)%8;
            int y = (temp-1)/8;
            switch (enemy_board[temp].pos)
            {
            case 1:
                draw_boat(x,y,boat_left);
                break;
            case 2:
                draw_boat(x,y,boat_middle_hor);
                break;
            case 3:
                draw_boat(x,y,boat_right);
                break;
            case 4:
                draw_boat(x,y,boat_up);
                break;
            case 5:
                draw_boat(x,y,boat_middle_vert);
                break;
            case 6:
                draw_boat(x,y,boat_down);
            break;
            }
        }
    }
}

void draw_boats(){
    for(int temp=1;temp<66;temp++){
        if(player_board[temp].hasBoat){
            int x = (temp-1)%8;
            int y = (temp-1)/8;
            switch (player_board[temp].pos)
            {
            case 1:
                draw_boat(x,y,boat_left);
                break;
            case 2:
                draw_boat(x,y,boat_middle_hor);
                break;
            case 3:
                draw_boat(x,y,boat_right);
                break;
            case 4:
                draw_boat(x,y,boat_up);
                break;
            case 5:
                draw_boat(x,y,boat_middle_vert);
                break;
            case 6:
                draw_boat(x,y,boat_down);
            break;
            }
        }
    }
}

void draw_boat(int x,int y,Sprite *sprite){
    draw_sprite_xpm(sprite, original_board_x+((x)*50), original_board_y+((y)*50));
    /*
    switch (index)
    {
    case 1:
        draw_sprite_xpm(sprite, original_board_x, original_board_y);
        break;
    case 2:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y);
        break;
    case 3:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y);
        break;
    case 4:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y);
        break;
    case 5:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y);
        break;
    case 6:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y);
        break;
    case 7:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y);
        break;
    case 8:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y);
        break;
    case 9:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+50);
        break;
    case 10:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+50);
        break;
    case 11:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+50);
        break;
    case 12:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+50);
        break;
    case 13:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+50);
        break;
    case 14:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+50);
        break;
    case 15:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+50);
        break;
    case 16:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+50);
        break;
    case 17:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+100);
        break;
    case 18:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+100);
        break;
    case 19:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+100);
        break;
    case 20:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+100);
        break;
    case 21:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+100);
        break;
    case 22:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+100);
        break;
    case 23:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+100);
        break;
    case 24:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+100);
        break;
    case 25:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+150);
        break;
    case 26:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+150);
        break;
    case 27:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+150);
        break;
    case 28:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+150);
        break;
    case 29:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+150);
        break;
    case 30:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+150);
        break;
    case 31:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+150);
        break;
    case 32:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+150);
        break;
    case 33:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+200);
        break;
    case 34:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+200);
        break;
    case 35:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+200);
        break;
    case 36:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+200);
        break;
    case 37:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+200);
        break;
    case 38:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+200);
        break;
    case 39:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+200);
        break;
    case 40:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+200);
        break;
    case 41:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+250);
        break;
    case 42:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+250);
        break;
    case 43:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+250);
        break;
    case 44:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+250);
        break;
    case 45:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+250);
        break;
    case 46:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+250);
        break;
    case 47:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+250);
        break;
    case 48:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+250);
        break;
    case 49:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+300);
        break;
    case 50:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+300);
        break;
    case 51:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+300);
        break;
    case 52:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+300);
        break;
    case 53:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+300);
        break;
    case 54:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+300);
        break;
    case 55:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+300);
        break;
    case 56:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+300);
        break;
    case 57:
        draw_sprite_xpm(sprite, original_board_x, original_board_y+350);
        break;
    case 58:
        draw_sprite_xpm(sprite, original_board_x+50, original_board_y+350);
        break;
    case 59:
        draw_sprite_xpm(sprite, original_board_x+100, original_board_y+350);
        break;
    case 60:
        draw_sprite_xpm(sprite, original_board_x+150, original_board_y+350);
        break;
    case 61:
        draw_sprite_xpm(sprite, original_board_x+200, original_board_y+350);
        break;
    case 62:
        draw_sprite_xpm(sprite, original_board_x+250, original_board_y+350);
        break;
    case 63:
        draw_sprite_xpm(sprite, original_board_x+300, original_board_y+350);
        break;
    case 64:
        draw_sprite_xpm(sprite, original_board_x+350, original_board_y+350);
        break;
    default:
        break;
    }
    */
}

void draw_reticle(){
    if (board_index!=0){
        int x = (board_index-1)%8;
        int y = (board_index-1)/8;
        draw_sprite_xpm(frame, original_board_x+((x)*50), original_board_y+((y)*50));
    }
    /*
    switch (board_index)
    {
    case 1:
        draw_sprite_xpm(frame, original_board_x, original_board_y);
        break;
    case 2:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y);
        break;
    case 3:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y);
        break;
    case 4:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y);
        break;
    case 5:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y);
        break;
    case 6:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y);
        break;
    case 7:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y);
        break;
    case 8:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y);
        break;
    case 9:
        draw_sprite_xpm(frame, original_board_x, original_board_y+50);
        break;
    case 10:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+50);
        break;
    case 11:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+50);
        break;
    case 12:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+50);
        break;
    case 13:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+50);
        break;
    case 14:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+50);
        break;
    case 15:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+50);
        break;
    case 16:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+50);
        break;
    case 17:
        draw_sprite_xpm(frame, original_board_x, original_board_y+100);
        break;
    case 18:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+100);
        break;
    case 19:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+100);
        break;
    case 20:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+100);
        break;
    case 21:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+100);
        break;
    case 22:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+100);
        break;
    case 23:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+100);
        break;
    case 24:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+100);
        break;
    case 25:
        draw_sprite_xpm(frame, original_board_x, original_board_y+150);
        break;
    case 26:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+150);
        break;
    case 27:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+150);
        break;
    case 28:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+150);
        break;
    case 29:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+150);
        break;
    case 30:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+150);
        break;
    case 31:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+150);
        break;
    case 32:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+150);
        break;
    case 33:
        draw_sprite_xpm(frame, original_board_x, original_board_y+200);
        break;
    case 34:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+200);
        break;
    case 35:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+200);
        break;
    case 36:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+200);
        break;
    case 37:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+200);
        break;
    case 38:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+200);
        break;
    case 39:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+200);
        break;
    case 40:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+200);
        break;
    case 41:
        draw_sprite_xpm(frame, original_board_x, original_board_y+250);
        break;
    case 42:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+250);
        break;
    case 43:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+250);
        break;
    case 44:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+250);
        break;
    case 45:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+250);
        break;
    case 46:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+250);
        break;
    case 47:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+250);
        break;
    case 48:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+250);
        break;
    case 49:
        draw_sprite_xpm(frame, original_board_x, original_board_y+300);
        break;
    case 50:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+300);
        break;
    case 51:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+300);
        break;
    case 52:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+300);
        break;
    case 53:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+300);
        break;
    case 54:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+300);
        break;
    case 55:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+300);
        break;
    case 56:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+300);
        break;
    case 57:
        draw_sprite_xpm(frame, original_board_x, original_board_y+350);
        break;
    case 58:
        draw_sprite_xpm(frame, original_board_x+50, original_board_y+350);
        break;
    case 59:
        draw_sprite_xpm(frame, original_board_x+100, original_board_y+350);
        break;
    case 60:
        draw_sprite_xpm(frame, original_board_x+150, original_board_y+350);
        break;
    case 61:
        draw_sprite_xpm(frame, original_board_x+200, original_board_y+350);
        break;
    case 62:
        draw_sprite_xpm(frame, original_board_x+250, original_board_y+350);
        break;
    case 63:
        draw_sprite_xpm(frame, original_board_x+300, original_board_y+350);
        break;
    case 64:
        draw_sprite_xpm(frame, original_board_x+350, original_board_y+350);
        break;
    default:
        break;
    }
    */
}

void draw_selected(){
    switch (current_boat)
    {
    case 2:
        if (vert){
            draw_sprite_xpm(boat_up, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_down, mouse_info.x, mouse_info.y+50);
        }
        else{
            draw_sprite_xpm(boat_left, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_right, mouse_info.x+50, mouse_info.y);
        }
        break;
    case 3:
        if (vert){
            draw_sprite_xpm(boat_up, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_middle_vert, mouse_info.x, mouse_info.y+50);
            draw_sprite_xpm(boat_down, mouse_info.x, mouse_info.y+100);
        }
        else{
            draw_sprite_xpm(boat_left, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_middle_hor, mouse_info.x+50, mouse_info.y);
            draw_sprite_xpm(boat_right, mouse_info.x+100, mouse_info.y);
        }
        break;
    case 4:
        if (vert){
            draw_sprite_xpm(boat_up, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_middle_vert, mouse_info.x, mouse_info.y+50);
            draw_sprite_xpm(boat_middle_vert, mouse_info.x, mouse_info.y+100);
            draw_sprite_xpm(boat_down, mouse_info.x, mouse_info.y+150);
        }
        else{
            draw_sprite_xpm(boat_left, mouse_info.x, mouse_info.y);
            draw_sprite_xpm(boat_middle_hor, mouse_info.x+50, mouse_info.y);
            draw_sprite_xpm(boat_middle_hor, mouse_info.x+100, mouse_info.y);
            draw_sprite_xpm(boat_right, mouse_info.x+150, mouse_info.y);
        }
        break;
    default:
        break;
    }
}

void draw_mouse() {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
}

void draw_x(){
    draw_sprite_xpm(XIco, 0, 0);
}

void draw_title_selection(){    
    if (selected==1){
        draw_sprite_xpm(player1, 300, 300);
    }
    else if (selected==2){
        draw_sprite_xpm(player2, 300, 336);
    }
}

int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

    secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    drawing_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    title_screen_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    game_board_backround_buffer=(uint8_t *) malloc(frame_buffer_size);
    return 0;
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
