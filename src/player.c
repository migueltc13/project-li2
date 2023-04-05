#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "position.h"

#define MAX_HEALTH 100

Player* newPlayer(char symbol, Position *pos) {
    Player *p = (Player *) malloc(sizeof(struct player));
    p->symbol = symbol;
    p->health = MAX_HEALTH;
    p->position = pos;
    return p;
}

void freePlayer(Player *p) {
    freePosition(p->position);
    free(p);
}
