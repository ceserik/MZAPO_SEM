
#include "key_handler.h"

#include "bounds_handler.h"

void handle_key(char input, game *g, unsigned char *lcd, player *p) {
  switch (input) {
    case LEFT:
      write_blank(g->matrix, p->x, p->y, p->width, p->height);
      move_left(p);
      check_player_bounds(g, p);
      write_player_left(g, p);
      break;
    case UP:
      write_blank(g->matrix, p->x, p->y, p->width, p->height);
      move_up(p);
      check_player_bounds(g, p);
      write_player_up(g, p);
      break;
    case DOWN:
      write_blank(g->matrix, p->x, p->y, p->width, p->height);
      move_down(p);
      check_player_bounds(g, p);
      write_player_down(g, p);
      break;
    case RIGHT:
      write_blank(g->matrix, p->x, p->y, p->width, p->height);
      move_right(p);
      check_player_bounds(g, p);
      write_player_right(g, p);
      break;
    case FIRE:
      if (!p->fired) {
        p->fired = true;
        fire(g, p, lcd);
      }
      break;
  }
}

// void handle_key2(char input, uint16_t *matrix,unsigned char *lcd, player *p) {
//   switch (input)
//   {
//   case 'j':
//     write_blank(matrix, p->x, p->y, p->width, p->height);
//     p->x -= 5;
//     WritePlayerRight(matrix, p->x, p->y, p->width, p->height, p->color);
//     RefreshLCD(lcd, matrix);
//     break;
//   case 'i':
//     write_blank(matrix, p->x, p->y, p->width, p->height);
//     p->y -= 5;
//     WritePlayerRight(matrix, p->x, p->y, p->width, p->height, p->color);
//     RefreshLCD(lcd, matrix);
//     break;
//   case 'k':
//     write_blank(matrix, p->x, p->y, p->width, p->height);
//     p->y += 5;
//     WritePlayerRight(matrix, p->x, p->y, p->width, p->height, p->color);
//     RefreshLCD(lcd, matrix);
//     break;
//   case 'l':
//     write_blank(matrix, p->x, p->y, p->width, p->height);
//     p->x += 5;
//     WritePlayerRight(matrix, p->x, p->y, p->width, p->height, p->color);
//     RefreshLCD(lcd, matrix);
//     break;
//   }
// }
