#include <lcom/lcf.h>
#include "controller/timer/timer.h"
#include "controller/graphic/graphic.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
//#include "controller/rtc/rtc.h"
#include "model/model.h"
#include "draw/draw.h"
#include "config.h"


extern SystemState systemState;
extern uint8_t scancode;
extern uint8_t mouseBytes[3];
extern MouseInfo mouse_info;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int setup(){
  
  if (timer_set_frequency(TIMER_SEL0, GAME_FPS) != 0) return 1;
  if (set_frame_buffers(VIDEO_MODE) != 0) return 1;
  if (set_graphic_mode(VIDEO_MODE) != 0) return 1;

  setup_sprites();
  setup_backround();
  if (timer_subscribe_interrupts() != 0) return 1;
  if (keyboard_subscribe_interrupts() != 0) return 1;
  if (mouse_subscribe_int() != 0) return 1;

  if (mouse_write(ENABLE_STREAM_MODE) != 0) return 1;
  if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int turnoff(){

  if (vg_exit() != 0) return 1; 

  destroy_sprites();

  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  
  if(setup() != 0) return 1;
  

  int ipc_status;
  message msg;
  
  while (systemState == RUNNING) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }
  //print_xpm((xpm_map_t)plus_xpm, 100, 100,secondary_frame_buffer);
    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & TIMER_MASK){
            //destroy_sprites(&secondary_frame_buffer,frame_buffer_size);
            update_timer_state();
          }    
          if (msg.m_notify.interrupts & KEYBOARD_MASK){
            update_keyboard_state();
            kbd_print_scancode(!(scancode & MAKE_CODE), 1, &scancode);
          }
          if (msg.m_notify.interrupts & MOUSE_MASK){
            //systemState = EXIT;
            update_mouse_state();  
            
          }    
          //if (msg.m_notify.interrupts & RTC_MASK) update_rtc_state();
        }
    }
  }
  
  //sleep(3);
  if(turnoff() != 0) return 1;
  
  return 0;
}
