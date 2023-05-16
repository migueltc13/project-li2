#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

Cell *initCell(int x, int y, char symbol, int is_walkable, int block_light) {
    Cell *cell = (Cell *) malloc(sizeof(struct cell));
    cell->x = x;
    cell->y = y;
    cell->symbol = symbol;
    cell->is_walkable = is_walkable;
    cell->block_light = block_light;
    cell->distance_to_player = -1;
    cell->is_visible = 0; // Not visible by default
    return cell;
}

Cell *initCellFloor(int x, int y) {
    return initCell(x, y, '.', 1, 0);
}

Cell *initCellWall(int x, int y) {
    return initCell(x, y, '#', 0, 1);
}

char getCellSymbol(Cell *cell) {
    return cell->symbol;
}

int isCellWalkable(Cell *cell) {
    return cell->is_walkable;
}

int isCellBlockingLight(Cell *cell) {
    return cell->block_light;
}

/*
int isCellMonster(Cell *cell) {
    return cell->monster != NULL;
}
*/

void freeCell(void *p) {
    Cell *cell = (Cell *) p;
    cell->x = 0;
    cell->y = 0;
    cell->symbol = '\0';
    cell->is_walkable = 0;
    cell->block_light = 0;
    cell->is_visible = 0;
    free(cell);
}