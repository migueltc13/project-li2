#ifndef _CELL_H_
#define _CELL_H_

typedef struct cell {
    char symbol;
    unsigned int x;
    unsigned int y;
    unsigned int is_walkable;
    unsigned int block_light; // is this cell blocking light?
    unsigned int is_visible; // is this cell visible?
    int distance_to_player; // initialized to -1
    int color;
    // Monster* monster; // monster in this cell, if not NULL
} Cell;

Cell *initCell(int x, int y, char symbol, int is_walkable, int block_light, int color);
Cell *initCellFloor(int x, int y);
Cell *initCellWall(int x, int y);
char getCellSymbol(Cell *cell);
int isCellWalkable(Cell *cell);
int isCellBlockingLight(Cell *cell);
// int isCellMonster(Cell *cell);
void freeCell(void *p);

#endif
