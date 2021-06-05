#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "player.h"

#define LCDWIDTH 480
#define LCDHEIGHT 320
#define BULLETS_SIZE 10
#define NPC_COLOR 0xD95B
#define NPC_HP 100
#define NPC_DMG 100
#define NPC_WIDTH 30
#define NPC_HEIGHT 30

typedef struct game {
  uint16_t *matrix;  //matrix that stores everything that appears on the screen
  //bullets_count -> count of bullets in bullets.
  int bullets_count, bullets_size;  //bullets_size -> maximum size in the moment
  bullet **bullets;                 //stores bullets
  int score;                        //current score of the player1

  player *npc;
} game;

game *create_game();
void destroy_game(game *g);

bool add_bullet(game *g, bullet *b);
bool remove_bullet(game *g, int id);
void update_bullets(game *g);

bool fire(game *g, player *p, unsigned char *parlcd_mem_base);

void game_start(game *g, player *p, unsigned char *parlcd_mem_base);
void update_npc(game *g, player *p);

char npc_finding_player(game *g, player *p);
char move_npc(game *g, player *p, unsigned char *lcd, int *reloading, int *reload_time);
int npc_find_possible_ways(game *g, char possible_ways[]);

#endif