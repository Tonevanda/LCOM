#include "model.h"

extern uint8_t scancode;
extern uint8_t byteIndex;
SystemState systemState = RUNNING;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;

int timer_interrupts = 0;

void setup_sprites() {
}

void destroy_sprites() {
}

void update_timer_state() {
    if (DOUBLE_BUFFER) swap_buffers();
    timer_interrupts++;
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
