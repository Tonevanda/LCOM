#ifndef _mouse_h_
#define _mouse_h_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC_Mouse.h"

#define MOUSE_INTERRUPT BIT(2)

typedef struct {
    uint8_t right_click;
    uint8_t left_click;
    int16_t x;
    int16_t y;
} MouseInfo;   

/**
 * @brief Subscribes mouse interrupts
 * @return 0 if successful
 */
int (mouse_subscribe_int)();

/**
 * @brief Unsubscribes mouse interrupts
 * @return 0 if successful
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Checks the state of the mouse interrupt and stores the current byte accordingly in the mouseBytes array
 */
void (mouse_sync_bytes)();

/**
 * @brief When the mouse packet is complete, updates the mouse info values.
 */
void (mouse_sync_mouseInfo)();

/**
 * @brief Writes the command passed as parameter to the according port to configure the mouse
 * @param command Represents the desired command to be written
 * @return 0 if successful, 1 otherwise
 */
int (mouse_write)(uint8_t command);

#endif
