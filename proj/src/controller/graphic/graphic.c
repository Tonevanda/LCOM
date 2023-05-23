#include <lcom/lcf.h>
#include "graphic.h"

unsigned int get_bytes_per_pixel(){
  return (mode_info.BitsPerPixel + 7) / 8;
}

int (set_graphic_mode)(uint16_t submode) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;              
  reg86.ah = 0x4f;                 
  reg86.al = 0x02;                 
  reg86.bx = submode | BIT(14);
  if (sys_int86(&reg86) != 0) return 1;
  
  return 0;
}

int (set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer){
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info)) return 1;

  struct minix_mem_range physAddr;

  unsigned int bytes_per_pixel = get_bytes_per_pixel();
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
  physAddr.mr_base = mode_info.PhysBasePtr;
  physAddr.mr_limit = physAddr.mr_base + frame_size;
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physAddr)) return 1;

  *frame_buffer = vm_map_phys(SELF, (void*) physAddr.mr_base, frame_size);
  if (frame_buffer == NULL) return 1;

  return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color,uint8_t* frame_buffer) {
  for(unsigned i = 0; i < height ; i++)
    if (draw_hline(x, y+i, width, color,frame_buffer) != 0) return 1;
  return 0;
}

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color,uint8_t* frame_buffer){
    for (unsigned i = 0 ; i < len ; i++)   
      if (draw_pixel(x+i, y, color,frame_buffer) != 0) return 1;
    return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer) {

  if( (x > mode_info.XResolution) || (y > mode_info.YResolution) ) return 1;
  
  unsigned BytesPerPixel = get_bytes_per_pixel();
  unsigned int idx = (mode_info.XResolution * y + x) * BytesPerPixel;

  if (memcpy(&frame_buffer[idx], &color, BytesPerPixel) == NULL) return 1;

  return 0;
}


int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y,uint8_t * buffer) {

  xpm_image_t img;
  uint32_t *sprite = (uint32_t*) xpm_load(xpm, XPM_8_8_8_8, &img);
  
  for(int indexY=0;indexY<img.height;indexY++){
    for(int indexX=0;indexX<img.width;indexX++){
      if(draw_pixel(indexX + x, indexY + y, *sprite, buffer)) return 1;
      sprite++;
    }
  }

  return 0;
}
