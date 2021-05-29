#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "player.h"

#define LCDWIDTH 480
#define LCDHEIGHT 320
#define BULLETS_SIZE 10

typedef struct game{
  uint16_t *matrix;
  int bullets_count, bullets_size;
  bullet **bullets;
}game;

game *create_game();
void destroy_game(game *g);

bool add_bullet(game *g, bullet *b);
bool remove_bullet(game *g, int id);

void update_bullets(game *g);

void check_player_bounds(player *p);
//void check_bullets_bounds(game *g);
void check_bounds(game *g, player *p);

bool fire(game *g, player *p, unsigned char *lcd_mem_base);


#endif