#include "i8042.h"
#include "keyboard.h"
#include <lcom/lcf.h>

int keyboard_hook_id = 1;
uint8_t scancode = 0;

int (keyboard_subscribe_interrupts)(){
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_interrupts) (){
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
  read_KBC_Keyboard_output(KBC_OUT_CMD, &scancode);
}

int (keyboard_restore)(){
  uint8_t cmdWord;
  
  if (write_KBC_Keyboard_command(0x64, 0x20) != 0) return 1;
  if (read_KBC_Keyboard_output(0x60, &cmdWord) != 0) return 1; 

  cmdWord |= BIT(0);

  if (write_KBC_Keyboard_command(0x64, 0x60) != 0) return 1;
  if (write_KBC_Keyboard_command(0x60, cmdWord) != 0) return 1;
    
  return 0;
}
