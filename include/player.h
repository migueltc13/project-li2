#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_SYMBOL '@'
#define MAX_HEALTH 100
#define PLAYER_VISION 20 // cells

typedef struct player {
    unsigned int x;
    unsigned int y;
    char symbol;
    int health;
    int vision; // PLAYER_VISION
    // ...
} Player;

Player *initPlayer(int x, int y);
void movePlayer(Player *p, int dx, int dy);
void getPlayerCoords(Player *p, int *x, int *y);

void drawPlayer(Player *p);
void freePlayer(void *p);

#endif