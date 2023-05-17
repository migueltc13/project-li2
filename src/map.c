#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include "map.h"
#include "cell.h"
#include "state.h"

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
    // TODO: check if malloc fails
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
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++)
            freeCell(map->cells[i][j]);
        free(map->cells[i]);
    }
    free(map->cells);
    free(map);
}

/**
 * @brief Couns the number of walls within a given radius around a specific position
*/

int radius_count(State *s, int row, int col, int radius) {
    int count = 0;

    for (int i = row - radius; i <= row + radius; i++) {
        for (int j = col - radius; j <= col + radius; j++) {
            if (i >= 0 && i < s->map->height && j >= 0 && j < s->map->width) {
                if (s->map->cells[i][j]->symbol == '#')
                    count++;
            }
        }
    }

    return count;
}

/**
 * @brief Generate the map based on algorithm TODO explain more
 * - Top, bottom, left and right walls
 * - Fill the rest with 40% walls
 * 
 * Saves a (int**) map twice
 * 
 * @param Map *map 
 * @return void
 */
void generateMap(State *st) {
    int height = st->map->height;
    int width = st->map->width;

    // Generate walls for the outer border
    for (int i = 0; i < height; i++) {
        // Free the previous cells, if any (avoiding memory leaks by overwriting the pointer)
        if (st->map->cells[i][0] != NULL)
            free(st->map->cells[i][0]);
        if (st->map->cells[i][width - 1] != NULL)
            free(st->map->cells[i][width - 1]);

        st->map->cells[i][0] = initCellWall(0, i);
        st->map->cells[i][width - 1] = initCellWall(width - 1, i);
    }
    for (int j = 0; j < width; j++) {
        if (st->map->cells[0][j] != NULL)
            free(st->map->cells[0][j]);
        if (st->map->cells[height - 1][j] != NULL)
            free(st->map->cells[height - 1][j]);

        st->map->cells[0][j] = initCellWall(j, 0);
        st->map->cells[height - 1][j] = initCellWall(j, height - 1);
    }

    // Fill the middle area with walls or openings randomly
    srand(time(NULL));  // Seed the random number generator
    for (int R = 1; R < height - 1; R++) {
        for (int C = 1; C < width - 1; C++) {
            if (rand() % 100 < st->wall_prob) {  // 40% chance to create a wall
                if (st->map->cells[R][C] != NULL)
                    free(st->map->cells[R][C]);
                st->map->cells[R][C] = initCellWall(C, R);
            } else {
                if (st->map->cells[R][C] != NULL)
                    free(st->map->cells[R][C]);
                st->map->cells[R][C] = initCellFloor(C, R);
            }
        }
    }

    // First pass
    int mapa[st->map->height][st->map->width];
    for (int I = 0; I < st->first_pass; I++) {
        for (int R = 1; R < height - 1; R++) {
            for (int C = 1; C < width - 1; C++)
                mapa[R][C] = ((radius_count(st, R, C, 1) >= 5) || (radius_count(st, R, C, 2) <= 2)) ? 1 : 0;
        }
        for (int R = 1; R < height - 1; R++) {
            for (int C = 1; C < width - 1; C++) {
                if (mapa[R][C]) {
                    if (st->map->cells[R][C] != NULL)
                        free(st->map->cells[R][C]);
                    st->map->cells[R][C] = initCellWall(C, R);
                } else {
                    if (st->map->cells[R][C] != NULL)
                        free(st->map->cells[R][C]);
                    st->map->cells[R][C] = initCellFloor(C, R);
                }
            }
        }
    }

    // Second pass
    for (int I = 0; I < st->second_pass; I++) {
        for (int R = 1; R < height - 1; R++) {
            for (int C = 1; C < width - 1; C++)
                mapa[R][C] = ((radius_count(st, R, C, 1) >= 5) || (radius_count(st, R, C, 2) <= 2)) ? 1 : 0;
        }
        for (int R = 1; R < height - 1; R++) {
            for (int C = 1; C < width - 1; C++) {
                if (mapa[R][C]) {
                    if (st->map->cells[R][C] != NULL)
                        free(st->map->cells[R][C]);
                    st->map->cells[R][C] = initCellWall(C, R);
                } else {
                    if (st->map->cells[R][C] != NULL)
                        free(st->map->cells[R][C]);
                    st->map->cells[R][C] = initCellFloor(C, R);
                }
            }
        }
    }
}

/* void generateMap_old(Map* map, int* x, int* y) {
    int height = map->height;
    int width = map->width;

    // Generate walls for the outer border
    for (int i = 0; i < height; i++) {
        map->cells[i][0] = initCellWall(0, i);
        map->cells[i][width - 1] = initCellWall(width - 1, i);
    }
    for (int j = 1; j < width - 1; j++) {
        map->cells[0][j] = initCellWall(j, 0);
        map->cells[height - 1][j] = initCellWall(j, height - 1);
    }

    for (int i = 2; i < height - 2; i++) {
        for (int j = 2; j < width - 2; j++) {
            bool isValidPosition = true;

            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (getCellSymbol(map->cells[i + di][j + dj]) != '.') {
                        isValidPosition = false;
                        break;
                    }
                }
                if (!isValidPosition)
                    break;
            }

            if (isValidPosition) {
                // Save inital player coordenates
                *x = j;
                *y = i;
                return;
            }
        }
    }
} */

