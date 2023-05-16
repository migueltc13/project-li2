#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "player.h"

Player *initPlayer(int x, int y) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    p->symbol = PLAYER_SYMBOL;
    p->health = MAX_HEALTH;
    p->vision = PLAYER_VISION;
    return p;
}

void drawPlayer(Player *p) {
    // TODO: Add color to player; use attron(COLOR_PAIR(1)) and attroff(COLOR_PAIR(1))
    mvaddch(p->y, p->x, p->symbol);
}

void freePlayer(void *p) {
    Player *player = (Player *) p;
    /* p->x = 0;
    p->y = 0;
    p->symbol = ' ';
    p->health = 0;
    p->vision = 0; */
    free(player);
}
