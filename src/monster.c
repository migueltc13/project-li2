#include <stdlib.h>
#include <ncurses.h>
#include "monster.h"
#include "state.h"
#include "map.h"
#include "cell.h"


// TODO: monster value in gold
/**
 * @brief Initialize a new monster
 * 
 * @param x Monster x coordinate 
 * @param y Monster y coordinate
 * @param health Monster health points
 * @param attack Monster attack points
 * @param defense Monster defense points
 * @param symbol Monster symbol
 * @param name Monster name
 * @param color Monster color
 * @param pathfinding Monster pathfinding type
 * @param index Monster index in the monsters array also stored in the cell (used as a pointer)
 * @return Monster*
 */
Monster *initMonster(int x, int y, int health, int attack, int defense, char symbol, char *name, int color, int pathfinding, int index) {
    Monster *monster = (Monster *) malloc(sizeof(struct monster));
    monster->x = x;
    monster->y = y;
    monster->health = health;
    monster->attack = attack;
    monster->defense = defense;
    //monster->speed = speed; // TODO
    monster->is_alive = 1;
    monster->symbol = symbol;
    monster->name = name;
    monster->color = color;
    monster->pathfinding = pathfinding;
    monster->index = index;
    return monster;
}

// TODO attack and defense

/**
 * @brief Initialize a new monster: rat
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initRat(int x, int y, int index) {
    return initMonster(x, y, 10, 1, 1, RAT_SYMBOL, "rat", RAT_COLOR, PATHFINDING_RANDOM, index);
}

/**
 * @brief Initialize a new monster: goblin
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initGoblin(int x, int y, int index) {
    return initMonster(x, y, 20, 2, 2, GOBLIN_SYMBOL, "goblin", GOBLIN_COLOR, PATHFINDING_RANDOM, index);
}

/**
 * @brief Initialize a new monster: orc
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initOrc(int x, int y, int index) {
    return initMonster(x, y, 30, 3, 3, ORC_SYMBOL, "orc", ORC_COLOR, PATHFINDING_SEARCHING, index);
}

/**
 * @brief Initialize a new monster: troll
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initTroll(int x, int y, int index) {
    // fun: PATHFINING_RECRUITING or PATHFINDING_AWAY_FROM_PLAYER
    return initMonster(x, y, 40, 4, 4, TROLL_SYMBOL, "troll", TROLL_COLOR, PATHFINDING_SEARCHING, index);
}

/**
 * @brief Initialize a new monster: dragon
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initDragon(int x, int y, int index) {
    return initMonster(x, y, 50, 5, 5, DRAGON_SYMBOL, "dragon", DRAGON_COLOR, PATHFINDING_SEARCHING, index);
}

/**
 * @brief Draw the monster on the map
 * 
 * @param st The game state
 * @param monster Monster to draw
 * @param mode The game mode draw monsters in (VISION_MODE, NORMAL_MODE, DISTANCE_MODE)
*/
void drawMonster(State* st, Monster *monster, int mode) {
    if (mode == VISION_MODE) {
        if (st->map->cells[monster->y][monster->x]->is_visible) {
            attron(COLOR_PAIR(monster->color));
            mvaddch(monster->y, monster->x, monster->symbol);
            attroff(COLOR_PAIR(monster->color));
        }
    }
    else if (mode == DISTANCE_MODE) {
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(monster->y, monster->x, monster->symbol);
        attroff(COLOR_PAIR(COLOR_CYAN));
    }
    else { // NORMAL_MODE
        attron(COLOR_PAIR(monster->color));
        mvaddch(monster->y, monster->x, monster->symbol);
        attroff(COLOR_PAIR(monster->color));
    }
}

/**
 * @brief Draw all the monsters on the map
 * 
 * @param st The game state
*/
void drawMonsters(State* st) {
    for (int i = 0; i < st->nMonsters; i++) {
        // TODO if (st->monsters[i]->is_alive)
        drawMonster(st, st->monsters[i], st->mode);
    }
}

/**
 * @brief Move the monster to the given coordinates
 * 
 * @details Update the monster's coordinates and the cell's monster_index
 * 
 * 
 * @param monster
 * @param map
 * @param x
 * @param y
 */
void moveMonster(Monster* monster, Map *map, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) return;
    if (!map->cells[y][x]->is_walkable) return;
    if (map->cells[y][x]->monster_index != -1) return;
    if (map->cells[y][x]->has_player) /* TODO monsterAttacksPlayer */ return;
    // Update the previous cell's monster_index
    map->cells[monster->y][monster->x]->monster_index = -1;
    // Update the monster's coordinates
    monster->x = x;
    monster->y = y;
    // Update the cell's to monster_index
    map->cells[y][x]->monster_index = monster->index;
}

