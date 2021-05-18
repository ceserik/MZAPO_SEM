#include "LCD_lib.h"
#include "stdio.h"
#include "stdlib.h"

uint16_t S[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc000,
    0xc000,
    0x7000,
    0x1c00,
    0x0600,
    0x0600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

uint16_t C[16] = {
    0x0000,
    0x0000,
    0x3c00,
    0x6600,
    0xc200,
    0xc000,
    0xc000,
    0xc000,
    0xc000,
    0xc200,
    0x6600,
    0x3c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

};

uint16_t O[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

void WriteChar(uint16_t * matrix, int Xoffset, int Yoffset, uint16_t *c, uint16_t color)
{

    int Xiterable = Xoffset;
    //printf("aj idem");
    for (int Ycounter = 0; Ycounter < 16; ++Ycounter)
    {
        for (int Xcounter = 0; Xcounter < 8; ++Xcounter)
        {

            if ((S[Ycounter] << Xcounter) & 0b1000000000000000)
            {
                printf("%d ", c[Ycounter] << (Xcounter));
                int nextPos = ((Ycounter + Yoffset) * 480) + Xiterable;

                matrix[nextPos] = color;
                //printf("farbim");
            }
            Xiterable += 1;
        }
        printf("\n");
        Xiterable = Xoffset;
    }
}