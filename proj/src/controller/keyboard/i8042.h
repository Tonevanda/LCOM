#ifndef _i8042_h_kbd_
#define _i8042_h_kbd_
#include <lcom/lcf.h>

#define IRQ_KEYBOARD    1

uint32_t count;

#define BREAK_ESC       0x81
#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10


#define ESC_Key          1
#define ONE_KEY          2
#define TWO_KEY          3
#define THREE_KEY        4
#define FOUR_KEY         5
#define Q_KEY           16
#define W_KEY           17
#define A_KEY           30
#define S_KEY           31
#define D_KEY           32
#define E_KEY           18
#define R_KEY           19

#endif
