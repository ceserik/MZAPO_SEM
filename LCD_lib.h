#ifndef LCD_lib_H
#define LCD_lib_H

#include <stdint.h>

extern uint16_t S[16];
extern uint16_t C[16];
extern uint16_t O[16];

void WriteChar(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t* c, uint16_t color);
int RefreshLCD(unsigned char* membase, uint16_t * matrix);

#endif  