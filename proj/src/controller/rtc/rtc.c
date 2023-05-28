#include "rtc.h"

uint8_t rtc_data[6]; // sec, min, hour, day, month, year
static int rtc_hook_id = 8;


int enable_int_update(bool enable) {
    uint32_t reg;

    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_inb(RTC_DATA_REG, &reg);

    if (enable)
        reg |= RTC_UIE;
    else
        reg &= ~RTC_UIE;

    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_outb(RTC_DATA_REG, reg);

    return 0;
}


int(rtc_subscribe_int)() {
    sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);
    return 0;
}


int(rtc_unsubscribe_int)() {
    sys_irqrmpolicy(&rtc_hook_id);
    return 0;
}


void(rtc_ih)() {
    uint32_t reg;

    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    sys_inb(RTC_DATA_REG, &reg);

    if (RTC_UF & reg) {
        rtc_read(RTC_SEC, 0);
        rtc_read(RTC_MIN, 1);
        rtc_read(RTC_HOURS, 2);
        rtc_read(RTC_DAY, 3);
        rtc_read(RTC_MONTH, 4);
        rtc_read(RTC_YEAR, 5);
    }
}


int wait_rtc() {
    uint32_t reg = 0;

    do {

        sys_irqdisable(&rtc_hook_id);
        sys_outb(RTC_ADDR_REG, RTC_REG_A);
        sys_inb(RTC_DATA_REG, &reg);
        sys_irqenable(&rtc_hook_id);

    } while (RTC_UIP & reg);

    return 0;
}


uint8_t bcd2dec(uint8_t bcdNum) {
    return (bcdNum - 6 * (bcdNum >> 4));
}


int rtc_read(uint8_t val, uint8_t pos) {

    wait_rtc();


    uint32_t read;
    sys_outb(RTC_ADDR_REG, val);
    sys_inb(RTC_DATA_REG, &read);


    rtc_data[pos] = bcd2dec(read);

    return 0;
}
