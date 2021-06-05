#ifndef BOUNDS_HANDLER_H
#define BOUNDS_HANDLER_H

#include "game.h"
#include "lcd_lib.h"
#include "player.h"

void check_bullets_bounds(game *g, player *p);
bool check_player_bounds(game *g, player *p);

#endif