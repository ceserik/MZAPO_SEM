#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h> 

#include "peripherals.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "LCD_lib.h"
#include "maps.h"
#include "font.h"



void call_termios(int reset)
{
   static struct termios tio, tioOld;
   tcgetattr(STDIN_FILENO, &tio);
   if (reset) {
      tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
   } else {
      tioOld = tio; //backup 
      cfmakeraw(&tio);
      tio.c_oflag |= OPOST;
      tcsetattr(STDIN_FILENO, TCSANOW, &tio);
   }
}




void WriteDefault(unsigned short *matrix);

int letter_size = 3;

int main(int argc, char *argv[])
{
  call_termios(0);
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  uint32_t val_line = 5;
  int i, j;
  unsigned int c;

  printf("Hello world\n");
  uint16_t *matrix = (uint16_t *)malloc(sizeof(uint16_t) * 480 * 320);
  memset(matrix, 255, 320 * 480 * 2);
  sleep(1);

  /*
   * Setup memory mapping which provides access to the peripheral
   * registers region of RGB LEDs, knobs and line of yellow LEDs.
   */

  mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  //RedPot = map_phys_address(SPILED_REG_BASE_PHYS, );
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  /* If mapping fails exit with error code */
  if (mem_base == NULL)
    exit(1);

  struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 20 * 1000 * 1000};
  for (i = 0; i < 30; i++)
  {
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
    val_line <<= 1;
    printf("LED val 0x%x\n", val_line);
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }

  if (parlcd_mem_base == NULL)
    exit(1);

  parlcd_hx8357_init(parlcd_mem_base);
  write_blank(matrix,0,0,480,320);
  welcome_screen (matrix,0x07E1);
  refresh_lcd(parlcd_mem_base, matrix);
  sleep(1);
  map2(matrix,0x0);
  refresh_lcd(parlcd_mem_base,matrix);

  
  printf("Goodbye world\n");
  uint32_t a = 1;
  char input;
  
  //setting terminal to raw mode
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  struct termios attrs;
  if (tcgetattr(STDIN_FILENO, &attrs) < 0) {
    return -1;
  }
  cfmakeraw(&attrs);
  tcsetattr(STDIN_FILENO, TCSANOW, &attrs);


  do {

    a *=2;
    if(a==0){
      a=1;
    }
    input = getchar();
    write_led_val(mem_base,a);
    uint8_t blue = get_blue_val(mem_base);
    uint8_t green = get_green_val(mem_base);
    uint8_t red = get_red_val(mem_base);
    write_val(matrix, blue, 10 + (6 * (letter_size + 1) * 8), 0, 3, 0xFFFF); // SPEED VAL
    refresh_lcd(parlcd_mem_base, matrix);
    
    //printf("greeeb = %d\n",green);
  } while(input != 'q');

  call_termios(1);

  free(matrix);
  return 0;
}

