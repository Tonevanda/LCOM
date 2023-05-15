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

int selected=0;

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

    draw_mouse();
    //draw_sprite_xpm(plus, mode_info.XResolution/2 - 100, mode_info.YResolution/2 - 100);  
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
