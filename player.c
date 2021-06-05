
#include "player.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "key_handler.h"

#define PLAYER_SPEED 5

player *create_player(int x, int y, int hp, char start_dir, int width, int height, uint16_t color, int damage) {
  player *tmp = (player *)malloc(sizeof(player));
  if (!tmp) return tmp;
  tmp->x = x;
  tmp->y = y;
  tmp->hp = hp;
  tmp->last_dir = start_dir;
  tmp->width = width;
  tmp->height = height;
  tmp->fired = false;
  tmp->can_fire = false;
  tmp->color = color;
  tmp->speed = PLAYER_SPEED;
  tmp->dmg = damage;

  return tmp;
}

void destroy_player(player *p) {
  free(p);
}

//moves
void move_right(player *p) {
  p->x += p->speed;
  p->last_dir = RIGHT;
}
void move_up(player *p) {
  p->y -= p->speed;
  p->last_dir = UP;
}
void move_down(player *p) {
  p->y += p->speed;
  p->last_dir = DOWN;
}
void move_left(player *p) {
  p->x -= p->speed;
  p->last_dir = LEFT;
}

bullet *create_bullet(player *p, int id) {
  bullet *b = (bullet *)malloc(sizeof(bullet));
  if (!b) {
    return b;
  }
  char direction = p->last_dir;
  b->direction = direction;
  b->id = id;
  if (direction == RIGHT) {
    b->x = p->x + p->width + b->width;
    b->y = p->y + p->height / 2;
  } else if (direction == LEFT) {
    b->x = p->x + b->width;
    b->y = p->y + p->height / 2;
  } else if (direction == DOWN) {
    b->x = p->x + p->width / 2;
    b->y = p->y + p->height + b->height;
  } else if (direction == UP) {
    b->x = p->x + p->width / 2;
    b->y = p->y - b->height;
  }

  return b;
}

int take_dmg(player *p, int amount) {
  p->hp -= amount;
  return p->hp;
}

void destroy_bullet(bullet *b) {
  free(b);
}

void change_player_dir(player *p) {
  switch (p->last_dir) {
    case RIGHT:
      p->last_dir = DOWN;
      break;
    case DOWN:
      p->last_dir = LEFT;
      break;
    case LEFT:
      p->last_dir = UP;
      break;
    case UP:
      p->last_dir = RIGHT;
      break;
  }
}