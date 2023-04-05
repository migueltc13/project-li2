#include <stdio.h>
#include <stdlib.h>
#include "position.h"

Position *newPosition(int x, int y) {
    Position *p = (Position *) malloc(sizeof(struct position));
    p->x = x;
    p->y = y;
    return p;
}

void freePosition(Position *p) {
    free(p);
}