/**
 * @brief Draw the map
 * 
 * @param Map *map 
 * @return void
 */
void drawMap(Map *map, int mode) {
    if (mode == DISTANCE_MODE) {
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->width; j++) {
                int distance = map->cells[i][j]->distance_to_player;
                // TODO store last distance digit in cell
                while (distance > 9) {
                    distance %= 10;
                }
                char cellDistance = distance + '0';
                if (map->cells[i][j]->distance_to_player < 10) {
                    attron(COLOR_PAIR(COLOR_RED));
                    mvaddch(i, j, cellDistance);
                    attroff(COLOR_PAIR(COLOR_RED));
                }
                else if (map->cells[i][j]->distance_to_player < 30) {
                    attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, cellDistance);
                    attroff(COLOR_PAIR(COLOR_YELLOW));
                }
                else mvaddch(i, j, cellDistance);
            }
        }
    }
    else if (mode == VISION_MODE) {
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->width; j++) {
                if(map->cells[i][j]->is_visible == 1) {
                    // TODO: use color pairs
                    //attron(COLOR_PAIR(map->cells[i][j]->color));
                    mvaddch(i, j, map->cells[i][j]->symbol);
                    //attroff(COLOR_PAIR(map->cells[i][j]->color));
                } else
                    mvaddch(i, j, ' ');
            }
        }
    }
    else {
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->width; j++) {
                // TODO: use color pairs
                char cellSymbol = getCellSymbol(map->cells[i][j]); // TODO remove this function
                mvaddch(i, j, cellSymbol);
            }
        }
    }
}

/**
 * @brief Get the player initial (3x3 empty space)
 * 
 * @param Map *map
 * @param int *x
 * @param int *y
*/
void getPlayerInitialPosition(Map* map, int* x, int* y) {
    int height = map->height;
    int width = map->width;

    for (int j = 2; j < width - 2; j++) {
        for (int i = 2; i < height - 2; i++) {
            bool isValidPosition = true;
            
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (getCellSymbol(map->cells[i + di][j + dj]) != '.') {
                        isValidPosition = false;
                        break;
                    }
                }
                if (!isValidPosition)
                    break;
            }
            if (isValidPosition) {
                *x = j;
                *y = i;
                return;
            }
        }
    }
}

/**
 * @brief Calculate the distances from the player to all the cells
 * 
 * Adjanct cells have a distance of 1
 * 
 * @param Map *map
 * @param int x
 * @param int y
*/
void calculateDistances(Map* map, int x, int y) {
    for (int R = 0; R < map->height; R++) {
        for (int C = 0; C < map->width; C++) {
            int dy = abs(R - y);
            int dx = abs(C - x);
            int distance = (dx > dy) ? dx : dy;
            map->cells[R][C]->distance_to_player = distance;
        }
    }
}

/** 
 * @brief Calculate the vision of the player and updates the is_visible field in the cells
 * 
 * Based on the player position and the distance to the cells, the is_visible field is updated
 * 
 * @param Map *map
 * @param int x
 * @param int y
*/ 
void calculateVision(Map* map, int x, int y) {
    // Start from the player position
    int R = y;
    int C = x;

    // Iterate over all eight directions
    for (int dir = 0; dir < 8; dir++) {
        int dR, dC;
        // Set the direction offsets based on the current direction
        switch (dir) {
            case 0:  // Right
                dR = 0;
                dC = 1;
                break;
            case 1:  // Up-Right
                dR = -1;
                dC = 1;
                break;
            case 2:  // Up
                dR = -1;
                dC = 0;
                break;
            case 3:  // Up-Left
                dR = -1;
                dC = -1;
                break;
            case 4:  // Left
                dR = 0;
                dC = -1;
                break;
            case 5:  // Down-Left
                dR = 1;
                dC = -1;
                break;
            case 6:  // Down
                dR = 1;
                dC = 0;
                break;
            case 7:  // Down-Right
                dR = 1;
                dC = 1;
                break;
            default:
                break;
        }

        // Reset R and C to the player position
        R = y;
        C = x;

        // TODO: update a ranged grid length 5x5
        // Traverse in the current direction until the vision limit is reached or a blocking cell is encountered
        int visionDistance = 0;
        while (visionDistance < PLAYER_VISION) {
            R += dR;
            C += dC;

            // Check if the new position is within the map boundaries
            if (R < 0 || R >= map->height || C < 0 || C >= map->width)
                break;

            // Update the cell visibility
            map->cells[R][C]->is_visible = 1;

            // If the cell is a wall or smoke, stop
            if (isCellBlockingLight(map->cells[R][C]))
                break;

            // Increment the vision distance
            visionDistance++;
        }
    }
}


/* void calculateVision(Map* map, int x, int y) {
    // start from the player position
    int R = y;
    int C = x;
    // Start from the player position and go to the right
    while (R < map->width) {
        // If the distance is greater than the player vision, stop
        if (map->cells[R][C]->distance_to_player > PLAYER_VISION)
            break;
        // Update the cell visibility
        map->cells[R][C]->is_visible = 1; // true
        // If the cell is a wall, stop
        if (isCellBlockingLight(map->cells[R][C]))
            break;
        // Go to the next cell
        C++;
    }

    // TODO - do the same for the other directions
}
 */