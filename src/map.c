#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "map.h"

typedef struct map {
    int width; // nCols
    int height; // nRows
    char **tiles; // 2D array of chars; Future: Tile *tiles
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
    map->tiles = (char **) malloc(sizeof(char *) * height);
    for (int i = 0; i < height; i++) {
        map->tiles[i] = (char *) malloc(sizeof(char) * width);
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
    for (int i = 0; i < map->height; i++) {
        free(map->tiles[i]);
    }
    free(map->tiles);
    // map->width = 0;
    // map->height = 0;
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
    for (int i = 0; i < map->height; i++) {
        map->tiles[i][0] = '#';
        map->tiles[i][map->width - 1] = '#';
    }
    // Left and right walls
    for (int i = 0; i < map->width; i++) {
        map->tiles[0][i] = '#';
        map->tiles[map->height - 1][i] = '#';
    }
    // Fill the rest with '.'
    for (int i = 1; i < map->height - 1; i++) {
        for (int j = 1; j < map->width - 1; j++) {
            map->tiles[i][j] = '.';
        }
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
            mvaddch(i, j, map->tiles[i][j]);
        }
    }
}
