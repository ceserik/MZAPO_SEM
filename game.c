#include "game.h"

#define BULLET_WIDTH 5
#define BULLET_SPEED 20
#define PLAYERWIDTH 30
#define PLAYERHEIGHT 30
#define WALL_WIDTH 20



game *create_game() {
    game *g = (game*)malloc(sizeof(game));
    if (!g) {
        fprintf(stderr, "Game didn't allocated correctly.");
        return NULL;
    }
    g->matrix = (uint16_t*)malloc(sizeof(uint16_t) * LCDWIDTH * LCDHEIGHT);
    g->bullets = (bullet**)malloc(sizeof(bullet*)*BULLETS_SIZE);
    //the fuck
    for (int i = 0; i < g->bullets_size; i++) {
        g->bullets[i] = (bullet*)malloc(sizeof(bullet));
    }
    g->bullets_size = BULLETS_SIZE;
    g->bullets_count = 0;

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
    }
    else if (id >= 0 && id < g->bullets_count) {
        printf("%d %d\n", g->bullets[id]->x, g->bullets[id]->y);
        destroy_bullet(g->bullets[id]);
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
    bullet* b = create_bullet(p, bullet_id);
    if (!b) return false;
    add_bullet(g, b);

    return true;

}

//boundaries check ----------------------------------------------------------------------------------

void check_player_bounds(player *p) {
    p->x = p->x + PLAYERWIDTH + WALL_WIDTH >= LCDWIDTH ? LCDWIDTH-WALL_WIDTH-PLAYERWIDTH :
           p->x <= WALL_WIDTH ? WALL_WIDTH :
           p->x; 
    p->y = p->y + PLAYERHEIGHT + WALL_WIDTH >= LCDHEIGHT ? LCDHEIGHT-WALL_WIDTH :
           p->y <= WALL_WIDTH ? WALL_WIDTH-PLAYERHEIGHT :
           p->y;

}


void check_bounds(game *g, player *p) {
    //check_bullets_bounds(g);
    check_player_bounds(p);
}

//end of boundaries check ----------------------------------------------------------------------------------
//update ---------------------------------------------------------------------------------------------------


//end of update -------------------------------------------------------------------------------------------
//render --------------------------------------------------------------------------------------------------



