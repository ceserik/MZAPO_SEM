#include "maps.h"
#include "LCD_lib.h"


void welcome_screen(uint16_t *matrix,int color){
    int size = 5;
    int x = 24;
    int y = 0;
    WriteBackground(matrix);
    WriteChar(matrix,24,100,B,color,size);
    WriteChar(matrix,24 +(size*9)*1,100,U,color,size);
    WriteChar(matrix,24 +(size*9)*2,100,L,color,size);
    WriteChar(matrix,24 +(size*9)*3,100,A,color,size);
    WriteChar(matrix,24 +(size*9)*4,100,N,color,size);
    WriteChar(matrix,24 +(size*9)*5,100,C,color,size);
    WriteChar(matrix,24 +(size*9)*6,100,I,color,size);
    WriteChar(matrix,24 +(size*9)*8,100,three,color,size);

}





void map1(uint16_t *matrix, int color)
{
    WriteLineHorizon(matrix, 120, 60, color, 20, 120);
    WriteLineVert(matrix, 100, 40, color, 20, 60);      
    WriteLineHorizon(matrix, 120, 180, color, 20, 120);
    WriteLineVert(matrix, 240, 160, color, 20, 60);
    WriteLineHorizon(matrix, 120, 260, color, 20, 120);
    WriteLineHorizon(matrix, 360, 40, color, 20, 20);
    WriteLineHorizon(matrix, 360, 100, color, 20, 20);
    WriteLineHorizon(matrix, 360, 160, color, 20, 20);
    WriteLineHorizon(matrix, 360, 220, color, 20, 20);
    WriteLineHorizon(matrix, 360, 280, color, 20, 20);
}

void map2(uint16_t *matrix, int color)
{
    WriteLineHorizon(matrix, 0, 0, 0xccc4, 3, 480);
    WriteLineHorizon(matrix, 0, 317, 0xccc4, 3, 480);
    WriteLineVert(matrix, 477, 0, 0xccc4, 3, 320);
    WriteLineVert(matrix, 0, 0, 0xccc4, 3, 320);
    WriteLineHorizon(matrix, 0, 60, 0xccc4, 20, 40);
    WriteLineHorizon(matrix, 80, 60, 0xccc4, 20, 180);
    WriteLineHorizon(matrix, 340, 60, 0xccc4, 20, 140);
    WriteLineHorizon(matrix, 0, 220, 0xccc4, 20, 180);
    WriteLineHorizon(matrix, 380, 220, 0xccc4, 20, 100);
    WriteLineHorizon(matrix, 380, 220, 0xccc4, 20, 100);
    WriteLineHorizon(matrix, 180, 180, 0xEED6, 60, 60);
    WriteLineHorizon(matrix, 80, 80, 0xEED6, 40, 40);
    WriteLineHorizon(matrix, 40, 160, 0xEED6, 20, 40);
    WriteLineHorizon(matrix, 40, 280, 0xEED6, 40, 40);
    //CAR
    WriteLineHorizon(matrix, 400, 120, 0xC618, 60, 40);
    //Barrels
    WriteLineHorizon(matrix, 180, 240, 0x0019, 20, 20); //B
    WriteLineHorizon(matrix, 200, 240, 0xC800, 20, 20); //R
    WriteLineHorizon(matrix, 200, 260, 0x1660, 20, 20); //G
    WriteChar(matrix, 200, 190, B, 0xF800, 3);
}