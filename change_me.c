#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "LCD_lib.h"
#include "game.h"
#include "player.h"

#define PLAYERX 25 
#define PLAYERY 25 
#define PLAYERHP 100
#define PLAYERWIDTH 30
#define PLAYERHEIGHT 30
#define BULLET_SPEED 20

#define WIDTH 480
#define HEIGHT 320

int saveBmp(FILE* file, unsigned short * matrix);
void WriteDefault(unsigned short * matrix);
void WriteVal(uint16_t* matrix, int num[3],int x, int y, int size, int color );
void refill_matrix(uint16_t *matrix);
void handle_key(char input, game *g, unsigned char * mem_base, player *p);
void handle_key2(char input, uint16_t *matrix, unsigned char * mem_base, player *p);
void check_bullets_bounds(game *g, unsigned char * lcd);
void map2(uint16_t *matrix, int color);
int letterSize = 3;



int main(int argc, char *argv[]) {
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  uint32_t val_line=5;
  int i,j,k;
  unsigned int c;
  
  printf("Hello world\n");
  
  uint16_t *matrix = (uint16_t*)malloc(sizeof(uint16_t) * 480 * 320);
  game* game = create_game();
  memset(game->matrix,255,320*480*2);

  sleep(1);

  /*
   * Setup memory mapping which provides access to the peripheral
   * registers region of RGB LEDs, knobs and line of yellow LEDs.
   */
  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

  /* If mapping fails exit with error code */
  if (mem_base == NULL)
    exit(1);



  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 20 * 1000 * 1000};
  for (i=0; i<30; i++) {
     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
     val_line<<=1;
     printf("LED val 0x%x\n", val_line);
     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }
  
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = 0x0;
      //parlcd_write_data(parlcd_mem_base, c);
      game->matrix[i*480+j]=c;
    }
  }
    map2(game->matrix, 0x0);
    int n[3] = {4,2,0};
    WriteVal(game->matrix, n,10+(6*(letterSize+1)*8),0,3,0xFFFF);// SPEED VAL
    int x[3] = {0,6,9};
    WriteVal(game->matrix, x,10+(4*(letterSize+1)*8), (1*(letterSize + 1)*16),3,0xFFFF);// SPEED VAL


  //WriteChar(matrix, 200,200,S,0xF800,4);
  //WriteChar(matrix, 100,40,C,0xF800,10);
  //WriteChar(matrix, 200,80,O,0xF800,10);
  //WriteChar(matrix, 300,300,S,0xF800,1);
  RefreshLCD(parlcd_mem_base,matrix);

  clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);


  printf("Goodbye world\n");

  //setting terminal to raw mode
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  struct termios attrs;
  if (tcgetattr(STDIN_FILENO, &attrs) < 0) {
    return -1;
  }
  cfmakeraw(&attrs);
  tcsetattr(STDIN_FILENO, TCSANOW, &attrs);

  //getting input
  char input;

  player *p = create_player(PLAYERX, PLAYERY, 100, 'd', PLAYERWIDTH, PLAYERHEIGHT);
  //player *p2 = create_player(PLAYERX + 100, PLAYERY, 100);
  write_horizon_line(game->matrix, p->x,p->y,0x07E0, PLAYERWIDTH,PLAYERHEIGHT);
  write_horizon_line(game->matrix, p->x+12, p->y+12, 0x0, 6, 15);
  // write_horizon_line(matrix, p2->x,p2->y,0x07E0, PLAYERWIDTH,PLAYERHEIGHT);
  // write_horizon_line(matrix, p2->x+12, p2->y+12, 0x0, 6, 15);
  RefreshLCD(parlcd_mem_base, game->matrix);

  do
  {
    input = getchar();
    handle_key(input, game, parlcd_mem_base, p);
    update_bullets(game);
    check_bounds(game, p);
    check_bullets_bounds(game, parlcd_mem_base);
    RefreshLCD(parlcd_mem_base, game->matrix);


   // handle_key2(input, matrix, parlcd_mem_base, p2);
    
  } while (input != 'q');

  // for (int i = 0; i < 80; i++)
  // {
  //   write_blank(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
  //   p->x += 1;
  //   WritePlayerRight(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
  //   RefreshLCD(parlcd_mem_base, matrix);
  // }
   
  

  return 0;
}

