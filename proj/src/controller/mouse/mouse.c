#include "mouse.h"
#include <lcom/lcf.h>
#include "i8042.h"
#include <lcom/timer.h>
#include <stdint.h>

uint8_t currentByte;
uint8_t byteIndex;
uint8_t mouseBytes[3];
int mouse_hook_id = 2;
MouseInfo mouse_info = {0, 0, 100, 100};
extern vbe_mode_info_t mode_info;

int (mouse_subscribe_int)(){
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(){
    read_KBC_Mouse_output(KBC_WRITE_CMD, &currentByte, 1);
}

void (mouse_sync_bytes)(){
  if((byteIndex == 0 && (currentByte & FIRST_BYTE)) || byteIndex > 0){
    mouseBytes[byteIndex]= currentByte;
    byteIndex++;
  }
  
}

void(mouseBytes_to_packet)(){
  for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = mouseBytes[i];
  }

  mouse_packet.lb = mouseBytes[0] & MOUSE_LB;
  mouse_packet.mb =mouseBytes[0] & MOUSE_MB;
  mouse_packet.rb = mouseBytes[0] & MOUSE_RB;
  mouse_packet.x_ov = mouseBytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = mouseBytes[0] & MOUSE_Y_OVERFLOW;
  mouse_packet.delta_x = (mouseBytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1];
  mouse_packet.delta_y = (mouseBytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2];
}

int (mouse_write)(uint8_t command)
{
  uint8_t attemps = 10;
  uint8_t mouse_response;

  // Enquanto houver tentativas e a resposta não for satisfatória
  do {
    attemps--;
    // Ativar do modo D4 do i8042
    if (write_KBC_Mouse_command(0x64, 0xD4))
      return 1;
    // O comando para o rato é escrito na porta 0x60
    if (write_KBC_Mouse_command(0x60, command))
    tickdelay(micros_to_ticks(20000));
    // Ler a resposta do rato pela porta 0x60
    if (read_KBC_Mouse_output(0x60, &mouse_response, 1))
      return 1;
  } while (mouse_response != 0xFA && attemps);       

  return 0;
}

void (mouse_sync_mouseInfo)(){
  mouse_info.right_click = mouseBytes[0] & MOUSE_RB;
  mouse_info.left_click = mouseBytes[0] & MOUSE_LB;

  if (mouseBytes[0] & MOUSE_X_OVERFLOW || mouseBytes[0] & MOUSE_Y_OVERFLOW) return;

  int16_t delta_x = mouse_info.x + ((mouseBytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1]);
  int16_t delta_y = mouse_info.y - ((mouseBytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2]);

  if (delta_x < 0 || delta_x > mode_info.XResolution || delta_y < 0 || delta_y > mode_info.YResolution) return;
  mouse_info.x = delta_x;
  mouse_info.y = delta_y;
}




