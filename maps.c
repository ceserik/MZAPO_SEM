
#define _POSIX_C_SOURCE 200112L
#include "maps.h"
#include "LCD_lib.h"
#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


void welcome_screen(uint16_t *matrix,int color){

    int size = 5;
    int x = 24;
    int y = 0;
    write_background(matrix);
    clock_nanosleep
    write_char(matrix,24,100,B,color,size);
    write_char(matrix,24 +(size*9)*1,100,U,color,size);
    write_char(matrix,24 +(size*9)*2,100,L,color,size);
    write_char(matrix,24 +(size*9)*3,100,A,color,size);
    write_char(matrix,24 +(size*9)*4,100,N,color,size);
    write_char(matrix,24 +(size*9)*5,100,C,color,size);
    write_char(matrix,24 +(size*9)*6,100,I,color,size);
    write_char(matrix,24 +(size*9)*8,100,three,color,size);
    
}

void map1(uint16_t *matrix, int color)
{
    write_horizon_line(matrix, 120, 60, color, 20, 120);
    write_vert_line(matrix, 100, 40, color, 20, 60);      
    write_horizon_line(matrix, 120, 180, color, 20, 120);
    write_vert_line(matrix, 240, 160, color, 20, 60);
    write_horizon_line(matrix, 120, 260, color, 20, 120);
    write_horizon_line(matrix, 360, 40, color, 20, 20);
    write_horizon_line(matrix, 360, 100, color, 20, 20);
    write_horizon_line(matrix, 360, 160, color, 20, 20);
    write_horizon_line(matrix, 360, 220, color, 20, 20);
    write_horizon_line(matrix, 360, 280, color, 20, 20);
}

void map2(uint16_t *matrix, int color) // DUST 2
{
    write_blank(matrix,0,0,480,320);
    write_horizon_line(matrix, 0, 0, 0xccc4, 3, 480);
    write_horizon_line(matrix, 0, 317, 0xccc4, 3, 480);
    write_vert_line(matrix, 477, 0, 0xccc4, 3, 320);
    write_vert_line(matrix, 0, 0, 0xccc4, 3, 320);
    write_horizon_line(matrix, 0, 60, 0xccc4, 20, 40);
    write_horizon_line(matrix, 80, 60, 0xccc4, 20, 180);
    write_horizon_line(matrix, 340, 60, 0xccc4, 20, 140);
    write_horizon_line(matrix, 0, 220, 0xccc4, 20, 180);
    write_horizon_line(matrix, 380, 220, 0xccc4, 20, 100);
    write_horizon_line(matrix, 380, 220, 0xccc4, 20, 100);
    write_horizon_line(matrix, 180, 180, 0xEED6, 60, 60);
    write_horizon_line(matrix, 80, 80, 0xEED6, 40, 40);
    write_horizon_line(matrix, 40, 160, 0xEED6, 20, 40);
    write_horizon_line(matrix, 40, 280, 0xEED6, 40, 40);
    //CAR
    write_horizon_line(matrix, 400, 120, 0xC618, 60, 40);
    //Barrels
    write_horizon_line(matrix, 180, 240, 0x0019, 20, 20); //B
    write_horizon_line(matrix, 200, 240, 0xC800, 20, 20); //R
    write_horizon_line(matrix, 200, 260, 0x1660, 20, 20); //G
    write_char(matrix, 200, 190, B, 0xF800, 3);
}

/*
void WriteDefault(unsigned short *matrix)
{
  write_horizon_line(matrix, 0, 0, 0x07E0, 3, 480);
  write_horizon_line(matrix, 0, 317, 0x07E0, 3, 480);
  write_vert_line(matrix, 477, 0, 0x07E0, 3, 320);
  write_vert_line(matrix, 0, 0, 0x07E0, 3, 320);

  int size = 3;
  // SPEED=
  write_char(matrix, 10, 0, S, 0x07E0, size);
  write_char(matrix, 10 + (1 * (size + 1) * 8), 0, P, 0x07E0, size);
  write_char(matrix, 10 + (2 * (size + 1) * 8), 0, E, 0x07E0, size);
  write_char(matrix, 10 + (3 * (size + 1) * 8), 0, E, 0x07E0, size);
  write_char(matrix, 10 + (4 * (size + 1) * 8), 0, D, 0x07E0, size);
  write_char(matrix, 10 + (5 * (size + 1) * 8), 0, Equal, 0x07E0, size);
  // SET =
  write_char(matrix, 10 + (0 * (size + 1) * 8), (1 * (size + 1) * 16), S, 0x07E0, size);
  write_char(matrix, 10 + (1 * (size + 1) * 8), (1 * (size + 1) * 16), E, 0x07E0, size);
  write_char(matrix, 10 + (2 * (size + 1) * 8), (1 * (size + 1) * 16), T, 0x07E0, size);
  write_char(matrix, 10 + (3 * (size + 1) * 8), (1 * (size + 1) * 16), Equal, 0x07E0, size);
  //DIFF
  write_char(matrix, 10 + (0 * (size + 1) * 8), (2 * (size + 1) * 16), D, 0x07E0, size);
  write_char(matrix, 10 + (1 * (size + 1) * 8), (2 * (size + 1) * 16), I, 0x07E0, size);
  write_char(matrix, 10 + (2 * (size + 1) * 8), (2 * (size + 1) * 16), F, 0x07E0, size);
  write_char(matrix, 10 + (3 * (size + 1) * 8), (2 * (size + 1) * 16), F, 0x07E0, size);
  write_char(matrix, 10 + (5 * (size + 1) * 8), (2 * (size + 1) * 16), Equal, 0x07E0, size);

  // P=
  write_char(matrix, 10, (3 * (size + 1) * 16), P, 0x07E0, size);
  write_char(matrix, 10 + (1 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
  // I=
  write_char(matrix, 10 + (5 * (size + 1) * 8), (3 * (size + 1) * 16), I, 0x07E0, size);
  write_char(matrix, 10 + (6 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
  // D=
  write_char(matrix, 10 + (10 * (size + 1) * 8), (3 * (size + 1) * 16), D, 0x07E0, size);
  write_char(matrix, 10 + (11 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
}
*/