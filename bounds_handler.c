
#include "bounds_handler.h"

#include "key_handler.h"

void check_bullets_bounds(game *g, player *p) {
  player *npc = g->npc;
  unsigned char ground_color = 0x0;
  for (int i = 0; i < g->bullets_size; i++) {
    bullet *cur_b = g->bullets[i];
    if (!cur_b) return;
    unsigned int curIdx;
    if (cur_b->direction == LEFT) {
      curIdx = cur_b->x - BULLET_SPEED + cur_b->y * 480;
      if (g->matrix[curIdx] != ground_color) {
        if (g->matrix[curIdx] == npc->color) {
          npc->hp = 0;
        }
        write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT);
        remove_bullet(g, cur_b->id);
      }
    }
    if (cur_b->direction == RIGHT) {
      curIdx = cur_b->x + BULLET_SPEED + cur_b->y * 480;
      if (g->matrix[curIdx] != ground_color) {
        if (g->matrix[curIdx] == npc->color) {
          npc->hp = 0;
        }
        write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT);
        remove_bullet(g, cur_b->id);
      }
    }
    if (cur_b->direction == UP) {
      curIdx = (cur_b->y - BULLET_SPEED) * LCDWIDTH + cur_b->x;
      if (g->matrix[curIdx] != ground_color) {
        if (g->matrix[curIdx] == npc->color) {
          npc->hp = 0;
        }
        write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT);
        remove_bullet(g, cur_b->id);
      }
    }
    if (cur_b->direction == DOWN) {
      curIdx = cur_b->x + (BULLET_SPEED + cur_b->y) * LCDWIDTH;
      if (g->matrix[curIdx] != ground_color) {
        if (g->matrix[curIdx] == npc->color) {
          npc->hp = 0;
        }
        write_blank(g->matrix, cur_b->x, cur_b->y, BULLET_WIDTH, BULLET_HEIGHT);
        remove_bullet(g, cur_b->id);
      }
    }
  }
}

bool check_player_bounds(game *g, player *p) {
  if (p->last_dir == LEFT) {
    for (int i = p->y; i < p->y + p->height; i++) {
      if (g->matrix[p->x - p->speed + i * LCDWIDTH] != 0x0) {
        p->x += p->speed;
        return true;
      }
    }
  }
  if (p->last_dir == RIGHT) {
    for (int i = p->y; i < p->y + p->height; i++) {
      if (g->matrix[p->x + p->width + p->speed + i * LCDWIDTH] != 0x0) {
        p->x -= p->speed;
        return true;
      }
    }
  }
  if (p->last_dir == UP) {
    for (int i = p->y; i < p->y + p->width; i++) {
      if (g->matrix[(i - p->speed) * LCDWIDTH + p->x] != 0x0) {
        p->y += p->speed;
        return true;
      }
    }
  }
  if (p->last_dir == DOWN) {
    for (int i = p->x; i < p->x + p->width; i++) {
      if (g->matrix[i + (p->speed + p->y + p->height) * LCDWIDTH] != 0x0) {
        p->y -= p->speed;
        return true;
      }
    }
  }
  return false;
}
