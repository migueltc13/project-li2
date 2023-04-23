#include <stdio.h>
#include <stdlib.h>
#include "player.h"

#define PLAYER_SYMBOL '@'
#define MAX_HEALTH 100

struct player {
    unsigned int x;
    unsigned int y;
    char symbol;
    int health;
    // ...
} Player;

Player *initPlayer(int x, int y) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    p->symbol = PLAYER_SYMBOL;
    p->health = MAX_HEALTH;
    return p;
}

// add map or use funtion is_valid_move(map, x, y) on coords x+dx, y+dy
/** 
 * @brief Moves the player by dx and dy
 * 
 * Assumes that the player is on the map and that the move is valid.
 * 
 * @param p Pointer to the player
 * @param dx Change in x
 * @param dy Change in y
 */
void movePlayer(Player *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

void drawPlayer(Player *p) {
    // TODO: Add color
    mvaddch(p->y, p->x, p->symbol);
}

void freePlayer(Player *p) {
    // p->x = 0;
    // p->y = 0;
    // p->symbol = ' ';
    // p->health = 0;
    free(p);
}
