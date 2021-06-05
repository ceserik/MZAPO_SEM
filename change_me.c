#define _POSIX_C_SOURCE 200112L

#include "change_me.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "bounds_handler.h"
#include "game.h"
#include "key_handler.h"
#include "lcd_lib.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "player.h"

#define WIDTH 480
#define HEIGHT 320

int letter_size = 3;

int main(int argc, char *argv[]) {
  unsigned char *mem_base;
  unsigned char *parlcd_mem_base;
  int i, j;
  unsigned int c;

  game *game = create_game();
  memset(game->matrix, 255, LCDHEIGHT * LCDWIDTH * 2);
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
  parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  if (parlcd_mem_base == NULL)
    exit(1);
  parlcd_hx8357_init(parlcd_mem_base);
  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320; i++) {
    for (j = 0; j < 480; j++) {
      c = 0x0;
      //parlcd_write_data(parlcd_mem_base, c);
      game->matrix[i * 480 + j] = c;
    }
  }
  clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);

  if (!set_terminal_mode_raw()) exit(1);
  player *p = create_player(PLAYER_X, PLAYER_Y, PLAYER_HP, RIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_COLOR, PLAYER_DMG);
  //player *p2 = create_player(PLAYERX + 100, PLAYERY, 100);

  write_map(game->matrix, 0x0);
  write_player(game, p);
  write_npc(game);
  refresh_lcd(parlcd_mem_base, game->matrix);

  game_start(game, p, parlcd_mem_base);

  return 0;
}

bool set_terminal_mode_raw() {
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  struct termios attrs;
  if (tcgetattr(STDIN_FILENO, &attrs) < 0) {
    return false;
  }
  cfmakeraw(&attrs);
  tcsetattr(STDIN_FILENO, TCSANOW, &attrs);

  return true;
}

void update_bullets(game *g) {
  for (int i = 0; i < g->bullets_size; i++) {
    bullet *cur_b = g->bullets[i];
    if (cur_b) {
      write_blank(g->matrix, cur_b->x, cur_b->y, 5, 5);
      if (cur_b->direction == LEFT) {
        cur_b->x -= BULLET_SPEED;
      } else if (cur_b->direction == DOWN)
        cur_b->y += BULLET_SPEED;
      else if (cur_b->direction == RIGHT)
        cur_b->x += BULLET_SPEED;
      else if (cur_b->direction == UP)
        cur_b->y -= BULLET_SPEED;
      write_horizon_line(g->matrix, cur_b->x, cur_b->y, 0x74F5, 5, 5);
    }
  }
}
