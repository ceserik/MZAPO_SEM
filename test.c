#include <stdio.h>
#include <stdlib.h>
#include "LCD_lib.h"
#include "font_types.h"
//#include
#include <math.h>


int saveBmp(FILE* file, unsigned short * matrix);
void WriteDefault(unsigned short * matrix);
void WriteVal(uint16_t* matrix, int num[3],int x, int y, int size, int color );
int letter_size = 3;    
int main()
{

    FILE *outfile = fopen("matrix.ppm", "wb");
    unsigned short *matrix = (unsigned short *)malloc(480 * 320 * sizeof(uint16_t));

    WriteDefault(matrix);
    int n[3] = {4,2,0};
    WriteVal(matrix, n,10+(6*(letter_size+1)*8),0,3,0xFFFF);// SPEED VAL
    int x[3] = {0,6,9};
    WriteVal(matrix, x,10+(4*(letter_size+1)*8), (1*(letter_size + 1)*16),3,0xFFFF);// SPEED VAL
    saveBmp(outfile,matrix);
    fclose(outfile);
    write_blank(matrix,100,100,300,300);
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
    WriteChar(matrix, 10+(5*(size+1)*8), 0, Equal, 0x07E0,size);
    // SET =
     WriteChar(matrix, 10+(0*(size+1)*8), (1*(size+1)*16), S, 0x07E0,size);
     WriteChar(matrix, 10+(1*(size+1)*8), (1*(size+1)*16), E, 0x07E0,size);
     WriteChar(matrix, 10+(2*(size+1)*8), (1*(size+1)*16), T, 0x07E0,size);
     WriteChar(matrix, 10+(3*(size+1)*8), (1*(size+1)*16), Equal, 0x07E0,size);
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
    WriteChar(matrix, 10 + (5*(size+1)*8),(3 * (size+1)*16),I,0x07E0,size );
    WriteChar(matrix, 10+(6*(size+1)*8), (3 * (size+1)*16), Equal, 0x07E0,size);
    // D=
    WriteChar(matrix, 10 + (10*(size+1)*8), (3*(size+1)*16), D, 0x07E0, size );
    WriteChar(matrix, 10+(11*(size+1)*8), (3*(size+1)*16), Equal, 0x07E0, size);
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
        uint8_t R8 = (int) floor( R * 255.0 / 31.0 + 0.5);
        //uint8_t G8 = (int) floor( G * 255.0 / 63.0 + 0.5);
        //uint8_t B8 = (int) floor( B/ 31.0 + 0.5);
        fputc(R, outfile);
        fputc(G,outfile);
        fputc(B,outfile);
        //printf("XDDD\n");
    }
}



