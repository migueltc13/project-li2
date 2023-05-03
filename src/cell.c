#include <stdio.h>

typedef struct cell {
    int x;
    int y;
    int is_walkable;
    int block_light; // is this cell blocking light?
    // Monster* monster; // monster in this cell, if any, if not NULL
} Cell;

