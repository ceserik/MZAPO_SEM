typedef struct
{
    int x, y;
    int hp;
}player;

player* create_player(int x, int y, int hp);
void destroy_player(player *p);

//moves
void move_right(player *p);
void move_up(player *p);
void move_down(player *p);
void move_left(player *p);
