#include <stdlib.h>
#include <ncurses.h>
#include "monster.h"
#include "state.h"
#include "map.h"
#include "cell.h"

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
    //monster->speed = speed;
    monster->is_alive = 1;
    monster->symbol = symbol;
    monster->name = name;
    monster->color = color;
    monster->pathfinding = pathfinding;
    monster->index = index;
    return monster;
}

/**
 * @brief Initialize a new monster: rat
 * 
 * @param x Starting x coordinate
 * @param y Starting y coordinate
 * @param index Monster index relative to the monsters array
 * @return Monster*
 */
Monster *initRat(int x, int y, int index) {
    return initMonster(x, y, 10, 1, 1, 'r', "rat", COLOR_GREEN, PATHFINDING_RANDOM, index);
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
    return initMonster(x, y, 20, 2, 2, 'g', "goblin", COLOR_MAGENTA, PATHFINDING_RANDOM, index);
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
    return initMonster(x, y, 30, 3, 3, 'o', "orc", COLOR_BLUE, PATHFINDING_SEARCHING, index);
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
    return initMonster(x, y, 40, 4, 4, 't', "troll", COLOR_YELLOW, PATHFINDING_SEARCHING, index);
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
    return initMonster(x, y, 50, 5, 5, 'd', "dragon", COLOR_RED, PATHFINDING_SEARCHING, index);
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
    else { // NORMAL_MODE or DISTANCE_MODE
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
 * @param monster
 * @param map
 * @param x
 * @param y
 */
void moveMonster(Monster* monster, Map *map, int x, int y) {
    // Update the previous cell's monster_index
    map->cells[monster->y][monster->x]->monster_index = -1;
    // Update the monster's coordinates
    monster->x = x;
    monster->y = y;
    // Update the cell's to monster_index
    map->cells[y][x]->monster_index = monster->index;
}

/**
 * @brief Pathfinding algorithm to find the shortest path to the player.
 * 
 * @details Checks the distance field of the cells around the monster and
 * moves to the cell with the lowest distance to the player.
 * 
 * @param monster
 * @param map
 */
void monsterPathfinding(Monster *monster, Map *map) {
    // Uses the distance field of the cells around the monster to find the shortest path to the player
    int x = monster->x;
    int y = monster->y;
    int min_distance = map->cells[y][x]->distance_to_player;
    int min_distance_x = x;
    int min_distance_y = y;
    // for loop for each cell around the monster
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            // check if the cell is walkable
            if (map->cells[y+di][x+dj]->is_walkable) {
                // check if the cell has a lower distance to the player, if so move the monster to that cell
                if (map->cells[y+di][x+dj]->distance_to_player < min_distance) {
                    min_distance = map->cells[y+di][x+dj]->distance_to_player;
                    min_distance_x = x+dj;
                    min_distance_y = y+di;
                    moveMonster(monster, map, min_distance_x, min_distance_y);
                }
            }
        }
    }

}

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
    if (map->cells[y-1][x-1]->is_walkable) {
        available_cells[available_cells_count] = 7; // numpad movement meaning 7
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
    default:
        break;
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
        initRat(x, y, index);
        break;
    case 1:
        return initGoblin(x, y, index);
        break;
    case 2:
        return initOrc(x, y, index);
        break;
    case 3:
        return initTroll(x, y, index);
        break;
    case 4:
        return initDragon(x, y, index);
        break;
    default:
        break;
    }
    return NULL;
}

/**
 * @brief Generate monsters array randomly 
*/
Monster **generateMonsters(Map* map) {
    Monster **monsters = (Monster **) malloc(sizeof(Monster *) * NUMBER_OF_MONSTERS);
    for (int i = 0; i < NUMBER_OF_MONSTERS; i++) {
        monsters[i] = generateMonster(map, i);
        if (monsters[i] == NULL) {
            i--; // unnecessary
        }
    }
    return monsters;
}

/**
 * @brief Move all the monsters
 * 
 * @details depends on monster pathfinding type
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
            // TODO
            break;
        case PATHFINDING_TOWARDS_PLAYER:
            monsterPathfinding(monsters[i], map);
            break;
        case PATHFINDING_AWAY_FROM_PLAYER:
            // TODO
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
