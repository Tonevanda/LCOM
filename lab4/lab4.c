#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "i8254.h"
#include "KBC.h"
#include "mouse.h"
#include <lcom/timer.h>

typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
} SystemState;

SystemState state = START;
uint16_t x_len_total = 0;
uint8_t x_len_;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status;
    message msg;
    int r = 0;
    uint8_t bit_no = 0x00;
    byteIndex = 0;

    if(mouse_subscribe_int(&bit_no) != 0){
        return 1;
    }
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;

    while(cnt){
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:				
                if (msg.m_notify.interrupts & bit_no) { 
                    mouse_ih();
                    mouse_sync_bytes();
                    if (byteIndex == 3) {                    // Quando tivermos três bytes do mesmo pacote
                        mouse_bytes_to_packet();                // Formamos o pacote
                        mouse_print_packet(&mouse_packet);      // Mostramos o pacote
                        byteIndex = 0;
                        cnt--;
            }
                }
                break;
                default:
                break;
            }
        }
    }
    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

    if(mouse_unsubscribe_int() != 0) return 1;
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    int ipc_status;
    message msg;
    uint8_t bit_noTimer = 0;
    int r = 0;
    uint8_t bit_no = 0x00;
    byteIndex = 0;
    timerCount = 0;

    if(timer_subscribe_int(&bit_noTimer) != 0) return 1;
    if(mouse_subscribe_int(&bit_no) != 0){
        return 1;
    }
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;
    int bit_setTimer = BIT(bit_noTimer); 

    while((timerCount/60) < idle_time){
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
                    mouse_ih();
                    mouse_sync_bytes();
                    timerCount = 0;
                    if (byteIndex == 3) {                    // Quando tivermos três bytes do mesmo pacote
                        mouse_bytes_to_packet();                // Formamos o pacote
                        mouse_print_packet(&mouse_packet);      // Mostramos o pacote
                        byteIndex = 0;
                        
                    }
                }
                break;
                default:
                break;
            }
        }
    }
    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

    if(mouse_unsubscribe_int() != 0) return 1;
    return 0;
}


void (update_state_machine)(uint8_t tolerance) {

    switch (state) {

      case START:
          if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) { // I
            state = UP;
          }
          break;

      case UP:

          if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
            if((mouse_packet.delta_y < (0-tolerance)) || (mouse_packet.delta_x < (0-tolerance))){ //II
              state = START;
              x_len_total=0;
            }
          }
          else if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) { // III
            state = VERTEX;
          }
          else{ // F
            state=START;
          }
          break;

      case VERTEX:
          if(mouse_packet.rb && !mouse_packet.lb && !mouse_packet.mb){ // IV
            state=DOWN;
          }
          else{ //F
            state = START;
          }
          break;

      case DOWN:
          if(mouse_packet.rb && !mouse_packet.lb && !mouse_packet.mb){ // V
            if((mouse_packet.delta_y > (tolerance)) || (mouse_packet.delta_x < (0-tolerance))){ //F
              state = START;
              x_len_total=0;
            }
          }
          else if(!mouse_packet.rb && !mouse_packet.lb && !mouse_packet.mb && mouse_packet.delta_x>=x_len_){ //VI
              state=END;
          }
          else{ // F
            state = START;
          } 
            
          break;

      case END:
          break;
    }
    
    if((x_len_total + mouse_packet.delta_x > 0) && (x_len_total + mouse_packet.delta_x <= x_len_)){
       x_len_total+=mouse_packet.delta_x;
    }
    else{
      x_len_total = 0;
    }
    
    
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    int ipc_status;
    message msg;
    int r = 0;
    uint8_t bit_no = 0x00;
    byteIndex = 0;
    x_len_ = x_len;

    if(mouse_subscribe_int(&bit_no) != 0){
        return 1;
    }
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;

    while(state != END){
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:				
                if (msg.m_notify.interrupts & bit_no) { 
                    mouse_ih();
                    mouse_sync_bytes();
                    if (byteIndex == 3) {                    // Quando tivermos três bytes do mesmo pacote
                        mouse_bytes_to_packet();                // Formamos o pacote
                        mouse_print_packet(&mouse_packet);      // Mostramos o pacote
                        update_state_machine(tolerance);
                        byteIndex = 0;
            }
                }
                break;
                default:
                break;
            }
        }
    }
    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;
    if(mouse_unsubscribe_int() != 0) return 1;
    return 0;
}
