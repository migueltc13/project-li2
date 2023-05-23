#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include "map.h"
#include "cell.h"
#include "player.h"
#include "state.h"
#include "monster.h"
#include "item.h"
#include "menu.h" // macro for height of the menu

/**
 * @brief Initialize a new map
 * 
 * @param width 
 * @param height 
 * @return Map*
 */
Map *initMap(int width, int height) {
    Map *map = (Map *) malloc(sizeof(struct map));
    map->width = width;
    map->height = height - MENU_HEIGHT;
    // TODO: check if malloc fails
    map->cells = (Cell ***) malloc(sizeof(Cell **) * height);
    for (int i = 0; i < height; i++) {
        map->cells[i] = (Cell **) malloc(sizeof(Cell *) * width);
        for (int j = 0; j < width; j++) {
            map->cells[i][j] = initCellFloor(j, i);
        }
    }
    // Initialize the items list
    map->items = (Item **) malloc(sizeof(Item *) * MAX_ITEMS);
    return map;
}

/**
 * @brief Free the map structure
 * 
 * @param p a pointer to the map to free 
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
    for (int i = 0; i < map->nr_items; i++) {
        if (map->items[i] != NULL)
            freeItem(map->items[i]);
    }
    free(map->items);
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
 * @param st State to generate the map 
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

/**
 * @brief Draw the map according to the mode
 * 
 * @param map Map to draw
 * @param mode normal mode, vision mode or distance mode 
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
                mvaddch(i, j, map->cells[i][j]->symbol);
            }
        }
    }
}

/**
 * @brief Get the player initial (3x3 empty space)
 * 
 * @param map Map to get the player initial position
 * @param x returns the x coordinate
 * @param y returns the y coordinate
 * @return void
*/
void getPlayerInitialPosition(Map* map, int* x, int* y) {
    int height = map->height;
    int width = map->width;

    for (int j = 2; j < width - 2; j++) {
        for (int i = 2; i < height - 2; i++) {
            bool isValidPosition = true;
            
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (map->cells[i + di][j + dj]->symbol != '.') {
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
 * @brief Get random coordinates for monsters
 * 
 * @param map Map to get the random coordinates
 * @param x returns the x coordinate
 * @param y returns the y coordinate
*/
void getRandomCoordinates(Map* map, int* x, int* y) {
    int height = map->height;
    int width = map->width;

    while (true) {
        *x = rand() % width;
        *y = rand() % height;
        if (map->cells[*y][*x]->symbol == '.' && map->cells[*y][*x]->has_player == 0)
            return;
    }
}

/**
 * @brief Calculate the distances from the player to all the cells
 * 
 * @details The distance is calculated as the maximum between the x and y distance
 * 
 * @param map Map to calculate the distances
 * @param x Starting x coordinate
 * @param y Starting y coordinate
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
 * @param map Map to calculate the vision
 * @param x Starting x coordinate
 * @param y Starting y coordinate
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

/**
 * @brief Distribute the items on the map
 * 
 * @param map Map to distribute the items
 * @param items Array of items to distribute
 * @param nr_items Number of items in the array
 * 
 * TODO: replace while loop with for loop
 * TODO: CHECK if x doesnt have a valid value, then the item is not placed
*/
void distributeItems(Map* map, Item** items, int nr_items) {
    int height = map->height;
    int width = map->width;
    int missing_items = 0;
    for (int i = 0; i < nr_items; i++) {
        int x = (width / nr_items) * i;
        if (x >= width) x = width - 2;
        if (x <= 0) x = 1;
        int y = 0;
        // y has 4 options:
        // 1. 0 -> height - 1
        // 2. height - 1 -> 0
        // 3. height / 2 -> height - 1, height / 2 -> 0
        // 4. height - 1 -> height / 2, 0 -> height / 2
        int option = rand() % 4;
        if (option == 0) {
            for (y = 0; y < height; y++) {
                if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                    map->items[i] = items[i];
                    break;
                }
            }
            // TODO updateItem function 
            if (y < height) {
                items[i]->x = x;
                items[i]->y = y;
                map->cells[y][x]->has_item = 1;
                map->items[i] = items[i];
                map->nr_items++;
            }
            else {
                missing_items++;
            }
            // else TODO try again
        }
        else if (option == 1) {
            for (y = height - 1; y >= 0; y--) {
                if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                    map->items[i] = items[i];
                    break;
                }
            }
            if (y >= 0) {
                items[i]->x = x;
                items[i]->y = y;
                map->cells[y][x]->has_item = 1;
                map->items[i] = items[i];
                map->nr_items++;
            }
            else {
                missing_items++;
            }
            // else TODO try again
        }
        else if (option == 2) {
            int found = 0;
            for (y = height / 2; y < height; y++) {
                if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                    map->items[i] = items[i];
                    found = 1;
                    break;
                }
            }
            // if y is not found, search from the top
            if (!found) {
                for (y = height / 2; y >= 0; y--) {
                    if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                        map->items[i] = items[i];
                        break;
                    }
                }
            }
            if (y >= 0 && y < height) { 
                items[i]->x = x;
                items[i]->y = y;
                map->cells[y][x]->has_item = 1;
                map->items[i] = items[i];
                map->nr_items++;
            }
            // else TODO try again
            else {
                missing_items++;
            }
        }
        else {
            for (y = height - 1; y >= height / 2; y--) {
                if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                    map->items[i] = items[i];
                    break;
                }
            }
            if (map->cells[y][x]->symbol != '.') {
                for (y = 0; y < height / 2; y++) {
                    if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0) {
                        map->items[i] = items[i];
                        break;
                    }
                }
            }
            if (y >= 0 && y < height) {
                items[i]->x = x;
                items[i]->y = y;
                map->cells[y][x]->has_item = 1;
                map->items[i] = items[i];
                map->nr_items++;
            }
            // else TODO try again
            else {
                missing_items++;
            }
        }
    }
    // random for missing items
    while (missing_items > 0) {
        int x = rand() % width;
        int y = rand() % height;
        if (map->cells[y][x]->symbol == '.' && map->cells[y][x]->has_player == 0 && map->cells[y][x]->has_item == 0) {
            map->items[nr_items - missing_items] = items[nr_items - missing_items];
            items[nr_items - missing_items]->x = x;
            items[nr_items - missing_items]->y = y;
            map->cells[y][x]->has_item = 1;
            map->items[nr_items - missing_items] = items[nr_items - missing_items];
            map->nr_items++;
            missing_items--;
        }
    }
}
