#include <stdio.h>
#include <stdlib.h>
#include "LCD_lib.h"

int main()
{
    unsigned short *matrix = (unsigned short *)malloc(480 * 320 * sizeof(uint16_t));
    WriteChar(matrix, 10, 0, S, 0x07E0,5);
    WriteLineHorizon(matrix,1,1,0x07E0, 3,50);
    WriteLineVert(matrix,1,1,0x07E0,3,50);
    for (int y = 0; y < 320; y++)
    {
        for (int x = 0; x < 480; x++)
        {
            printf("%d ", (matrix[(480*y)+x])/2016);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}