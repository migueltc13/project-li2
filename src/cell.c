#include <stdlib.h>
#include <ncurses.h>
#include "cell.h"
#include "item.h" // macros

/**
 * @brief Initializes a new cell. Allocates memory for a new cell and initializes its fields.
 * 
 * @param x Cell x coordinate
 * @param y Cell y coordinate
 * @param symbol Cell symbol
 * @param is_walkable Is the cell walkable?
 * @param block_light Is the cell blocking light?
 * @param color Cell color
 * 
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
    cell->was_visible = 0; // Not visible by default
    cell->is_visible = 0; // Not visible by default
    cell->has_item = 0; // No item by default
    cell->monster_index = -1; // No monster by default
    cell->effect = 0; // No effect by default (global variable NO_EFFECT defined in combat.h as 0)
    cell->effect_duration = 0; // No effect by default
    cell->color = color;
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
    return initCell(x, y, FLOOR_SYMBOL, 1, 0, FLOOR_COLOR);
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
    return initCell(x, y, WALL_SYMBOL, 0, 1, WALL_COLOR);
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
    return cell->block_light == 1 || cell->effect == SMOKE_BOMB_EFFECT;
}

/**
 * @brief Checks if a cell is has a item. returns the cell->has_item field.
 * 
 * @param cell Cell to check
 * @return int 1 if the cell has a item, 0 otherwise
 */
int isCellItem(Cell *cell) {
    return cell->has_item;
}

/**
 * @brief Checks if a cell is has a monster
 * 
 * @param cell Cell to check
 * @return int 1 if the cell has a monster, 0 otherwise
 */
int isCellMonster(Cell *cell) {
    return cell->monster_index != -1;
}

/* Deprecated
void drawCell(Cell *cell) {
    attron(COLOR_PAIR(cell->color));
    mvaddch(cell->y, cell->x, cell->symbol);
    attroff(COLOR_PAIR(cell->color))
} */

/**
 * @brief Checks if a cell is the exit cell.
 * 
 * @param cell Cell to check
 * @return int 1 if the cell is the exit cell, 0 otherwise
*/
int isCellExit(Cell *cell) {
    return cell->symbol == EXIT_SYMBOL;
}

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
    cell->was_visible = 0;
    cell->is_visible = 0;
    cell->has_item = 0;
    cell->color = 0;
    free(cell);
}
