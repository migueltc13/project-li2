#ifndef _CELL_H_
#define _CELL_H_

/** @brief Structure of a cell
 * 
 * @details A cell is a single unit of the map with 2D coordinates,
 * a symbol, a walkable flag, a blocking light flag, a distance to the player,
 * a visibility flag and a color.
 * 
 * @param symbol Character to be displayed on the screen
 * @param x X coordinate
 * @param y Y coordinate
 * @param is_walkable 1 if the cell is walkable, 0 otherwise
 * @param block_light 1 if the cell blocks light, 0 otherwise
 * @param is_visible 1 if the cell is visible, 0 otherwise
 * @param distance_to_player Distance to the player, initialized to -1
 * @param color Color of the cell
 * TODO: @param monster Monster in this cell, if not NULL
 */
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
int isCellWalkable(Cell *cell);
int isCellBlockingLight(Cell *cell);
// int isCellMonster(Cell *cell);
void freeCell(void *p);

#endif
