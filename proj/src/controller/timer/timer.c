#include <lcom/lcf.h>

#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int timerCount = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(TIMER_FREQ < freq || freq < 19) return 1;

  uint8_t msb = 0;
  uint8_t lsb= 0;
  uint8_t st = 0;
  uint16_t val = TIMER_FREQ / freq;
  if(timer_get_conf(timer, &st)!=0)return 1;

  st = (st &0x0f) | TIMER_LSB_MSB; 

  if(sys_outb(TIMER_CTRL, timer<<6 | TIMER_LSB_MSB | (st & (BIT(3)|TIMER_SQR_WAVE|BIT(0))))!=0) return 1;
  if(util_get_LSB(val, &lsb)!=0) return 1;
  if(util_get_MSB(val, &msb)!= 0) return 1;
  if(sys_outb(TIMER_0+timer, lsb)!= 0) return 1;
  if(sys_outb(TIMER_0+timer, msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_interrupts)() {
  /* The implementation of timer_subscribe_int() is straightforward. You must call the sys_irqsetpolicy() kernel call, which is described in the paragraph
      on interrupt subscription in Subsection 5.3 . The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so that the generic interrupt handler
      will acknowledge the interrupt, i.e. ouput the EOI command to the PIC, thus enabling further interrupts on the corresponding IRQ line. You need not call
      sys_irqenable(), because the current implementation of sys_irqsetpolicy() already enables the corresponding interrupt. timer_subscribe_int() must return, 
      via its input argument, the value that it has passed to the kernel in the third argument of sys_irqsetpolicy(). */
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  /* The implementation of timer_unsusbscribe_int() is even simpler; 
  you just need to call sys_irqrmpolicy(), which is described in the paragraph on interrupt subscription in Subsection 5.3*/
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;

  return 0;
}

void (timer_int_handler)() {
  /* Thus all timer_int_handler() needs to do is to increment a global counter variable */
  timerCount++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2 || st == NULL) {return 1;}
  if(sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)) != 0) return 1;
  if (util_sys_inb(TIMER_0+timer, st) != 0) {return 1;}
  return 0;
}
