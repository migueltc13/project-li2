#ifndef _MAP_H_
#define _MAP_H_

typedef struct state State;
typedef struct cell Cell;
typedef struct item Item;
typedef struct player Player;
typedef struct projectile Projectile;

/**
 * @brief Structure that represents a map in the game
 * 
 * @details A map is a 2D array of cells with a width and a height.
 * It also contains an array of items distributed in the map.
 * 
 * @param width Width of the map (number of columns)
 * @param height Height of the map (number of rows)
 * @param cells 2D array of cells
 * @param has_menu Does the map have a menu? yes if MENU_HEIGHT > 3 * height
 * @param items Array of items in the map
 * @param nr_items Number of items in the map
 * @param projectiles Array of projectiles in the map
 * @param nr_projectiles Number of projectiles in the map
*/
typedef struct map {
    int width;
    int height;
    Cell ***cells;

    int has_menu;

    Item **items;
    int nr_items;

    Projectile **projectiles;
    int nr_projectiles;
} Map;

Map *initMap(int width, int height);
void freeMap(void *p);

void drawMap(Map *map, int mode);

//void generateMap(Map *map, int *x, int *y);
void generateMap(State *st);

void getPlayerInitialPosition(Map *map, int *x, int *y);
void getRandomCoordinates(Map* map, int* x, int* y);

void calculateDistances(Map* map, int x, int y);
void calculateVision(Map* map, Player* player);

void distributeItems(Map* map, Item** items, int nr_items); 

#endif