/* // TODO
void monsterAttacksPlayer(Monster *monster, Player *player) { // State *st, Monster *monster
    // calculate damage combat.c
    // send message to the menu: (Monster->name)"%s" attacked you for %d damage, it lost %d health, sucker!
    // player->health -= damage;
    // if (player->health <= 0) { player->is_alive = 0; return; } // TODO set st->mode = EXIT_MODE
} */

/**
 * @brief Move the monster randomly.
 * 
 * @details Checks the available walkable cells around the monster and moves the monster randomly to one of them.
 * 
 * @param monster
 * @param map
 */
void moveMonsterRandom(Monster *monster, Map *map) {
    // check the available walkable cells around the monster
    int available_cells[8];
    int available_cells_count = 0;
    int x = monster->x;
    int y = monster->y;
    // TODO: for loop with: int dx = 0, dy = 0;
    // numpad movement
    if (map->cells[y-1][x-1]->is_walkable) {
        available_cells[available_cells_count] = 7;
        available_cells_count++;
    }
    if (map->cells[y-1][x]->is_walkable) {
        available_cells[available_cells_count] = 8;
        available_cells_count++;
    }
    if (map->cells[y-1][x+1]->is_walkable) {
        available_cells[available_cells_count] = 9;
        available_cells_count++;
    }
    if (map->cells[y][x-1]->is_walkable) {
        available_cells[available_cells_count] = 4;
        available_cells_count++;
    }
    if (map->cells[y][x+1]->is_walkable) {
        available_cells[available_cells_count] = 6;
        available_cells_count++;
    }
    if (map->cells[y+1][x-1]->is_walkable) {
        available_cells[available_cells_count] = 1;
        available_cells_count++;
    }
    if (map->cells[y+1][x]->is_walkable) {
        available_cells[available_cells_count] = 2;
        available_cells_count++;
    }
    if (map->cells[y+1][x+1]->is_walkable) {
        available_cells[available_cells_count] = 3;
        available_cells_count++;
    }
    // move to a random available cell
    int random_index = rand() % available_cells_count;
    switch (available_cells[random_index])
    {
    case 7:
        moveMonster(monster, map, x-1, y-1);
        break;
    case 8:
        moveMonster(monster, map, x, y-1);
        break;
    case 9:
        moveMonster(monster, map, x+1, y-1);
        break;
    case 4:
        moveMonster(monster, map, x-1, y);
        break;
    case 6:
        moveMonster(monster, map, x+1, y);
        break;
    case 1:
        moveMonster(monster, map, x-1, y+1);
        break;
    case 2:
        moveMonster(monster, map, x, y+1);
        break;
    case 3:
        moveMonster(monster, map, x+1, y+1);
        break;
    }
}

/**
 * @brief Pathfinding algorithm to find the shortest path to the player.
 * 
 * @details Calculates the path if any else return a message to the menu.
 * Moves the monster to the cell in the path with the lowest distance to the player.
 * 
 * @param monster
 * @param map
 */
void monsterPathfinding(Monster* monster, Map* map) {
    // Uses the distance field of the cells around the monster to find the shortest path to the player
    int x = monster->x;
    int y = monster->y;
    int min_distance = map->cells[y][x]->distance_to_player;
    int min_distance_x = x;
    int min_distance_y = y;
    int found = 0;

    // Loop through each cell around the monster
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            // Calculate the coordinates of the current cell
            int current_x = x + dj;
            int current_y = y + di;

            // Check if the current cell is within the map bounds
            if (current_x >= 0 && current_x < map->width && current_y >= 0 && current_y < map->height) {
                // Check if the cell is walkable
                if (map->cells[current_y][current_x]->is_walkable) {
                    // Check if the cell has a lower distance to the player, if so move the monster to that cell
                    if (map->cells[current_y][current_x]->distance_to_player < min_distance) {
                        min_distance = map->cells[current_y][current_x]->distance_to_player;
                        min_distance_x = current_x;
                        min_distance_y = current_y;
                        found = 1;
                    }
                    else if (map->cells[current_y][current_x]->distance_to_player == min_distance) {
                        // If the distance is the same, randomly decide whether to move to the new cell or not
                        found = rand() % 2; // 0 or 1 // TODO: 
                        if (found) {
                            min_distance_x = current_x;
                            min_distance_y = current_y;
                        }
                    }
                }
            }
        }
    }

    if (found) {
        moveMonster(monster, map, min_distance_x, min_distance_y);
    }
    else {
        // Send a message to the menu that the monster can't find the player
        // clearMessageLine(map->height + 5 - 2, map->width - 2);
        mvprintw(map->height + 5 - 2, 2, "%s", "The monster can't find the player");
        moveMonsterRandom(monster, map); // Implement moveMonsterRandom function if necessary
    }
}

