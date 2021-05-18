#include <stdio.h>
#include <stdlib.h>
#include "LCD_lib.h"

int main()
{
    unsigned short *matrix = (unsigned short *)malloc(480 * 360 * sizeof(uint16_t));
    WriteChar(matrix, 10, 0, &S, 0x07E0);
    for (int y = 0; y < 480; y++)
    {
        for (int x = 0; x < (360); x++)
        {
            printf("%d ", matrix[(480*y)+x]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}