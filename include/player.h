#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct player Player;

Player *initPlayer(int x, int y);
void movePlayer(Player *p, int dx, int dy);
void parseInput(int input_key, Player* player);
void drawPlayer(Player *p);
void freePlayer(void *p);

#endif