#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "map.h"
#include "cell.h"

typedef struct map {
    int width; // nCols
    int height; // nRows
    Cell ***cells; // 2D array of Cells
} Map;

/**
 * @brief Initialize a new map
 * 
 * @param int width 
 * @param int height 
 * @return Map*
 */
Map *initMap(int width, int height) {
    Map *map = (Map *) malloc(sizeof(struct map));
    map->width = width;
    map->height = height;
    map->cells = (Cell ***) malloc(sizeof(Cell **) * height);
    for (int i = 0; i < height; i++) {
        map->cells[i] = (Cell **) malloc(sizeof(Cell *) * width);
        for (int j = 0; j < width; j++)
            map->cells[i][j] = initCellFloor(j, i);
    }
    return map;
}

/**
 * @brief Free the map
 * 
 * @param void *p (a pointer to a Map) 
 * @return void
 */
void freeMap(void *p) {
    Map *map = (Map *) p;
    for (int i = 0; i < map->height; i++)
        free(map->cells[i]);
    free(map->cells);
    map->width  = 0;
    map->height = 0;
    free(map);
}

/**
 * @brief Generate the map with folowing rules:
 * - Top and bottom walls
 * - Left and right walls
 * - Fill the rest with '.' (TODO enhance)
 * 
 * @param Map *map 
 * @return void
 */
void generateMap(Map *map) {
    // Top and bottom walls
    for (int i=1; i<map->width - 1; i++) {
        for (int j=1; j<map->height - 1; j++) {
            // generate floors
            map->cells[j][i] = initCellFloor(i, j);
            // Left and right walls
            map->cells[j][0] = initCellWall(0, j);
            map->cells[j][map->width-1] = initCellWall(map->width-1, j);
        }
        // Top and bottom walls
        map->cells[0][i] = initCellWall(i, 0);
        map->cells[map->height-1][i] = initCellWall(i, map->height-1);
        // Corners
        map->cells[0][0] = initCellWall(0, 0);
        map->cells[0][map->width-1] = initCellWall(map->width-1, 0);
        map->cells[map->height-1][0] = initCellWall(0, map->height-1);
        map->cells[map->height-1][map->width-1] = initCellWall(map->width-1, map->height-1);
    }
}

/**
 * @brief Draw the map
 * 
 * @param Map *map 
 * @return void
 */
void drawMap(Map *map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            // TODO: use color pairs
            char cellSymbol = getCellSymbol(map->cells[i][j]);
            mvaddch(i, j, cellSymbol);
        }
    }
}
