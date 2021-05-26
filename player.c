#include "player.h"
#include <stdio.h>
#include <stdlib.h>

player* create_player(int x, int y, int hp) {
    player *tmp = (player*)malloc(sizeof(player));
    if (!tmp) return tmp;
    tmp->x = x;
    tmp->y = y;
    tmp->hp = hp;

    return tmp;
}

void destroy_player(player *p) {
    free(p);
}


//moves
void move_right(player *p) {
    p->x += 1;
}
void move_up(player *p) {
    p->y -= 1;
}
void move_down(player *p) {
    p->y += 1;
}
void move_left(player *p) {
    p->x -= 1;
}




