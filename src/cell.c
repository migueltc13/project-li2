#include <stdlib.h>
#include "cell.h"

/**
 * @brief Initializes a new cell. Allocates memory for a new cell and initializes its fields.
 * 
 * @param x Cell x coordinate
 * @param y Cell y coordinate
 * @param symbol Cell symbol
 * @param is_walkable Is the cell walkable?
 * @param block_light Is the cell blocking light?
 * @param color Cell color TODO
 * @return Cell* Initialized cell
 */
Cell *initCell(int x, int y, char symbol, int is_walkable, int block_light, int color) {
    Cell *cell = (Cell *) malloc(sizeof(struct cell));
    cell->x = x;
    cell->y = y;
    cell->symbol = symbol;
    cell->is_walkable = is_walkable;
    cell->block_light = block_light;
    cell->distance_to_player = -1;
    cell->is_visible = 0; // Not visible by default
    cell->color = color; // TODO
    return cell;
}

/**
 * @brief Initializes a new floor cell with the initCell() function.
 * 
 * @details A floor cell is walkable, doesn't block light and has a grey color.
 * 
 * @param x Cell x coordinate
 * @param y Cell y coordinate
 * @return Cell* Initialized cell as a floor cell
 */
Cell *initCellFloor(int x, int y) {
    return initCell(x, y, '.', 1, 0, 6); // 6 is COLOR_GREY TODO window.h
}

/**
 * @brief Initializes a new wall cell with the initCell() function.
 * 
 * @details A wall cell is not walkable, blocks light and has a black color.
 * 
 * @param x Cell x coordinate
 * @param y Cell y coordinate
 * @return Cell* Initialized cell as a wall cell
 */
Cell *initCellWall(int x, int y) {
    return initCell(x, y, '#', 0, 1, 0); // 0 is COLOR_BLACK TODO window.h
}

/**
 * @brief Checks if a cell is walkable. returns the cell->is_walkable field.
 * 
 * @param cell Cell to check
 * @return int 1 if the cell is walkable, 0 otherwise
 */
int isCellWalkable(Cell *cell) {
    return cell->is_walkable;
}

/**
 * @brief Checks if a cell is blocking light. returns the cell->block_light field.
 * 
 * @param cell Cell to check
 * @return int 1 if the cell is blocking light, 0 otherwise
 */
int isCellBlockingLight(Cell *cell) {
    return cell->block_light;
}

/* TODO
Monster *isCellMonster(Cell *cell) {
    return cell->has_monster != NULL;
}*/

/* TODO: deprecated
void drawCell(Cell *cell) {
    attron(COLOR_PAIR(cell->color));
    mvaddch(cell->y, cell->x, cell->symbol);
    attroff(COLOR_PAIR(cell->color))
} */

/**
 * @brief Frees the memory allocated for a cell by initCell(). Sets all the cell fields to 0.
 * 
 * @param p Cell to free
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
