#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PLAYER_X 350
#define PLAYER_Y 160
#define PLAYER_HP 100
#define PLAYER_WIDTH 30
#define PLAYER_HEIGHT 30
#define PLAYER_COLOR 0x07E0
#define PLAYER_DMG 100
#define BULLET_HEIGHT 5
#define BULLET_WIDTH 5
#define BULLET_SPEED 15

typedef struct {
  int x, y;
  int hp;
  int speed;
  int width, height;
  char last_dir;
  bool fired, can_fire;
  uint16_t color;
  int dmg;

} player;

typedef struct {
  int id;
  int x, y;
  int width, height;
  char direction;
  bool active;
} bullet;

player *create_player(int x, int y, int hp, char start_dir, int width, int height, uint16_t color, int damage);
void destroy_player(player *p);
bullet *create_bullet(player *p, int id);
void destroy_bullet(bullet *b);

//moves
void move_right(player *p);
void move_up(player *p);
void move_down(player *p);
void move_left(player *p);

//direction = 0 in the direction of clock, direction = 1, in the opposite dir
void change_player_dir(player *p);

//returns player's current hp
int take_dmg(player *p, int amount);

#endif