// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include <lcom/timer.h>
#include "graphic.h"
#include "keyboard.h"
#include "VBE.h"

// modes
#define one 0x105
#define two 0x110
#define three 0x115
#define four 0x11A
#define five 0x14C

#define RETURN_VBE_MODE 0x01
#define SET_VBE_MODE 0x02
#define BX_REGISTER BIT(14)

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int pressed_escape(){
  int ipc_status;
  message msg;
  int r = 0;
  uint8_t bit_no = 0x00;
  output = 0;

  if(keyboard_subscribe_interrupts(&bit_no) != 0){
    return 1;
  }

  while(output != BREAK_ESC){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:				
          if (msg.m_notify.interrupts & bit_no) { 
            kbc_ih();
          }
          break;
        default:
          break;
       }
    }
  }
  if(keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_graphic_mode(mode) != 0) return 1;
  sleep(delay);
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color) {

  if (set_frame_buffer(mode) != 0) return 1; // passo 1
  if (set_graphic_mode(mode) != 0) return 1; // passo 2

  uint32_t new_color;
  if (normalize_color(color, &new_color) != 0) return 1;

  if (vg_draw_rectangle(x, y, width, height, new_color) != 0) return 1; // passo 3

  if (pressed_escape() != 0) return 1; //vai ficar num loop até clicar em escape
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (set_frame_buffer(mode) != 0) return 1; // passo 1
  if (set_graphic_mode(mode) != 0) return 1; // passo 2

  // Cálculo do número inteiro de rectângulos em cada eixo
  int vertical = mode_info.YResolution / no_rectangles;
  int horizontal = mode_info.XResolution / no_rectangles;

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {

      uint32_t color;

      if (mode_info.MemoryModel == DIRECT_COLOR) {
        uint32_t R = Red(j, step, first);
        uint32_t G = Green(i, step, first);
        uint32_t B = Blue(j, i, step, first);
        color = direct_mode(R, G, B);

      } else {
        color = indexed_mode(j, i, step, first, no_rectangles);
      }

      if (vg_draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color)) return 1;
    }
  }

  if (pressed_escape() != 0) return 1; //vai ficar num loop até clicar em escape
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (set_frame_buffer(0x105) != 0) return 1; // passo 1
  if (set_graphic_mode(0x105) != 0) return 1; // passo 2

  if(print_xpm(xpm,x,y)!= 0) return 1;

  if (pressed_escape() != 0) return 1; //vai ficar num loop até clicar em escape
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

  int ipc_status;
  message msg;
  int r = 0;
  uint8_t bit_no = 0x00;
  uint8_t irq_set_TIMER = 0x00;
  output = 0;

  uint8_t vertical_direction;
  if (xi == xf && yi < yf) vertical_direction = 1;
  else if (yi == yf && xi < xf) vertical_direction = 0;
  else return 1;

  if (set_frame_buffer(0x105) != 0) return 1; // passo 1
  if (set_graphic_mode(0x105) != 0) return 1; // passo 2

  if (keyboard_subscribe_interrupts(&bit_no) != 0) return 1;
  if (timer_subscribe_int(&irq_set_TIMER) != 0) return 1;

  // Atualiza o sistema para a frame rate dada
  if (timer_set_frequency(0, fr_rate) != 0) return 1;   

  if (print_xpm(xpm, xi, yi) != 0) return 1; //posição inicial

  while(output != BREAK_ESC || (xi < xf || yi < yf)){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:				
          if (msg.m_notify.interrupts & bit_no) { 
            kbc_ih();
          }
          if (msg.m_notify.interrupts & irq_set_TIMER) {
            // Apaga o desenho anterior para dar a sensação de movimetno
            if (vg_draw_rectangle(xi, yi, 100, 100, 0xFFFFFF) != 0) return 1;
            // Atualiza o valor da variável a incrementar
            if (vertical_direction) {
              yi += speed;
                if (yi > yf) yi = yf;
            } else {
                xi += speed;
                if (xi > xf) xi = xf;
            }
            // Imprime a nova imagem ligeiramente deslocada
            if (print_xpm(xpm, xi, yi) != 0) return 1;
          }
          break;
        default:
          break;
       }
    }
  }
  
  if (vg_exit() != 0) return 1;

  if(keyboard_unsubscribe_interrupts() != 0) return 1;
  if(timer_unsubscribe_int() != 0) return 1;
  return 0;
}
