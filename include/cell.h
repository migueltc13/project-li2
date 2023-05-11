#ifndef _CELL_H_
#define _CELL_H_

typedef struct cell Cell;

Cell *initCell(int x, int y, char symbol, int is_walkable, int block_light);
Cell *initCellFloor(int x, int y);
Cell *initCellWall(int x, int y);
char getCellSymbol(Cell *cell);
int isCellWalkable(Cell *cell);
int isCellBlockingLight(Cell *cell);
// int isCellMonster(Cell *cell);
void freeCell(void *p);

#endif
