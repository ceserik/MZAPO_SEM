#include "game.h"

#include "bounds_handler.h"
#include "key_handler.h"
#include "lcd_lib.h"

#define BULLET_WIDTH 5
#define BULLET_SPEED 15
#define WALL_WIDTH 20

#define NPC_RELOADING_TIME 50
#define PLAYER_RELOADING_TIME 50

int spawn_positions[][2] = {{25, 25}, {425, 25}};

game *create_game() {
  game *g = (game *)malloc(sizeof(game));
  if (!g) {
    fprintf(stderr, "Game didn't allocated correctly.");
    return NULL;
  }
  g->matrix = (uint16_t *)malloc(sizeof(uint16_t) * LCDWIDTH * LCDHEIGHT);
  g->bullets = (bullet **)malloc(sizeof(bullet *) * BULLETS_SIZE);
  for (int i = 0; i < g->bullets_size; i++) {
    g->bullets[i] = (bullet *)malloc(sizeof(bullet));
  }
  g->bullets_size = BULLETS_SIZE;
  g->bullets_count = 0;
  g->score = 0;

  g->npc = create_player(25, 25, 100, LEFT, NPC_WIDTH, NPC_HEIGHT, NPC_COLOR, NPC_DMG);

  return g;
}

void destroy_game(game *g) {
  free(g->matrix);
  for (int i = 0; i < g->bullets_size; i++) {
    free(g->bullets[i]);
  }
  free(g->bullets);
  free(g);
}

bool add_bullet(game *g, bullet *b) {
  g->bullets[b->id] = b;
  g->bullets_count += 1;
  if (g->bullets_count == g->bullets_size) {
    bullet **tmp;
    g->bullets_size *= 2;
    tmp = realloc(g->bullets, g->bullets_count);
    if (!tmp) {
      fprintf(stderr, "Reallocing bullets array of game didn't succeed.\n");
      return false;
    } else {
      g->bullets = tmp;
    }
  }
  return true;
}

bool remove_bullet(game *g, int id) {
  if (!g->bullets[id]) {
    fprintf(stderr, "Didn't have to remove anything. Value in bullets array on %d is already null.\n", id);
  } else if (id >= 0 && id < g->bullets_count) {
    destroy_bullet(g->bullets[id]);
    g->bullets_count -= 1;
    g->bullets[id] = NULL;
    //printf("%d %d\n", g->bullets[id]->x, g->bullets[id]->y);
    return true;
  }
  return false;
}

bool fire(game *g, player *p, unsigned char *lcd_mem_base) {
  int bullet_id = g->bullets_count;
  //find first null value because adding only to the end of array will cause terrible things
  //if not found, bullet is added to the end
  for (int i = 0; i < g->bullets_count; i++) {
    if (!g->bullets[i]) {
      bullet_id = i;
      break;
    }
  }
  bullet *b = create_bullet(p, bullet_id);
  if (!b) return false;
  add_bullet(g, b);

  return true;
}

void game_start(game *g, player *p, unsigned char *parlcd_mem_base) {
  char input;
  int p_rel_time = PLAYER_RELOADING_TIME, p_reloading = 0;
  int npc_rel_time = NPC_RELOADING_TIME, npc_reloading = 0;
  int direction = 0;
  do {
    if (p->fired) p_reloading++;
    if (p_reloading > p_rel_time) {
      p->fired = false;
      p_reloading = 0;
    }
    input = getchar();
    char npc_key = move_npc(g, p, parlcd_mem_base, &npc_reloading, &npc_rel_time);
    handle_key(input, g, parlcd_mem_base, p);
    handle_key(npc_key, g, parlcd_mem_base, g->npc);

    write_blank(g->matrix, g->npc->x, g->npc->y, g->npc->width, g->npc->height);
    printf("before move\n");
    printf("after move\n");

    update_bullets(g);
    check_bullets_bounds(g, p);
    if (g->npc->hp <= 0) {
      g->score += 1;
      write_blank(g->matrix, g->npc->x, g->npc->y, g->npc->width, g->npc->height);
      g->npc->x = spawn_positions[1][0];
      g->npc->y = spawn_positions[1][1];
      g->npc->hp = NPC_HP;
      write_npc(g);
    }
    refresh_lcd(parlcd_mem_base, g->matrix);

    // handle_key2(input, matrix, parlcd_mem_base, p2);

  } while (input != QUIT);
}

