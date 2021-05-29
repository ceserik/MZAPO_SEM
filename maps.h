#ifndef MAPS_H
#define MAPS_H

#include <stdint.h>

void map0(uint16_t *matrix,int color);
void map1(uint16_t *matrix,int color);
void map2(uint16_t *matrix,int color);
void welcome_screen(uint16_t *matrix, int color);
void end_screen(uint16_t *matrix, int score);

#endif  