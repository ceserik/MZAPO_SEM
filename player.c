
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "player.h"
#include "game.h"

#define PLAYER_SPEED 10


player* create_player(int x, int y, int hp, char start_dir, int width, int height) {
    player *tmp = (player*)malloc(sizeof(player));
    if (!tmp) return tmp;
    tmp->x = x;
    tmp->y = y;
    tmp->hp = hp;
    tmp->last_dir = start_dir;
    tmp->width = width;
    tmp->height = height;

    return tmp;
}

void destroy_player(player *p) {
    free(p);
}


//moves
void move_right(player *p) {
    p->x += 10;
    p->last_dir = 'd';
}
void move_up(player *p) {
    p->y -= 10;
    p->last_dir = 'w';
}
void move_down(player *p) {
    p->y += 10;
    p->last_dir = 's';
}
void move_left(player *p) {
    p->x -= 10;
    p->last_dir = 'a';
}

bullet* create_bullet(player *p, int id) {
    bullet *b = (bullet*)malloc(sizeof(bullet));    
    if (!b) {
        return b;
    }
    char direction = p->last_dir;
    b->direction = direction;
    b->id = id;
    if (direction == 'd') {
        b->x = p->x + p->width + BULLET_WIDTH;
        b->y = p->y + p->height/2;
    } else if (direction == 'a') {
        b->x = p->x + BULLET_WIDTH;
        b->y = p->y + p->height/2;
    } else if (direction == 's') {
        b->x = p->x + p->width/2;
        b->y = p->y + p->height + BULLET_HEIGHT;
    } else if (direction == 'w') {
        b->x = p->x + p->width/2;
        b->y = p->y - BULLET_HEIGHT;
    }

    return b;
}

void destroy_bullet(bullet *b) {
    free(b);
}

