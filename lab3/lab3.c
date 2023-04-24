#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "keyboard.h"
#include "KBC.h"
#include "i8042.h"
#include "i8254.h"
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;
  int r = 0;
  uint8_t bit_no = 0x00;
  uint8_t size = 1;
  count = 0;
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

            if(output == TWO_BYTES){
              size = 2;
            }

            kbd_print_scancode(!(output & MAKE_CODE), size, &output);
          }
          break;
        default:
          break;
       }
    }
  }
  if(keyboard_unsubscribe_interrupts() != 0) return 1;
  if(kbd_print_no_sysinb(count)!= 0)return 1;
  return 0;
}

int(kbd_test_poll)() {
  output = 0;
  count = 0;
  uint8_t size = 1;

  while(output != BREAK_ESC){
    read_KBC_output(KBC_OUT_CMD, &output);
    if(output == TWO_BYTES){
      size = 2;
    }
    kbd_print_scancode(!(output & MAKE_CODE), size, &output);
  }
  if(kbd_print_no_sysinb(count)!= 0)return 1;
  keyboard_restore();
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  message msg;
  int r = 0;
  uint8_t bit_no = 0x00;
  uint8_t bit_noTimer = 0;
  uint8_t size = 1;
  count = 0;
  output = 0;
  timerCount = 0;
  if(timer_subscribe_int(&bit_noTimer) != 0) return 1;
  if(keyboard_subscribe_interrupts(&bit_no) != 0) return 1;
  int bit_setTimer = BIT(bit_noTimer); 

  while((output != BREAK_ESC) && ((timerCount/60) < n)){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & bit_setTimer) { 
            timer_int_handler();
          }

          if (msg.m_notify.interrupts & bit_no) {
            kbc_ih();
            timerCount = 0;
            if(output == TWO_BYTES){
              size = 2;
            }
            kbd_print_scancode(!(output & MAKE_CODE), size, &output);
          }
          break;
        default:
          break;
       }
    }
  }
  if(timer_unsubscribe_int()!=0)return 1;
  if(keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}