void WriteDefault(unsigned short * matrix){
    write_horizon_line(matrix,0,0,0x07E0, 3,480);
    write_horizon_line(matrix,0,317,0x07E0, 3,480);
    write_vert_line(matrix,477,0,0x07E0,3,320);
    write_vert_line(matrix,0,0,0x07E0,3,320);

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

void map2(uint16_t *matrix, int color) // Rendition of Dust 2 from Counter Strike
{
    write_blank(matrix,0,0,480,320);
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
    write_horizon_line(matrix, 180, 240, 0x0019, 20, 20); //B
    write_horizon_line(matrix, 200, 240, 0xC800, 20, 20); //R
    write_horizon_line(matrix, 200, 260, 0x1660, 20, 20); //G
}

void refill_matrix(uint16_t *matrix) {
  for (int i = 0; i < 320; i++) {
    for (int j = 0; j < 480; j++) {
      matrix[i*480+j] = 0;
    }
  }
}

void handle_key(char input, game *g, unsigned char *lcd, player *p) {
  switch (input) {
    case 'a':
      write_blank(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      move_left(p);
      WritePlayerLeft(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      break;
    case 'w':
      write_blank(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      move_up(p);
      WritePlayerUp(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      break;
    case 's':
      write_blank(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      move_down(p);
      WritePlayerDown(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      break;
    case 'd':
      write_blank(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      move_right(p);
      WritePlayerRight(g->matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
      break;
    case 'f':
      fire(g, p, lcd);
    break;
  } 
}

void handle_key2(char input, uint16_t *matrix,unsigned char *lcd, player *p) {
  switch (input)
  {
  case 'j':
    write_blank(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    p->x -= 5;
    WritePlayerRight(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    RefreshLCD(lcd, matrix);
    break;
  case 'i':
    write_blank(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    p->y -= 5;
    WritePlayerRight(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    RefreshLCD(lcd, matrix);
    break;
  case 'k':
    write_blank(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    p->y += 5;
    WritePlayerRight(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    RefreshLCD(lcd, matrix);
    break;
  case 'l':
    write_blank(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    p->x += 5;
    WritePlayerRight(matrix, p->x, p->y, PLAYERWIDTH, PLAYERHEIGHT);
    RefreshLCD(lcd, matrix);
    break;
  } 
}

void update_bullets(game *g) {
    for (int i = 0; i < g->bullets_size; i++) {
        bullet *cur_b = g->bullets[i];
        if (cur_b) {
            write_blank(g->matrix, cur_b->x, cur_b->y, 5, 5);
            if (cur_b->direction == 'a') {
              cur_b->x -= BULLET_SPEED;
            }
            else if (cur_b->direction == 's') cur_b->y += BULLET_SPEED;
            else if (cur_b->direction == 'd') cur_b->x += BULLET_SPEED;
            else if (cur_b->direction == 'w') cur_b->y -= BULLET_SPEED;
            write_horizon_line(g->matrix, cur_b->x, cur_b->y, 0x74F5, 5, 5);
        }
    }
}

void check_bullets_bounds(game *g, unsigned char * lcd) {
    for (int i = 0; i < g->bullets_size; i++) {
        bullet *cur_b = g->bullets[i];
        if (!cur_b) return;
        if (cur_b->direction == 'a') {
            if(g->matrix[cur_b->x - BULLET_SPEED + cur_b->y*480 ] != 0x0) {
                remove_bullet(g, cur_b->id);
                write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT); 
            }
        }
        if (cur_b->direction == 'd') {
            if(g->matrix[cur_b->x + BULLET_SPEED + cur_b->y*480] != 0x0) {
              remove_bullet(g,cur_b->id);
              write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT); 
            }
        }
        if (cur_b->direction == 'w') {
            if(g->matrix[(cur_b->y - BULLET_SPEED)*480 + cur_b->x ] != 0x0) {
              remove_bullet(g, cur_b->id);
              write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT); 
            }
        }
        if (cur_b->direction == 's') {
            printf("%d\n", g->matrix[cur_b->x + cur_b->y*480 + BULLET_SPEED]);
            if(g->matrix[cur_b->x+(BULLET_SPEED+ cur_b->y)*480] != 0x0) {
              remove_bullet(g, cur_b->id);
              write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT); 
            }
        }
    }
}