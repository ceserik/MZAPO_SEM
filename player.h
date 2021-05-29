#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdbool.h>


#define BULLET_HEIGHT 5
#define BULLET_WIDTH 5


typedef struct {
    int x, y;
    int hp;
    char last_dir;
    int width, height;
}player;

typedef struct {
    int x, y;
    char direction;
    int id;
    bool active;
}bullet;


player* create_player(int x, int y, int hp, char start_dir, int width, int height);
void destroy_player(player *p);
bullet* create_bullet(player *p, int id);
void destroy_bullet(bullet *b);


//moves
void move_right(player *p);
void move_up(player *p);
void move_down(player *p);
void move_left(player *p);


#endif