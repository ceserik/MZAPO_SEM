#include "lcd_lib.h"

#include "mzapo_parlcd.h"
#include "stdio.h"
#include "stdlib.h"

// Writes character to matrix x y are coordiantes of top left corner of character
void write_char(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t *c, uint16_t color, int size) {
  int Xiterable = Xoffset;
  int TemplateRow = 0;
  int LastY = 0;
  //printf("aj idem");
  for (int Ycounter = 0; Ycounter < (size * 16); ++Ycounter) {
    for (int Xcounter = 0; Xcounter < 8; ++Xcounter) {
      if (Ycounter - LastY == size) {
        TemplateRow += 1;
        LastY = Ycounter;
      }
      //fprintf(stdout,"Teplate = %d Ycounter = %d  LastY = %d\n",TemplateRow,Ycounter,LastY);
      if ((c[TemplateRow] << Xcounter) & 0b1000000000000000) {
        for (int width = 0; width <= size; ++width) {
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

void write_horizon_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length) {
  for (int Yoffset = 0; Yoffset < size; Yoffset++) {
    for (int Xoffset = 0; Xoffset < length; Xoffset++) {
      int nextPos = ((y + Yoffset) * 480) + x + Xoffset;
      matrix[nextPos] = color;
    }
  }
}

void write_vert_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length) {
  for (int Xoffset = 0; Xoffset < size; Xoffset++) {
    for (int Yoffset = 0; Yoffset < length; Yoffset++) {
      int nextPos = ((y + Yoffset) * 480) + x + Xoffset;
      matrix[nextPos] = color;
    }
  }
}

void write_blank(uint16_t *matrix, int x, int y, int Width, int Heigth) {
  int StartPos = (480 * y) + x;
  int currentPos = StartPos;
  for (int i = 0; i < Heigth; i++) {
    for (int z = 0; z < Width; z++) {
      matrix[currentPos + z] = 0x0;
    }
    currentPos += 480;
  }
}

void write_player_right(game *g, player *p) {
  write_horizon_line(g->matrix, p->x, p->y, p->color, p->width, p->height);
  write_horizon_line(g->matrix, p->x + p->width / 2 - 3, p->y + p->width / 2 - 3, 0x0, 6, 15);
}
void write_player_down(game *g, player *p) {
  write_horizon_line(g->matrix, p->x, p->y, p->color, p->width, p->height);
  write_horizon_line(g->matrix, p->x + p->width / 2 - 3, p->y + p->height - 3, 0x0, 15, 6);
}
void write_player_left(game *g, player *p) {
  write_horizon_line(g->matrix, p->x, p->y, p->color, p->width, p->height);
  write_horizon_line(g->matrix, p->x, p->y + p->height / 2 - 3, 0x0, 6, 15);
}
void write_player_up(game *g, player *p) {
  write_horizon_line(g->matrix, p->x, p->y, p->color, p->width, p->height);
  write_horizon_line(g->matrix, p->x, p->y, 0x0, 15, 6);
}

void render_bullet(uint16_t *matrix, int x, int y, int w, int h) {
  write_horizon_line(matrix, x, y, 0x74F5, w, h);
}

// Writes 3 numbers to a row
void write_val(uint16_t *matrix, int num[3], int x, int y, int size, int color) {
  for (int i = 0; i < 3; i++) {
    uint16_t *ptr = NULL;
    switch (num[i]) {
      case 1:
        ptr = one;
        break;
      case 2:
        ptr = two;
        break;
      case 3:
        ptr = three;
        break;
      case 4:
        ptr = four;
        break;
      case 5:
        ptr = five;
        break;
      case 6:
        ptr = six;
        break;
      case 7:
        ptr = seven;
        break;
      case 8:
        ptr = eight;
        break;
      case 9:
        ptr = nine;
        break;
      case 0:
        ptr = zero;
        break;

      default:
        break;
    }

    write_char(matrix, x + (i * (size + 1) * 8), y, ptr, color, size);
  }
}

// Writes matrix to LCD
void refresh_lcd(unsigned char *membase, uint16_t *matrix) {
  parlcd_write_cmd(membase, 0x2c);
  for (int y = 0; y < 320; y++) {
    for (int x = 0; x < (480); x++) {
      parlcd_write_data(membase, matrix[(480 * y) + x]);
    }
  }
}

void write_map(uint16_t *matrix, int color)  // Rendition of Dust 2 from Counter Strike
{
  write_blank(matrix, 0, 0, 480, 320);
  write_horizon_line(matrix, 0, 0, 0xccc4, 20, 480);
  write_horizon_line(matrix, 0, 300, 0xccc4, 20, 480);
  write_vert_line(matrix, 460, 0, 0xccc4, 20, 320);
  write_vert_line(matrix, 0, 0, 0xccc4, 20, 320);
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
  write_horizon_line(matrix, 180, 240, 0x0019, 20, 20);  //B
  write_horizon_line(matrix, 200, 240, 0xC800, 20, 20);  //R
  write_horizon_line(matrix, 200, 260, 0x1660, 20, 20);  //G
}

void write_npc(game *g) {
  write_horizon_line(g->matrix, g->npc->x, g->npc->y, g->npc->color, g->npc->width, g->npc->height);
}

void write_player(game *g, player *p) {
  write_horizon_line(g->matrix, p->x, p->y, p->color, p->width, p->height);
  write_horizon_line(g->matrix, p->x + 12, p->y + 12, 0x0, 6, 15);
}

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

uint16_t P[16] = {

    0x0000,
    0x0000,
    0xfc00,
    0x6600,
    0x6600,
    0x6600,
    0x6600,
    0x7c00,
    0x6000,
    0x6000,
    0x6000,
    0xf000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

};

uint16_t E[16] = {
    0x0000,
    0x0000,
    0xfe00,
    0x6600,
    0x6000,
    0x6400,
    0x7c00,
    0x6400,
    0x6000,
    0x6000,
    0x6600,
    0xfe00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

};
uint16_t Equal[16] = {
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xfe00,
    0x0000,
    0xfe00,
    0x0000,
    0x0000,
    0x0000,
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
uint16_t D[16] = {
    0x0000,
    0x0000,
    0xf800,
    0x6c00,
    0x6600,
    0x6600,
    0x6600,
    0x6600,
    0x6600,
    0x6600,
    0x6c00,
    0xf800,
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
uint16_t I[16] = {
    0x0000,
    0x0000,
    0x3c00,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x3c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t F[16] = {
    0x0000,
    0x0000,
    0xfe00,
    0x6600,
    0x6000,
    0x6400,
    0x7c00,
    0x6400,
    0x6000,
    0x6000,
    0x6000,
    0xf000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t T[16] = {
    0x0000,
    0x0000,
    0x7e00,
    0x5a00,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x3c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

uint16_t one[16] = {
    0x0000,
    0x0000,
    0x1800,
    0x7800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x1800,
    0x7e00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t two[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc600,
    0x0600,
    0x0c00,
    0x1800,
    0x3000,
    0x6000,
    0xc600,
    0xfe00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

uint16_t three[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0x0600,
    0x0600,
    0x3c00,
    0x0600,
    0x0600,
    0x0600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t four[16] = {
    0x0000,
    0x0000,
    0x0c00,
    0x1c00,
    0x3c00,
    0x6c00,
    0xcc00,
    0xcc00,
    0xfe00,
    0x0c00,
    0x0c00,
    0x1e00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t five[16] = {
    0x0000,
    0x0000,
    0xfe00,
    0xc000,
    0xc000,
    0xc000,
    0xfc00,
    0x0600,
    0x0600,
    0x0600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t six[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc000,
    0xc000,
    0xfc00,
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
uint16_t seven[16] = {
    0x0000,
    0x0000,
    0xfe00,
    0xc600,
    0x0600,
    0x0c00,
    0x1800,
    0x3000,
    0x3000,
    0x3000,
    0x3000,
    0x3000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t eight[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc600,
    0xc600,
    0x7c00,
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
uint16_t nine[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc600,
    0xc600,
    0xc600,
    0x7e00,
    0x0600,
    0x0600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
uint16_t zero[16] = {
    0x0000,
    0x0000,
    0x7c00,
    0xc600,
    0xc600,
    0xce00,
    0xde00,
    0xf600,
    0xe600,
    0xc600,
    0xc600,
    0x7c00,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};
