#include "i8042.h"
#include "keyboard.h"
#include <lcom/lcf.h>

int keyboard_hook_id = 1;
uint8_t scancode = 0;

// subscribe interrupts
int (keyboard_subscribe_interrupts)()
{
  // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, 
                          &keyboard_hook_id);
}

// unsubscribe interrupts
int (keyboard_unsubscribe_interrupts) ()
{
  return sys_irqrmpolicy(&keyboard_hook_id);
}

int (keyboard_restore)()
{
  uint8_t commandWord;

  // Leitura da configuração atual
  if (write_KBC_command(0x64, 0x20) != 0)
    return 1;  // notificar o i8042 da leitura
    
  if (read_KBC_output(0x60, &commandWord) != 0)
    return 1;  // ler a configuração

  // Activar o bit das interrupções
  commandWord = commandWord | BIT(0);

  // Escrita da nova configuração
  if (write_KBC_command(0x64, 0x60) != 0)
    return 1;  // notificar o i8042 da escrita
    
  if (write_KBC_command(0x60, commandWord) != 0)
    return 1;  // escrever a configuração
    
  return 0;
}

void (kbc_ih)() {
  read_KBC_output(KBC_OUT_CMD, &scancode);
}
