#include <lcom/lcf.h>

#define RTC_IRQ 8
#define RTC_INTERRUPT BIT(8)

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

// Valores de acesso
#define RTC_SEC 0
#define RTC_SEC_A 1
#define RTC_MIN 2
#define RTC_MIN_A 3
#define RTC_HOURS 4
#define RTC_HOURS_A 5
#define RTC_WEEK_DAY 6
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9

// Reg
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

// Configuração
#define RTC_UIP BIT(7)
#define RTC_SET BIT(7)
#define RTC_UIE BIT(4)

// Interrupções
#define RTC_UF BIT(4) // Update Interrupt Pending
#define RTC_AF BIT(5) // Alarm Interrupt Pending
#define RTC_PF BIT(6) // Periodic Interrupt Pending

/**
 * @brief Enables/Disables the interrupt updates for the RTC
 * @param enable True to enable the interrupt updates, false to disable
 * @return 0 if successful 
 */
int(enable_int_update)(bool enable);

/**
 * @brief Subscribes interrupts for the RTC
 * @return 0 if successful
 */
int(rtc_subscribe_int)();

/**
 * @brief Unsubscribes interrupts for the RTC
 * @return 0 if successful
 */
int(rtc_unsubscribe_int)();

/**
 * @brief RTC interrupt handler. Reads every field of the RTC.
 * 
 */
void(rtc_ih)();

/**
 * @brief Waits for the RTC to update its values so it doesn't read the values during an update.
 * @return 0 if successful
 */
int(wait_rtc)();

/**
 * @brief Converts BCD to decimal
 * @param bcdNum Number in BCD
 * @return uint8_t Number now converted do decimal
 */
uint8_t(bcd2dec)(uint8_t bcdNum);

/**
 * @brief Reads the values in the RTC and updated the rtc_data array with said values
 * @param val command to decide which value to read from the RTC
 * @param pos position in which the value read from the RTC will be stored in the rtc_data array
 * @return 0 if successful
 */
int(rtc_read)(uint8_t reg, uint8_t pos);
