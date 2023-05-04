#ifndef _DRAW_H_
#define _DRAW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/graphic/graphic.h"
#include "controller/mouse/mouse.h"
//#include "controller/rtc/rtc.h"

void swap_buffers();
int set_frame_buffers(uint16_t mode);

#endif
