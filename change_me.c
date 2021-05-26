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
#include "player.h"

int saveBmp(FILE* file, unsigned short * matrix);
void WriteDefault(unsigned short * matrix);
void WriteVal(uint16_t* matrix, int num[3],int x, int y, int size, int color );
void refill_matrix(uint16_t *matrix);
void handle_key(char input, uint16_t *matrix, player *p);
int letterSize = 3;


//player definition

int main(int argc, char *argv[]) {
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  uint32_t val_line=5;
  int i,j,k;
  unsigned int c;
  
  printf("Hello world\n");
  uint16_t *matrix = (uint16_t*)malloc(sizeof(uint16_t) * 480 * 320);
  memset(matrix,255,320*480*2);
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
      matrix[i*480+j]=c;
    }
  }
    WriteDefault(matrix);
    int n[3] = {4,2,0};
    WriteVal(matrix, n,10+(6*(letterSize+1)*8),0,3,0xFFFF);// SPEED VAL
    int x[3] = {0,6,9};
    WriteVal(matrix, x,10+(4*(letterSize+1)*8), (1*(letterSize + 1)*16),3,0xFFFF);// SPEED VAL


  //WriteChar(matrix, 200,200,S,0xF800,4);
  //WriteChar(matrix, 100,40,C,0xF800,10);
  //WriteChar(matrix, 200,80,O,0xF800,10);
  //WriteChar(matrix, 300,300,S,0xF800,1);
  RefreshLCD(parlcd_mem_base,matrix);

  clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);


  printf("Goodbye world\n");

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  
  struct termios attrs;
  if (tcgetattr(STDIN_FILENO, &attrs) < 0) {
    return -1;
  }

  cfmakeraw(&attrs);
  tcsetattr(STDIN_FILENO, TCSANOW, &attrs);


  char input;
  player *p = create_player(200, 290, 100);
  do
  {
    input = getchar();
    handle_key(input, matrix, p);
    //printf("x: %d y:%d ", p->x, p->y);
  } while (input != 'q');
  

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

    // Player
    WriteLineHorizon(matrix,0,0,0x07E0, 3,480);
    WriteLineHorizon(matrix,0,317,0x07E0, 3,480);
    WriteLineVert(matrix,477,0,0x07E0,3,320);
    WriteLineVert(matrix,0,0,0x07E0,3,320);



    
}

void refill_matrix(uint16_t *matrix) {
  for (int i = 0; i < 320; i++) {
    for (int j = 0; j < 480; j++) {
      matrix[i*480+j] = 0;
    }
  }
}

void handle_key(char input, uint16_t *matrix, player *p) {
  switch (input)
  {
  case 'a':
    move_right(p);
    break;
  case 'w':
    move_right(p);
    break;
  case 's':
    move_down(p);
    break;
  case 'd':
    move_right(p);
    break;
  } 
}
