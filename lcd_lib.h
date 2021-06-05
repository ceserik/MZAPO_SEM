#ifndef lcd_lib_H
#define lcd_lib_H

#include <stdint.h>

#include "game.h"

extern uint16_t S[16];
extern uint16_t P[16];
extern uint16_t E[16];
extern uint16_t D[16];
extern uint16_t C[16];
extern uint16_t O[16];
extern uint16_t Equal[16];
extern uint16_t I[16];
extern uint16_t F[16];
extern uint16_t T[16];
extern uint16_t one[16];
extern uint16_t two[16];
extern uint16_t three[16];
extern uint16_t four[16];
extern uint16_t five[16];
extern uint16_t six[16];
extern uint16_t seven[16];
extern uint16_t eight[16];
extern uint16_t nine[16];
extern uint16_t zero[16];

void write_char(uint16_t *matrix, int Xoffset, int Yoffset, uint16_t *c, uint16_t color, int size);
void refresh_lcd(unsigned char *membase, uint16_t *matrix);
void write_line(int angle, int x, int y, int size);
void write_horizon_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length);
void write_vert_line(uint16_t *matrix, int x, int y, uint16_t color, int size, int length);
void write_val(uint16_t *matrix, int num[3], int x, int y, int size, int color);
void write_blank(uint16_t *matrix, int x, int y, int w, int h);
void write_player_right(game *g, player *p);
void write_player_up(game *g, player *p);
void write_player_left(game *g, player *p);
void write_player_down(game *g, player *p);
void render_bullet(uint16_t *matrix, int x, int y, int w, int h);
void write_npc(game *g);
void write_player(game *g, player *p);

void write_map(uint16_t *matrix, int color);  // Rendition of Dust 2 from Counter Strike

#endif