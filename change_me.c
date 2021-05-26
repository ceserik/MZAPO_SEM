#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "LCD_lib.h"
#include "maps.h"

void WriteDefault(unsigned short *matrix);

int letter_size = 3;

int main(int argc, char *argv[])
{
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

  
  //WriteBackground(matrix);
  
  printf("Goodbye world\n");
  int a = 0;
  while (a < 100)
  {
    a += 1;
    uint32_t rgb_knobs_value;
    rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    uint8_t green = rgb_knobs_value;
    
    WriteVal(matrix, green, 10 + (6 * (letter_size + 1) * 8), 0, 3, 0xFFFF); // SPEED VAL
    refresh_lcd(parlcd_mem_base, matrix);
    
    clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    printf("greeeb = %d\n",green);
    
  }
  return 0;
}

void WriteDefault(unsigned short *matrix)
{
  WriteLineHorizon(matrix, 0, 0, 0x07E0, 3, 480);
  WriteLineHorizon(matrix, 0, 317, 0x07E0, 3, 480);
  WriteLineVert(matrix, 477, 0, 0x07E0, 3, 320);
  WriteLineVert(matrix, 0, 0, 0x07E0, 3, 320);

  int size = 3;
  // SPEED=
  WriteChar(matrix, 10, 0, S, 0x07E0, size);
  WriteChar(matrix, 10 + (1 * (size + 1) * 8), 0, P, 0x07E0, size);
  WriteChar(matrix, 10 + (2 * (size + 1) * 8), 0, E, 0x07E0, size);
  WriteChar(matrix, 10 + (3 * (size + 1) * 8), 0, E, 0x07E0, size);
  WriteChar(matrix, 10 + (4 * (size + 1) * 8), 0, D, 0x07E0, size);
  WriteChar(matrix, 10 + (5 * (size + 1) * 8), 0, Equal, 0x07E0, size);
  // SET =
  WriteChar(matrix, 10 + (0 * (size + 1) * 8), (1 * (size + 1) * 16), S, 0x07E0, size);
  WriteChar(matrix, 10 + (1 * (size + 1) * 8), (1 * (size + 1) * 16), E, 0x07E0, size);
  WriteChar(matrix, 10 + (2 * (size + 1) * 8), (1 * (size + 1) * 16), T, 0x07E0, size);
  WriteChar(matrix, 10 + (3 * (size + 1) * 8), (1 * (size + 1) * 16), Equal, 0x07E0, size);
  //DIFF
  WriteChar(matrix, 10 + (0 * (size + 1) * 8), (2 * (size + 1) * 16), D, 0x07E0, size);
  WriteChar(matrix, 10 + (1 * (size + 1) * 8), (2 * (size + 1) * 16), I, 0x07E0, size);
  WriteChar(matrix, 10 + (2 * (size + 1) * 8), (2 * (size + 1) * 16), F, 0x07E0, size);
  WriteChar(matrix, 10 + (3 * (size + 1) * 8), (2 * (size + 1) * 16), F, 0x07E0, size);
  WriteChar(matrix, 10 + (5 * (size + 1) * 8), (2 * (size + 1) * 16), Equal, 0x07E0, size);

  // P=
  WriteChar(matrix, 10, (3 * (size + 1) * 16), P, 0x07E0, size);
  WriteChar(matrix, 10 + (1 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
  // I=
  WriteChar(matrix, 10 + (5 * (size + 1) * 8), (3 * (size + 1) * 16), I, 0x07E0, size);
  WriteChar(matrix, 10 + (6 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
  // D=
  WriteChar(matrix, 10 + (10 * (size + 1) * 8), (3 * (size + 1) * 16), D, 0x07E0, size);
  WriteChar(matrix, 10 + (11 * (size + 1) * 8), (3 * (size + 1) * 16), Equal, 0x07E0, size);
}