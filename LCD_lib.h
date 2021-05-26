#ifndef LCD_lib_H
#define LCD_lib_H

#include <stdint.h>

extern uint16_t S[16];
extern uint16_t P[16];
extern uint16_t E[16];
extern uint16_t D[16];
extern uint16_t C[16];
extern uint16_t O[16];
extern uint16_t Equal[16];
extern uint16_t I[16];
extern uint16_t F[16];
extern uint16_t T[16];
extern uint16_t one[16];
extern uint16_t two[16];
extern uint16_t three[16];
extern uint16_t four[16];
extern uint16_t five[16];
extern uint16_t six[16];
extern uint16_t seven[16];
extern uint16_t eight[16];
extern uint16_t nine[16];
extern uint16_t zero[16];
extern uint16_t SPACE[16];
extern uint16_t B[16];
extern uint16_t U[16];
extern uint16_t L[16];
extern uint16_t A[16];
extern uint16_t N[16];


void WriteChar(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t* c, uint16_t color,int size);
void refresh_lcd(unsigned char* membase, uint16_t * matrix);
void WriteLine(int angle,int x, int y,int size);
void WriteLineHorizon(uint16_t* matrix, int x, int y, uint16_t color, int size, int length);
void WriteLineVert(uint16_t* matrix, int x, int y, uint16_t color, int size, int length);
void WriteVal(uint16_t *matrix,  uint8_t number, int x, int y, int size, int color);
void write_blank(uint16_t *matrix, int x, int y, int EndX, int EndY );
void WriteBackground(uint16_t* matrix);

#endif  