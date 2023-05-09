#include <lcom/lcf.h>
#include "graphic.h"

int set_graphic_mode(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); // inicialização da estrutura com o valor 0 em todos os parâmetros
    reg86.intno = 0x10;               // intno é sempre 0x10      
    reg86.ah = 0x4F;                  // parte mais significativa de AX
    reg86.al = 0x02;                  // parte menos significativa de AX. 0x02 no caso de modo gráfico
    reg86.bx = submode | BIT(14);     // determinação do submodo com memória linear
    if (sys_int86(&reg86) != 0) {     // se houver algum erro, abortar a função
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}

int (set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer){

  // retirar informação sobre o @mode
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info)) return 1;

  // cálculo do tamanho do frame buffer, em bytes
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
  
  // preenchimento dos endereços físicos
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr; // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; // fim físico do buffer
  
  // alocação física da memória necessária para o frame buffer
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  // alocação virtual da memória necessária para o frame buffer
  *frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer) {

  // As coordenadas têm de ser válidas
  if(x > mode_info.XResolution || y > mode_info.YResolution) return 1;
  
  // Cálculo dos Bytes per pixel da cor escolhida. Arredondamento por excesso.
  unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  // Índice (em bytes) da zona do píxel a colorir
  unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;

  // A partir da zona frame_buffer[index], copia @BytesPerPixel bytes da @color
  if (memcpy(&frame_buffer[index], &color, BytesPerPixel) == NULL) return 1;

  return 0;
}


int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y,uint8_t * buffer) {

  xpm_image_t img;
  uint32_t *sprite = (uint32_t*) xpm_load(xpm, XPM_8_8_8_8, &img);
  
  for(int Yindx=0;Yindx<img.height;Yindx++){
    for(int Xindx=0;Xindx<img.width;Xindx++){
      if(draw_pixel(x+Xindx, y+Yindx,*sprite, buffer)){return 1;}
      sprite++;
    }
  }
  return 0;
}
