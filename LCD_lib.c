#include "LCD_lib.h"
#include "stdio.h"
#include "stdlib.h"
#include "mzapo_parlcd.h"

/* Character S (0x53):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**      |
   |**      |
   | ***    |
   |   ***  |
   |     ** |
   |     ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
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

void WriteChar(uint16_t * matrix, int Xoffset, int Yoffset, uint16_t *c, uint16_t color, int size)
{

    int Xiterable = Xoffset;
    int TemplateRow = 0;
    int LastY = 0;
    //printf("aj idem");
    for (int Ycounter = 0; Ycounter < (size* 16); ++Ycounter)
    {
        for (int Xcounter = 0; Xcounter < 8; ++Xcounter)
        {
            if (Ycounter - LastY == size){
                TemplateRow +=1;
                LastY = Ycounter;
            }
            //fprintf(stdout,"Teplate = %d Ycounter = %d  LastY = %d\n",TemplateRow,Ycounter,LastY);
            if ((c[TemplateRow] << Xcounter) & 0b1000000000000000)
            {
                
                for(int width = 0; width < size; ++width){
                    //printf("%d ", c[Ycounter] << (Xcounter));
                    int nextPos = ((Ycounter + Yoffset) * 480) + Xiterable + width;
                    matrix[nextPos] = color;
                    
                }

                //printf("farbim");
            }
            Xiterable += 1 +size;
        }
        //printf("\n");
        Xiterable = Xoffset;
        
    }
    //TemplateRow = 0;
}


void RefreshLCD(unsigned char* membase, uint16_t * matrix){
    parlcd_write_cmd( membase, 0x2c);
   for (int y = 0; y < 320; y++){
        for (int x = 0; x < (480); x++)
        {
            //uint16_t c = 0xF800;
            parlcd_write_data(membase,matrix[(480*y)+x]);
            //printf("%d ", matrix[(480*y)+x]);
        }
        printf("\n");
    }
    printf("\n");
}
