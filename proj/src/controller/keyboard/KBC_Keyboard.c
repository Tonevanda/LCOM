#include <stdint.h>
#include "i8042.h"
#include "KBC_Keyboard.h"

int (read_KBC_Keyboard_status)(uint8_t* status){ 
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_KBC_Keyboard_output)(uint8_t port, uint8_t *output)
{
  uint8_t status;
  uint8_t try = 10;

  while (try) {
    if (read_KBC_Keyboard_status(&status) != 0) return 1;
    if((status & BIT(7)) != 0) return 1;
    if ((status & BIT(6)) != 0) return 1;

    if ((status & BIT(0)) != 0) {
      if (util_sys_inb(port, output) != 0) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    try--;
  }
 
  return 1;
}

int (write_KBC_Keyboard_command)(uint8_t port, uint8_t commandByte)
{
  uint8_t status;
  uint8_t try = 10;

  while (try) {
    if (read_KBC_Keyboard_status(&status) != 0) return 1;

    if ((status & BIT(1)) == 0) {
      if (sys_outb(port, commandByte) != 0) return 1;
      return 0;
    }
    
    tickdelay(micros_to_ticks(20000));              
    try--;
  }
  
  return 1;
}
