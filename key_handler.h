#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include "game.h"
#include "player.h"

#define RIGHT 'd'
#define LEFT 'a'
#define UP 'w'
#define DOWN 's'
#define FIRE 'f'
#define QUIT 'q'

void handle_key(char input, game *g, unsigned char *lcd, player *p);
void handle_key2(char input, game *g, unsigned char *lcd, player *p);

#endif