char move_npc(game *g, player *p, unsigned char *lcd, int *reloading, int *reload_time) {
  player *npc = g->npc;
  //check if player is not in the view
  //if he is - fire. end.
  printf("in the move_npc\n");
  char player_found = npc_finding_player(g, p);
  printf("after finding_player\n");
  if (player_found != 'n') {
    npc->last_dir = player_found;
    if (npc->can_fire && !npc->fired) {
      fire(g, npc, lcd);
      npc->fired = true;
    }
    if (npc->fired) {
      reloading++;
    }
    if (reloading > reload_time) {
      npc->fired = false;
      reloading = 0;
    }
    return 'n';
  }
  //if he is not:
  //find possible ways
  char possible_ways[4] = {-1, -1, -1, -1};
  printf("possible_ways %d", possible_ways[0]);
  int possible_ways_number = npc_find_possible_ways(g, possible_ways);
  //choose one of the possible ways
  char sorted_possible_ways[possible_ways_number];
  int cur_idx = 0;
  for (int i = 0; i < 4; i++) {
    if (possible_ways[i] != -1) {
      sorted_possible_ways[cur_idx] = possible_ways[i];
      cur_idx++;
    }
  }
  int rand_idx = (rand() % (possible_ways_number + 1));
  printf("rand_idx %d", rand_idx);
  char chosen_way = sorted_possible_ways[rand_idx];

  printf("chosen way %c\n", chosen_way);
  return chosen_way;
}

//return 'n' for none
char npc_finding_player(game *g, player *p) {
  player *npc = g->npc;
  //look to direction up
  //if there is a player color return that direction;
  //else continue to other direction
  int row, x, y, col;

  //left
  row = LCDWIDTH * (npc->y + (npc->height / 2));
  x = npc->x;
  while (g->matrix[row - x] == 0x0) x -= 15;
  if (g->matrix[row - x] == p->color) return LEFT;

  //up
  y = npc->y;  //to start in the middle of the npc
  col = npc->x + (npc->width / 2);
  while (g->matrix[LCDWIDTH * y + col] == 0x0) y -= 15;
  if (g->matrix[LCDWIDTH * y + npc->x] == p->color) return UP;

  //right
  row = LCDWIDTH * (npc->y + (npc->height / 2));
  x = npc->x + npc->width;
  while (g->matrix[row - x] == 0x0) x -= 15;
  if (g->matrix[row - x] == p->color) return LEFT;

  //down
  y = npc->y + npc->height;
  col = npc->x + (npc->width / 2);
  while (g->matrix[LCDWIDTH * y + npc->x] == 0x0) y += 15;
  if (g->matrix[LCDWIDTH * y + npc->x] == p->color) return DOWN;

  return 'n';
}

int npc_find_possible_ways(game *g, char possible_ways[]) {
  player *npc = g->npc;
  int ret_val = 0;

  //right
  for (int i = npc->y; i < npc->y + npc->height; i++) {
    if (g->matrix[npc->x + npc->width + npc->speed + i * LCDWIDTH] != 0x0) {
      npc->x -= npc->speed;
      possible_ways[0] = RIGHT;
      ret_val++;
    }
  }
  //down
  for (int i = npc->x; i < npc->x + npc->width; i++) {
    if (g->matrix[i + (npc->speed + npc->y + npc->height) * LCDWIDTH] != 0x0) {
      npc->y -= npc->speed;
      possible_ways[1] = DOWN;
      ret_val++;
    }
  }
  //up
  for (int i = npc->y; i < npc->y + npc->width; i++) {
    if (g->matrix[(i - npc->speed) * LCDWIDTH + npc->x] != 0x0) {
      npc->y += npc->speed;
      possible_ways[2] = UP;
      ret_val++;
    }
  }
  //left
  for (int i = npc->y; i < npc->y + npc->height; i++) {
    if (g->matrix[npc->x - npc->speed + i * LCDWIDTH] != 0x0) {
      npc->x += npc->speed;
      possible_ways[3] = LEFT;
      ret_val++;
    }
  }
  return ret_val;
}

void update_npc(game *g, player *p) {
  player *npc = g->npc;
  if (npc->last_dir == LEFT) {
    int row = LCDWIDTH * npc->y;
    int x = npc->x;
    while (g->matrix[row + x] != 0x0) {
      x--;
    }
    if (g->matrix[row + x] == p->color) {
      npc->can_fire = true;
    } else {
      // npc->last_dir = UP;
    }

  } else if (npc->last_dir == RIGHT) {
    int row = LCDWIDTH * npc->y;
    int x = npc->x + npc->width;
    while (g->matrix[row + x] == 0x0) {
      x++;
    }
    if (g->matrix[row + x] == p->color) {
      npc->can_fire = true;
    } else {
      // npc->last_dir = DOWN;
    }
  } else if (npc->last_dir == UP) {
    int y = npc->y;
    while (g->matrix[LCDWIDTH * y + npc->x] == 0x0) {
      y--;
    }
    if (g->matrix[LCDWIDTH * y + npc->x] == p->color) {
      npc->can_fire = true;
    } else {
      // npc->last_dir = RIGHT;
    }
  } else if (npc->last_dir == DOWN) {
    int y = npc->y + npc->height;
    while (g->matrix[LCDWIDTH * y + npc->x] == 0x0) {
      y++;
    }
    if (g->matrix[LCDWIDTH * y + npc->x] == p->color) {
      npc->can_fire = true;
    } else {
      // npc->last_dir = LEFT;
    }
  }
}
