#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
// #include <mybool.h> TODO: implement replace
#include "state.h"
#include "player.h"
#include "map.h"
#include "cell.h"
#include "item.h"
#include "inventory.h"
#include "monster.h"
#include "combat.h"
#include "menu.h"

#define NUMBER_OF_ITEMS 15

/**
 * @brief Initialize a new game state
 * 
 * @details Generates the map, the player and the monsters
 * 
 * @return State*
 */ 
State *initState(int width, int height) {
    State *st = (State *) malloc(sizeof(struct state));

    st->turn = 0;
    st->first_pass = 3;
    st->second_pass = 3;
    st->wall_prob = 40;
    st->mode = VISION_MODE;

    st->map = initMap(width, height);
    generateMap(st);
    int x, y; getPlayerInitialPosition(st->map, &x, &y);
    st->player = initPlayer(x,y);
    
    Item **items = generateItems(NUMBER_OF_ITEMS);
    distributeItems(st->map, items, NUMBER_OF_ITEMS);

    // Generate monsters
    st->nMonsters = NUMBER_OF_MONSTERS;
    st->monsters = generateMonsters(st->map);

    calculateDistances(st->map, x, y);
    calculateVision(st->map, x, y);
    return st;
}

/**
 * @brief Free the game state
 * 
 * @param p a pointer to the State to free 
 * @return void
 */
void freeState(void *p) {
    State *st = (State *) p;
    freeMap(st->map);
    freePlayer(st->player);
    // TODO: free monsters
    free(st);
}

/**
 * @brief Update the game state
 * 
 * @param st 
 * @param input_key 
 * 
 */
void updateState(State *st, int input_key) {
    calculateState(st, input_key);
    if (st->mode == EXIT_MODE) {
        // TODO: Winner / Loser / Quiter
        freeState(st);
        endwin();
        exit(EXIT_SUCCESS);
    }
    else drawState(st);
}

/**
 * @brief Draw the game state
 * 
 * @param st 
 * @return void
 */
void drawState(State *st) {
    drawMap(st->map, st->mode); // TODO: mode outside drawMap()

    if (st->mode == VISION_MODE)
        drawVisibleItems(st->map, st->map->items, st->map->nr_items);
    else if (st->mode == NORMAL_MODE)
        drawAllItems(st->map->items, st->map->nr_items);

    drawMonsters(st);
    drawPlayer(st->player);
    if (st->map->has_menu)
        generateMenu(st);
    refresh();
}

/**
 * @brief Calulates the state based on the input key
 * 
 * @param st Calculated state
 * @param input_key Input keyboard key
 * @return void 
 * (0: normal view, 1: distance view enabled, -1: the player win/lost/quit)
*/
void calculateState(State *st, int input_key) {

    int dx = 0, dy = 0, direction = 0;
    switch(input_key) {
        // Movement keys, numpad, awsd and arrows keys
        case KEY_A1:
        case '7': dx = -1; dy = -1; direction = 7; break;
        case KEY_UP:
        case 'w':
        case '8': dx =  0; dy = -1; direction = 8; break;
        case KEY_A3:
        case '9': dx =  1; dy = -1; direction = 9; break;
        case KEY_LEFT:
        case 'a':
        case '4': dx = -1; dy =  0; direction = 4; break;
        case KEY_B2:
        case '5': break;
        case KEY_RIGHT:
        case 'd':
        case '6': dx =  1; dy =  0; direction = 6; break;
        case KEY_C1:
        case '1': dx = -1; dy =  1; direction = 1; break;
        case KEY_DOWN:
        case 's':
        case '2': dx =  0; dy =  1; direction = 2; break;
        case KEY_C3:
        case '3': dx =  1; dy =  1; direction = 3; break;
        // Inventory keys
        case 'i': switchEquippedItem(st->player->inventory); return;   // switch equipped item
        case 'o': sellEquippedItem(st, st->player->inventory); return; // sell equiped item
        // Combat / Potions keys
        case KEY_ENTER: // or space
        case 'u': useEquippedItem(st); return; // Use equipped item, potion or projectile
        // Menu keys
        case 'l': legendMenu(st);           return; // legend menu
        case 'q': st->mode = EXIT_MODE;     return; // quit
        case 'v': st->mode = VISION_MODE;   return; // normal view         
        case 'b': st->mode = DISTANCE_MODE; return; // distance / monsters view (optional mode)
        case 'n': st->mode = NORMAL_MODE;   return; // normal view (optional mode)
        //case 'm': // TODO                 return; // menu view (optional mode) TODO: implement
    }
    st->turn++;
    int x = st->player->x, y = st->player->y;
    x += dx; y += dy;

    if (isCellWalkable(st->map->cells[y][x]) && !isCellMonster(st->map->cells[y][x])) {
        // Moves the player
        st->map->cells[st->player->y][st->player->x]->has_player = 0; // false
        st->player->x = x;
        st->player->y = y;
        st->map->cells[y][x]->has_player = 1; // true
        st->player->direction = direction;
        // if the player moves we recalculate the distances and is_visible
        calculateDistances(st->map, x, y); // TODO enhance this (unnecessary calculations) by using updateDistances function
        calculateVision(st->map, x, y);
    }
    if (isCellItem(st->map->cells[y][x]) && !isCellMonster(st->map->cells[y][x])) {
        // TODO pick up item (inventory.h)
        // Find the item in the map->items array
        Item *item = getItem(st->map->items, st->map->nr_items, x, y);
        if (item != NULL) {
            addItem(st->player->inventory, item);
            st->map->nr_items--;
            // getItem removes the item from the map->items array by shifting the items to the left
            // so we only need to decrease the number of items in the map

            char *menu_message = (char *) malloc(sizeof(char) * st->map->width);
            sprintf(menu_message, "You picked up \"%s\"", item->name);
            sendMenuMessage(st, menu_message);
            free(menu_message);
        }
    }
    if (isCellMonster(st->map->cells[y][x])) {
        char *message = (char *) malloc(sizeof(char) * st->map->width);
        Monster *monster = st->monsters[st->map->cells[y][x]->monster_index];
        sprintf(message, "You attacked the \"%s\". Monster health: %d", monster->name, monster->health);
        sendMenuMessage(st, message);
        free(message);
        // TODO combat monster (combat.h)
    }
    /* if (isCellExit(st->map->cells[y][x])) {
        st->mode = EXIT_MODE;
    } */
    moveMonsters(st->map, st->monsters, st->nMonsters);
    updateProjectiles(st);
}
