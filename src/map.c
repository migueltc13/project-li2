#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "map.h"
#include "cell.h"
#include "player.h"
#include "state.h"
#include "monster.h"
#include "item.h"
#include "combat.h"
#include "inventory.h" // function pickUpItem
#include "menu.h" // macro for height of the menu

/**
 * @brief Initialize a new map
 * 
 * @details Allocates memory for the map and its cells,
 * items and projectiles arrays.
 * 
 * 
 * 
 * @param width Map width
 * @param height Map height
 * @return Map pointer to the new map
 */
Map *initMap(int width, int height) {
    Map *map = (Map *) malloc(sizeof(struct map));
    map->width = width;
    // Determine if the map height is enough to fit the menu comfortably
    if (height >= 3*MENU_HEIGHT) {
        map->has_menu = 1; // true has menu
        map->height = height - MENU_HEIGHT;
    }
    else {
        map->has_menu = 0; // false no menu
        map->height = height;
    }

    map->cells = (Cell ***) malloc(sizeof(Cell **) * height);
    if (map->cells == NULL) {
        endwin();
        fprintf(stderr, "Fatal: could not allocate memory for map cells matrix\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < height; i++) {
        map->cells[i] = (Cell **) malloc(sizeof(Cell *) * width);
        if (map->cells[i] == NULL) {
            endwin();
            fprintf(stderr, "Fatal: could not allocate memory for map cells row\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < width; j++) {
            map->cells[i][j] = initCellFloor(j, i);
        }
    }
    // Initialize the items array
    map->items = (Item **) malloc(sizeof(Item *) * MAX_ITEMS);
    if (map->items == NULL) {
        endwin();
        fprintf(stderr, "Fatal: could not allocate memory for items array\n");
        exit(EXIT_FAILURE);
    }
    map->nr_items = 0;

    // Initialize the projectiles array
    map->projectiles = (Projectile **) malloc(sizeof(Projectile *) * MAX_PROJECTILES);
    if (map->projectiles == NULL) {
        endwin();
        fprintf(stderr, "Fatal: could not allocate memory for projectiles array\n");
        exit(EXIT_FAILURE);
    }
    map->nr_projectiles = 0;
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
    for (int i = 0; i < map->nr_projectiles; i++) {
        if (map->projectiles[i] != NULL)
            freeProjectile(map->projectiles[i]);
    }
    free(map->projectiles);
    free(map);
}

/**
 * @brief Counts the number of walls within a given radius around a specific position
 * 
 * @param s State to count the walls
 * @param row Row of the position
 * @param col Column of the position
 * @param radius Radius around the position
 * @return int Number of walls
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
 * - Fill the rest with a 40% chance of generating walls
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
 * NOTE: This could be improved by only drawing the cells that have changed
 * This could be implemented by storing bool value in a cell, per example: is_changed
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
                if (map->cells[i][j]->is_visible == 1) {
                    attron(COLOR_PAIR(map->cells[i][j]->color));
                    mvaddch(i, j, map->cells[i][j]->symbol);
                    attroff(COLOR_PAIR(map->cells[i][j]->color));
                } else if (map->cells[i][j]->was_visible == 1) {
                    attron(COLOR_PAIR(COLOR_SHADOW));
                    mvaddch(i, j, map->cells[i][j]->symbol);
                    attroff(COLOR_PAIR(COLOR_SHADOW));
                } else 
                    mvaddch(i, j, ' ');   
            }
        }
    }
    else { // NORMAL_MODE
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < map->width; j++) {
                attron(COLOR_PAIR(map->cells[i][j]->color));
                mvaddch(i, j, map->cells[i][j]->symbol);
                attroff(COLOR_PAIR(map->cells[i][j]->color));
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
            int isValidPosition = true;
            
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
        if (map->cells[*y][*x]->symbol == FLOOR_SYMBOL && map->cells[*y][*x]->has_player == 0)
            return;
    }
}

/**
 * @brief Calculate the distances from the player to all the cells,
 * also updates the effect and effect_duration and color of the effected cells.
 * Finally updates the is_visible field in the cells
 * 
 * @details The distance is calculated as the maximum between the x and y distance
 * 
 * @param map Map to calculate
 * @param x Starting x coordinate
 * @param y Starting y coordinate
*/
void calculateDistances(Map* map, int x, int y) {
    for (int R = 0; R < map->height; R++) {
        for (int C = 0; C < map->width; C++) {
            
            // Calculate the distance to the player
            int dy = abs(R - y);
            int dx = abs(C - x);
            int distance = (dx > dy) ? dx : dy;
            Cell *cell = map->cells[R][C];
            cell->distance_to_player = distance;
            
            // Calculate the (projectile) effect
            // 70 % chance to update cell
            int effect_prob = rand() % 100;
            if (effect_prob < 70) {
                if (cell->effect_duration == 0) {
                    cell->effect = 0; // No effect by default
                    // Set cell color back to normal color
                    cell->color = (cell->symbol == '.') ? FLOOR_COLOR : WALL_COLOR; // Floor or Wall
                    cell->color = (cell->symbol == EXIT_SYMBOL) ? EXIT_COLOR : cell->color; // or Gate Keeper
                }
                else cell->effect_duration--;
            }

            // Update the cell is_visble field
            cell->is_visible = 0;
            
            // Update the exit cell color
            cell->color = (cell->symbol == EXIT_SYMBOL) ? EXIT_COLOR : cell->color;
        }
    }
}

/**
 * @brief Calculates the vision for one line of sight
 * 
 * @details Recursive function given a starting position, a direction and a distance
 * 
 * @param map Map to calculate the specific line of sight
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param direction Direction of the line of sight
 * @param n Distance of the line of sight
*/
void calculateVisionAux(Map *map, int x, int y, int direction, int n) {
    // Break conditions
    if (n == 0)
        return;
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
        return;

    // Update the cell visibility
    map->cells[y][x]->was_visible = 1;
    map->cells[y][x]->is_visible = 1;

    // If the cell is a wall or smoke, stop
    if (isCellBlockingLight(map->cells[y][x]))
        return;
    
    // Calculate the next cell x, y
    int dR = 0, dC = 0;
    switch (direction) {
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
    }

    calculateVisionAux(map, x + dC, y + dR, direction, n - 1);
}

/**
 * @brief Calculate the vision of the player and updates the is_visible and was_visible field in the cells
 * 
 * @details Uses the calculateVisionAux() function to calculate the vision for all eight directions
 * @details Based on the player vision width calculates the number adjacent lines of sight
 * 
 * @param map Map to calculate the vision
 * @param player Player to calculate the vision
*/
void calculateVision(Map* map, Player* player) {

    // Iterate over all eight directions
    for (int dir = 0; dir < 8; dir++) {
        // Calculate the vision for the current direction
        calculateVisionAux(map, player->x, player->y, dir, player->vision);
        // Based on the player direction determine the adjacent cells to calculate the lines of sight
        int dx = 0, dy = 0;
        if (dir == 0 || dir == 4) { dx = 0; dy = 1; }
        else { dx = 1; dy = 0; }
 
        // Calculate the vision for the adjacent lines of sight
        for (int i = 1; i <= player->vision_width; i++) {
            calculateVisionAux(map, player->x + (dx * i), player->y + (dy * i), dir, player->vision);
            calculateVisionAux(map, player->x - (dx * i), player->y - (dy * i), dir, player->vision);
        }
    }
}

/**
 * @brief Distribute the items on the map
 * 
 * @details Distribute the items randomly on the map, avoiding the player, monsters and other items
 * 
 * @param map Map to distribute the items
 * @param items Array of items to distribute
 * @param nr_items Number of items in the array
*/
void distributeItems(Map* map, Item** items, int nr_items) {
    int height = map->height;
    int width = map->width;
    int i = 0;
    int max_tries = 1000, current_tries = 0;
    // Random distribution
    while (nr_items > 0 && current_tries < max_tries) {
        int x = rand() % width;
        int y = rand() % height;
        if (map->cells[y][x]->symbol == '.'
         && map->cells[y][x]->has_player == 0
         && map->cells[y][x]->has_item == 0
         && map->cells[y][x]->monster_index == -1)
        {
            map->items[i] = items[i];
            items[i]->x = x;
            items[i]->y = y;
            map->cells[y][x]->has_item = 1;
            map->items[i] = items[i];
            map->nr_items++;
            nr_items--;
            i++;
        }
        current_tries++;
    }
}

/**
 * @brief Get the item on the player position
 * 
 * @details Removes the item from the map, 
 * adds it to the player inventory
 * and equips it if there's no equipped item.
 * 
 * Also sends a message to the menu.
 *  
 * @param st The game state
*/
void pickUpItem(State *st) {
    Item *item = getItem(st->map->items, st->map->nr_items, st->player->x, st->player->y);
    if (item != NULL) {
        char *menu_message = (char *) malloc(sizeof(char) * st->map->width);
        // Send message to the menu: Item name and value
        if (item->type == POT_OF_GOLD_TYPE)
            snprintf(menu_message, st->map->width, "You picked up %d gold.", item->value);
        else
            snprintf(menu_message, st->map->width, "You picked up \"%s\". Valued at %d gold.", item->name, item->value);
        sendMenuMessage(st, menu_message);
        free(menu_message);    

        // Add the item to the player inventory and remove it from the map
        addItem(st->player->inventory, item);
        st->map->nr_items--;
        // getItem removes the item from the map->items array by shifting the items to the left
        // so we only need to decrease the number of items in the map
    
        // If there's no equiped item, equip the new item except if it is a pot of gold
        if (st->player->inventory->equipped_item == NULL && item->type != POT_OF_GOLD_TYPE)
            st->player->inventory->equipped_item = item;
    }
}

/**
 * @brief Get the closer monster to the player
 * 
 * @details The closer monster is the one with the minimum distance to the player.
 * Return the first monster found if there are more than one with the same distance.
 * 
 * @param st State to get the closer monster
 * @return Monster* A close monster
 * TODO: use for earing monsters and for attacking monsters (menu info)
*/
Monster *getCloserMonster(State *st) {

    Monster *closer_monster = NULL;

    Map *map = st->map;
    int min_distance = map->width + map->height;

    for (int i = 0; i < st->nMonsters; i++) {
        Monster *monster = st->monsters[i];
        Cell *cell = map->cells[monster->y][monster->x];
        int distance = cell->distance_to_player;
        if (distance < min_distance) {
            min_distance = distance;
            closer_monster = monster;
        }
    }
    return closer_monster;
}


/**
 * @brief Get the closer unhurt monster to the player
 * 
 * @details The closer monster is the one with the minimum distance to the player.
 * Except if the monster is hurt or too close to the player (2 cells away).
 * This function is used to determine which monster to ear.
 * 
 * @param st State to get the monster
 * @return Monster* A close monster unhurt and 3+ cells away
*/
Monster *getCloserUnhurtMonster(State *st) {
    
        Monster *closer_monster = NULL;
    
        Map *map = st->map;
        int min_distance = map->width + map->height;
    
        for (int i = 0; i < st->nMonsters; i++) {
            Monster *monster = st->monsters[i];
            Cell *cell = map->cells[monster->y][monster->x];
            int distance = cell->distance_to_player;
            if (distance < min_distance && distance > 2 && monster->health == monster->max_health) {
                min_distance = distance;
                closer_monster = monster;
            }
        }
        return closer_monster;
}
