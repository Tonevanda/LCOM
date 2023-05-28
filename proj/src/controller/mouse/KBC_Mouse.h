#ifndef _kbc_mouse_h_
#define _kbc_mouse_h_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

/**
 * @brief Reads the status of the KBC and stores it in the variable passed as parameter
 * @param status Represents the variable in which the KBC status will be stored
 * @return 0 if successful, 1 otherwise
 */
int (read_KBC_Mouse_status)(uint8_t* status);

/**
 * @brief Reads the content of the KBC in the port passed as parameter and stores it in the output variable
 * @param port Represents the port to be read
 * @param output Represents the variable in which the data read in the port will be stored
 * @return 0 if successful, 1 otherwise
 */
int (read_KBC_Mouse_output)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes the command passed as parameter to the desired port
 * @param port Represents the port which the command will be written to
 * @param commandByte Represents the command written to the port
 * @return 0 if successful, 1 otherwise
 */
int (write_KBC_Mouse_command)(uint8_t port, uint8_t commandByte);

#endif
