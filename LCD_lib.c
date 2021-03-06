#include "LCD_lib.h"
#include "stdio.h"
#include "stdlib.h"
#include "mzapo_parlcd.h"
#include "font.h"

void write_background(uint16_t *matrix)
{
    for (int y = 0; y < 320; y++)
    {
        for (int x = 0; x < 480; x++)
        {
            int B = 31 - x / 15;
            int R = x / 15;
            int G = y/10;
            uint16_t color = B + (R << 11) + (G<<6);
            matrix[480 * y + x] = color;
        }
    }
}

// Writes character to matrix x y are coordiantes of top left corner of character
void write_char(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t *c, uint16_t color, int size)
{

    int Xiterable = Xoffset;
    int TemplateRow = 0;
    int LastY = 0;
    //printf("aj idem");
    for (int Ycounter = 0; Ycounter < (size * 16); ++Ycounter)
    {
        for (int Xcounter = 0; Xcounter < 8; ++Xcounter)
        {
            if (Ycounter - LastY == size)
            {
                TemplateRow += 1;
                LastY = Ycounter;
            }
            //fprintf(stdout,"Teplate = %d Ycounter = %d  LastY = %d\n",TemplateRow,Ycounter,LastY);
            if ((c[TemplateRow] << Xcounter) & 0b1000000000000000)
            {

                for (int width = 0; width <= size; ++width)
                {
                    //printf("%d ", c[Ycounter] << (Xcounter));
                    int nextPos = ((Ycounter + Yoffset) * 480) + Xiterable + width;
                    matrix[nextPos] = color;
                }

                //printf("farbim");
            }
            Xiterable += 1 + size;
        }
        //printf("\n");
        Xiterable = Xoffset;
    }
    //TemplateRow = 0;
}

void write_blank(uint16_t *matrix, int x, int y, int Width, int Heigth)
{

    int StartPos = (480 * y) + x;
    int currentPos = StartPos;
    for (int i = 0; i < Heigth; i++)
    {
        for (int z = 0; z < Width; z++)
        {
            matrix[currentPos + z] = 0x0;
        }
        currentPos += 480;
    }
}

void write_horizon_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length)
{
    for (int Yoffset = 0; Yoffset < size; Yoffset++)
    {
        for (int Xoffset = 0; Xoffset < length; Xoffset++)
        {
            int nextPos = ((y + Yoffset) * 480) + x + Xoffset;
            matrix[nextPos] = color;
        }
    }
}
void write_vert_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length)
{
    for (int Xoffset = 0; Xoffset < size; Xoffset++)
    {
        for (int Yoffset = 0; Yoffset < length; Yoffset++)
        {
            int nextPos = ((y + Yoffset) * 480) + x + Xoffset;
            matrix[nextPos] = color;
        }
    }
}

// Writes 3 numbers to a row
void write_val(uint16_t *matrix, uint8_t number, int x, int y, int size, int color,int transparent)
{
    if(!transparent)
        write_blank(matrix, x, y + 3, (size + 1) * 8 * 3, size * 16);
    char num[4];
    if (number < 100 && number > 9)
    {
        sprintf(num, "0%d", number);
    }
    if (number < 10)
    {
        sprintf(num, "00%d", number);
    }
    if (number >= 100)
    {
        sprintf(num, "%d", number);
    }
    for (int i = 0; i < 3; i++)
    {
        uint16_t *ptr;
        switch (num[i])
        {
        case '1':
            ptr = one;
            break;
        case '2':
            ptr = two;
            break;
        case '3':
            ptr = three;
            break;
        case '4':
            ptr = four;
            break;
        case '5':
            ptr = five;
            break;
        case '6':
            ptr = six;
            break;
        case '7':
            ptr = seven;
            break;
        case '8':
            ptr = eight;
            break;
        case '9':
            ptr = nine;
            break;
        case '0':
            ptr = zero;
            break;

        default:
            ptr = SPACE;
            break;
        }
        write_char(matrix, x + (i * (size + 1) * 8), y, ptr, color, size);
    }
}

// Writes matrix to LCD
void refresh_lcd(unsigned char *membase, uint16_t *matrix)
{
    parlcd_write_cmd(membase, 0x2c);
    for (int y = 0; y < 320; y++)
    {
        for (int x = 0; x < (480); x++)
        {
            parlcd_write_data(membase, matrix[(480 * y) + x]);
        }
    }
}


uint16_t convert_color (uint8_t red, uint8_t  green, uint8_t  blue ){
    //printf("%d",red);
    uint8_t red16 = (((uint16_t) red *31 )/256);
    
    uint8_t green16 = (((uint16_t) green *63 )/256);
    uint8_t blue16 = (((uint16_t) blue *31 )/256);
    return  (uint16_t)blue16 | (uint16_t)green16 <<5 | (uint16_t) red16 <<11;
}
