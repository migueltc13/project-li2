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
 * @param has_item 1 if the cell has an item, 0 otherwise
 * @param monster_index 1 if the cell has a monster, 0 otherwise
 * @param has_player 1 if the cell has the player, 0 otherwise
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
    unsigned int has_item; // does this cell have an item?
    unsigned int has_player; // does this cell have the player?
    int monster_index; // does this cell have a monster? -1 if not else index in the monster array
    int color;
} Cell;

#define FLOOR_SYMBOL '.'
#define FLOOR_COLOR COLOR_GREEN
#define WALL_SYMBOL  '#'
#define WALL_COLOR COLOR_CYAN

Cell *initCell(int x, int y, char symbol, int is_walkable, int block_light, int color);
Cell *initCellFloor(int x, int y);
Cell *initCellWall(int x, int y);
int isCellWalkable(Cell *cell);
int isCellBlockingLight(Cell *cell);
int isCellItem(Cell *cell);
int isCellMonster(Cell *cell);
void freeCell(void *p);

#endif
