#include "mouse.h"
#include <lcom/lcf.h>
#include "i8042.h"
#include <lcom/timer.h>
#include <stdint.h>

uint8_t currentByte;
uint8_t mouseBytes[3];
int mouse_hook_id = 2;

int (mouse_subscribe_int)(uint8_t *bit_no){
    if (bit_no == NULL)
        return 1;
  *bit_no = BIT(mouse_hook_id);
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(){
    if(read_KBC_output(KBC_WRITE_CMD, &currentByte, 1) != 0) printf("Error in reading byte from mouse\n");
}

void (mouse_sync_bytes)(){
  if((byteIndex == 0 && (currentByte & FIRST_BYTE)) || byteIndex > 0){
    mouseBytes[byteIndex]= currentByte;
    byteIndex++;
  }
  
}

void(mouse_bytes_to_packet)(){
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
    if (write_KBC_command(0x64, 0xD4))
      return 1;
    // O comando para o rato é escrito na porta 0x60
    if (write_KBC_command(0x60, command))
      return 1;
    tickdelay(micros_to_ticks(20000));
    // Ler a resposta do rato pela porta 0x60
    if (read_KBC_output(0x60, &mouse_response, 1))
      return 1;
  } while (mouse_response != 0xFA && attemps);       

  return 0;
}