/*
 * @brief Pathfinding algorithm to actively search for the player.
 * 
 * @details Uses vision to find the player and moves towards him.
 * If the player is not on vision or earing range, the monster moves randomly. 
 * 
 * @param monster
 * @param map
 * 
 * TODO: implement
 */
/* void monsterPathfindingSearch(Monster *monster, Map *map) {
    // Uses the distance field of the cells around the monster to find the shortest path to the player
    int x = monster->x;
    int y = monster->y;
    int max_distance = map->cells[y][x]->distance_to_player;
} */

/**
 * @brief Pathfinding algorithm to find the furthest path to the player.
 * 
 * @details Checks the distance field of the cells around the monster and
 * moves to the cell with the highest distance to the player.
 * 
 * @param monster
 * @param map
 */
void monsterPathfindingAway(Monster *monster, Map *map) {
    // Uses the distance field of the cells around the monster to find the shortest path to the player
    int x = monster->x;
    int y = monster->y;
    int max_distance = map->cells[y][x]->distance_to_player;
    int max_distance_x = x;
    int max_distance_y = y;
    // for loop for each cell around the monster
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            // check if the cell is walkable
            if (map->cells[y+di][x+dj]->is_walkable) {
                // check if the cell has a higher distance to the player, if so move the monster to that cell
                if (map->cells[y+di][x+dj]->distance_to_player > max_distance) {
                    max_distance = map->cells[y+di][x+dj]->distance_to_player;
                    max_distance_x = x+dj;
                    max_distance_y = y+di;
                    moveMonster(monster, map, max_distance_x, max_distance_y);
                }
                else {
                    // TODO check case map->cells[y+di][x+dj]->distance_to_player == max_distance
                    // if the distance can't be cutted, move the monster randomly
                    moveMonsterRandom(monster, map);
                }
            }
        }
    }
}

/**
 * @brief Generate one monster randomly
*/
Monster *generateMonster(Map *map, int index) {
    int random = rand() % 5; // 5 types of monsters
    int x, y; // monster coordinates
    getRandomCoordinates(map, &x, &y);
    // set cells monster_index value
    map->cells[y][x]->monster_index = index;
    switch (random)
    {
    case 0:
        return initRat(x, y, index);
    case 1:
        return initGoblin(x, y, index);
    case 2:
        return initOrc(x, y, index);
    case 3:
        return initTroll(x, y, index);
    case 4:
        return initDragon(x, y, index);
    }
    return NULL; // not reached
}

/**
 * @brief Generate monsters array randomly
 * 
 * @param map The game map
 * @return Monster** The monsters array with NUMBER_OF_MONSTERS monsters
*/
Monster **generateMonsters(Map* map) {
    Monster **monsters = (Monster **) malloc(sizeof(Monster *) * NUMBER_OF_MONSTERS);
    for (int i = 0; i < NUMBER_OF_MONSTERS; i++) {
        monsters[i] = generateMonster(map, i);
        /* if (monsters[i] == NULL) {
            i--; // unnecessary
        } */
    }
    return monsters;
}

/**
 * @brief Move all the monsters
 * 
 * @details Monster movements vary depending on the monster pathfinding type
 * and the map obstacles.
 * 
 * @param map The game map
 * @param monsters The monsters array
 * @param nMonsters The number of monsters
 * @return void
 */
void moveMonsters(Map* map, Monster **monsters, int nMonsters) {
    for (int i = 0; i < nMonsters; i++) {
        if (!monsters[i]->is_alive) continue;
        switch (monsters[i]->pathfinding)
        {
        case PATHFINDING_RANDOM:
            moveMonsterRandom(monsters[i], map);
            break;
        case PATHFINDING_SEARCHING:
            // TODO monsterPathfindingSearch(monsters[i], map);
            // meanwhile hardcoded to PATHFINDING_TOWARDS_PLAYER
            monsterPathfinding(monsters[i], map);
            break;
        case PATHFINDING_TOWARDS_PLAYER:
            monsterPathfinding(monsters[i], map);
            break;
        case PATHFINDING_AWAY_FROM_PLAYER:
            monsterPathfindingAway(monsters[i], map);
            break;
        }
    }
}


// TODO monster attack functions
// TODO monster death function

/**
 * @brief Free the monster
 * 
 * @param p a pointer to the Monster to free
 * @return void
 */
void freeMonster(void *p) {
    Monster *monster = (Monster *) p;
    free(monster);
}
