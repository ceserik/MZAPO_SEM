#include <stdio.h>
#include <stdlib.h>
#include "LCD_lib.h"
#include "font_types.h"
#include <math.h>


int saveBmp(FILE* file, unsigned short * matrix);
void WriteDefault(unsigned short * matrix);


int main()
{

    FILE *outfile = fopen("matrix.ppm", "wb");
    unsigned short *matrix = (unsigned short *)malloc(480 * 320 * sizeof(uint16_t));

    WriteDefault(matrix);
    for (int y = 0; y < 320; y++)
    {
        for (int x = 0; x < 480; x++)
        {
            //printf("%d ", (matrix[(480*y)+x])/2016);
        }
        //printf("\n");
    }
   // printf("\n");
    saveBmp(outfile,matrix);
    fclose(outfile);
    

    return 0;
   
}

void WriteDefault(unsigned short * matrix){
    WriteLineHorizon(matrix,0,0,0x07E0, 3,480);
    WriteLineHorizon(matrix,0,317,0x07E0, 3,480);
    WriteLineVert(matrix,477,0,0x07E0,3,320);
    WriteLineVert(matrix,0,0,0x07E0,3,320);

    int size = 3;
    // SPEED=
    WriteChar(matrix, 10, 0, S, 0x07E0,size);
    WriteChar(matrix, 10+(1*(size+1)*8), 0, P, 0x07E0,size);
    WriteChar(matrix, 10+(2*(size+1)*8), 0, E, 0x07E0,size);
    WriteChar(matrix, 10+(3*(size+1)*8), 0, E, 0x07E0,size);
    WriteChar(matrix, 10+(4*(size+1)*8), 0, D, 0x07E0,size);
    WriteChar(matrix, 10+(6*(size+1)*8), 0, Equal, 0x07E0,size);
    // SET =
     WriteChar(matrix, 10+(0*(size+1)*8), (1*(size+1)*16), S, 0x07E0,size);
     WriteChar(matrix, 10+(1*(size+1)*8), (1*(size+1)*16), E, 0x07E0,size);
     WriteChar(matrix, 10+(2*(size+1)*8), (1*(size+1)*16), T, 0x07E0,size);
     WriteChar(matrix, 10+(4*(size+1)*8), (1*(size+1)*16), Equal, 0x07E0,size);
    //DIFF
    WriteChar(matrix, 10 + (0*(size+1)*8), (2*(size+1)*16), D, 0x07E0, size );
    WriteChar(matrix, 10+(1*(size+1)*8), (2*(size+1)*16), I, 0x07E0, size);
    WriteChar(matrix, 10+(2*(size+1)*8), (2*(size+1)*16), F, 0x07E0, size);
    WriteChar(matrix, 10+(3*(size+1)*8), (2*(size+1)*16), F, 0x07E0, size);
    WriteChar(matrix, 10+(5*(size+1)*8), (2*(size+1)*16), Equal, 0x07E0, size);


    // P=
    WriteChar(matrix, 10,(3*(size+1)*16),P,0x07E0,size );
    WriteChar(matrix, 10+(1*(size+1)*8), (3*(size+1)*16), Equal, 0x07E0,size);
    // I=
    WriteChar(matrix, 10 + (3*(size+1)*8),(3 * (size+1)*16),I,0x07E0,size );
    WriteChar(matrix, 10+(4*(size+1)*8), (3 * (size+1)*16), Equal, 0x07E0,size);
    // D=
    WriteChar(matrix, 10 + (6*(size+1)*8), (3*(size+1)*16), D, 0x07E0, size );
    WriteChar(matrix, 10+(7*(size+1)*8), (3*(size+1)*16), Equal, 0x07E0, size);
}


int saveBmp(FILE* outfile, unsigned short * matrix){

   fputs("P6\n",outfile);
   fputs("480\n",outfile);
   fputs("320\n",outfile);
   fputs("255\n",outfile);
    int length = 320*480;
    for (int i = 0; i < length; i++){
        
        uint8_t R = (matrix[i]<<11);
        uint8_t G = (matrix[i]<<6);
        uint8_t B = (matrix[i]<<1) ;
        //uint8_t R8 = (int) floor( R * 255.0 / 31.0 + 0.5);
        //uint8_t G8 = (int) floor( G * 255.0 / 63.0 + 0.5);
        //uint8_t B8 = (int) floor( B/ 31.0 + 0.5);
        fputc(R, outfile);
        fputc(G,outfile);
        fputc(B,outfile);
        //printf("XDDD\n");
    }
}



