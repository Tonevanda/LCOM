#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(TIMER_FREQ < freq) return 1;

  uint8_t msb = 0;
  uint8_t lsb= 0;
  uint8_t st = 0;
  uint16_t val = TIMER_FREQ / freq;
  timer_get_conf(timer, &st);

  st &= 0x0f; 

  sys_outb(TIMER_CTRL, timer<<6 | TIMER_LSB_MSB | (st & (BIT(3)|TIMER_SQR_WAVE|BIT(0))));
  util_get_LSB(val, &lsb);
  util_get_MSB(val, &msb);
  sys_outb(TIMER_0+timer, lsb);
  sys_outb(TIMER_0+timer, msb);

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  /* The implementation of timer_subscribe_int() is straightforward. You must call the sys_irqsetpolicy() kernel call, which is described in the paragraph
      on interrupt subscription in Subsection 5.3 . The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so that the generic interrupt handler
      will acknowledge the interrupt, i.e. ouput the EOI command to the PIC, thus enabling further interrupts on the corresponding IRQ line. You need not call
      sys_irqenable(), because the current implementation of sys_irqsetpolicy() already enables the corresponding interrupt. timer_subscribe_int() must return, 
      via its input argument, the value that it has passed to the kernel in the third argument of sys_irqsetpolicy(). */
  hook_id = 0;
  *bit_no = hook_id;
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);

  return 0;
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
  sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  
  if (util_sys_inb(TIMER_0+timer, st) != 0) {return 1;}

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;

  if (timer < 0 || timer > 2)
    return 1;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      if((st & BIT(4)) && (st & BIT(5))){
        val.in_mode = MSB_after_LSB;
      }
      else if(st & BIT(4)){
        val.in_mode = LSB_only;
      }
      else if(st & BIT(5)){
        val.in_mode = MSB_only;
      }
      else{
        val.in_mode = INVAL_val;
      }
      st &= TIMER_LSB_MSB;
      break;
    case tsf_mode:
      st = (st & 0x0E) >> 1;
      if(st == 6) {
        val.count_mode = 2;
      }
      else if(st == 7){
        val.count_mode = 3;
      }
      else val.count_mode = st;
      break;
    case tsf_base:
      val.bcd = st & BIT(0);
      break;
    default:
      return 1;
  }

  timer_print_config(timer, field, val);
  return 0;
}
