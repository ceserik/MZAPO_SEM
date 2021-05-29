#ifndef LCD_lib_H
#define LCD_lib_H

#include <stdint.h>

void write_char(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t* c, uint16_t color,int size);
void refresh_lcd(unsigned char* membase, uint16_t * matrix);
void WriteLine(int angle,int x, int y,int size);
void write_horizon_line(uint16_t* matrix, int x, int y, uint16_t color, int size, int length);
void write_vert_line(uint16_t* matrix, int x, int y, uint16_t color, int size, int length);
void write_val(uint16_t *matrix, uint8_t number, int x, int y, int size, int color,int transparent);
void write_blank(uint16_t *matrix, int x, int y, int EndX, int EndY );
void write_background(uint16_t* matrix);

#endif  