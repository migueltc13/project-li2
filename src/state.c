#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "state.h"
#include "player.h"
#include "map.h"
#include "cell.h"
#include "item.h"
#include "inventory.h"
#include "monster.h"
#include "combat.h"
#include "menu.h"

#define NUMBER_OF_ITEMS 30 // TODO: move to item.h or even map.h

/**
 * @brief Initialize a new game state
 *
 * @details Generates the map, the player and the monsters
 *
 * @return State*
 */
State *initState(int width, int height)
{
    State *st = (State *)malloc(sizeof(struct state));

    // Set the state configs
    st->turn = 0;
    st->first_pass = 4;
    st->second_pass = 4;
    st->wall_prob = 40;
    st->mode = DEFAULT_MODE;

    // Set the map
    st->map = initMap(width, height);
    generateMap(st);

    // Set the player
    int x, y;
    getPlayerInitialPosition(st->map, &x, &y);
    st->player = initPlayer(x, y);

    // Set the exit (aka the gate keeper)
    Cell *exit = st->map->cells[y + 1][x];
    exit->symbol = EXIT_SYMBOL;
    exit->is_walkable = 0;
    exit->color = EXIT_COLOR;

    // Generate items
    Item **items = generateItems(NUMBER_OF_ITEMS);
    distributeItems(st->map, items, NUMBER_OF_ITEMS);

    // Generate monsters
    st->nMonsters = NUMBER_OF_MONSTERS;
    st->monsters = generateMonsters(st->map);

    // Calculate the distances and visibility of the player
    calculateDistances(st->map, x, y);
    calculateVision(st->map, st->player);
    return st;
}

/**
 * @brief Free the game state
 * 
 * @details Free the map, the player and the monsters
 * 
 * @param p a pointer to the State to free 
 */
void freeState(void *p) {
    State *st = (State *) p;
    freeMap(st->map);
    freePlayer(st->player);
    for (int i = 0; i < st->nMonsters; i++)
        freeMonster(st->monsters[i]);
    free(st->monsters);
    free(st);
}

/**
 * @brief Update the game state. Used in the main loop.
 * 
 * @details Calculates the new state based on the input key.
 * Handles the exit and quit modes, then draws the new state.
 * 
 * @param st The game state to update
 * @param input_key The input key from player
 */
void updateState(State *st, int input_key) {
    calculateState(st, input_key);
    if (st->mode == EXIT_MODE) {
        // Winner / Loser
        if (st->player->health <= 0) {
            attron(COLOR_PAIR(COLOR_RED));
            sendMenuMessage(st, "You died. Game over. (Press q key to exit.)");
            attroff(COLOR_PAIR(COLOR_RED));
        }
        else {
            attron(COLOR_PAIR(COLOR_GREEN));
            sendMenuMessage(st, "You payed the Gate Keeper and escaped the dungeon. (Press q key to exit.)");
            attroff(COLOR_PAIR(COLOR_GREEN));
        }
        while (getch() != 'q')
            ;;
        freeState(st);
        endwin();
        exit(EXIT_SUCCESS);
    }
    else if (st->mode == QUIT_MODE) {
        attron(COLOR_PAIR(COLOR_YELLOW));
        sendMenuMessage(st, "Do you really want to quit? (y/n)");
        attroff(COLOR_PAIR(COLOR_YELLOW));
        char quit_input = getch();
        if (quit_input == 'y') {
            freeState(st);
            endwin();
            exit(EXIT_SUCCESS);
        }
        else {
            sendMenuMessage(st, ""); // clear menu message
            st->mode = VISION_MODE; // default mode
            drawState(st);
        }
    }
    else drawState(st);
}

/**
 * @brief Draw the game state
 * 
 * @param st The game state to draw
 */
void drawState(State *st) {
    // clear();
    drawMap(st->map, st->mode); // TODO: mode outside drawMap()
    // draw projectiles in all modes
    drawProjectiles(st->map);

    if (st->mode == VISION_MODE)
        drawVisibleItems(st->map, st->map->items, st->map->nr_items);
    else if (st->mode == NORMAL_MODE)
        drawAllItems(st->map->items, st->map->nr_items);
    // No items in distance mode

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
        case 'i': switchEquippedItem(st->player->inventory);   return; // switch equipped item
        case 'o': sellEquippedItem(st, st->player->inventory); return; // sell equiped item
        // Combat + Inventory keys
        case KEY_ENTER:
        case ' ':
        case 'u': useEquippedItem(st); return; // Use equipped item
        // Menu keys
        case 'l': legendMenu(st);           return; // legend menu
        case 'q': st->mode = QUIT_MODE;     return; // quit
        case 'v': st->mode = VISION_MODE;   return; // normal view         
        case 'b': st->mode = DISTANCE_MODE; return; // distance / monsters view (optional mode)
        case 'n': st->mode = NORMAL_MODE;   return; // normal view (optional mode)
        default: return;
    }
    // If the cell is not walkable, don't update the state
    if (!isCellWalkable(st->map->cells[st->player->y + dy][st->player->x + dx])) return;

    // Increment the turn number
    st->turn++;

    // Update the player direction
    if (direction != 0) st->player->direction = direction;

    // Update the player potion effects
    updatePotionEffects(st->player);

    // Update the player position and the map
    int x = st->player->x + dx;
    int y = st->player->y + dy;

    if (!isCellMonster(st->map->cells[y][x])) {
        // Moves the player
        st->map->cells[st->player->y][st->player->x]->has_player = 0; // false
        st->player->x = x;
        st->player->y = y;
        st->map->cells[y][x]->has_player = 1; // true
        // if the player moves we recalculate the distances, bomb effects and visibility
        calculateDistances(st->map, x, y);
        calculateVision(st->map, st->player);
    }

    if (isCellItem(st->map->cells[y][x]) && !isCellMonster(st->map->cells[y][x]))
        pickUpItem(st);        

    if (isCellMonster(st->map->cells[y][x])) {
        int monster_index = st->map->cells[y][x]->monster_index;
        Monster *monster = st->monsters[monster_index];

        // TODO enhance damage with fight monster function (combat.h)
        monster->health -= st->player->attack;
        st->player->health -= monster->attack;

        // Send message to the menu: Monster name and health
        char *message = (char *) malloc(sizeof(char) * st->map->width);
        snprintf(message, st->map->width, "You attacked the \"%s\". Monster health: %d", monster->name, monster->health);
        sendMenuMessage(st, message);
        free(message);

        // Check if the monster is death
        if (monster->health <= 0) {
            // Send message to the menu: Monster name and gold value
            char *message = (char *) malloc(sizeof(char) * st->map->width);
            snprintf(message, st->map->width, "You killed the \"%s\" and got %d gold!", monster->name, monster->gold);
            sendMenuMessage(st, message);
            free(message);

            // Kill and remove the monster from the map
            killMonster(st, monster->x, monster->y);
        }
        
        // Check for player death
        if (st->player->health <= 0) {
            sendMenuMessage(st, "You died. Game over.");
            st->mode = EXIT_MODE;
            drawState(st);
            return;
        }        
    }
    if (isCellExit(st->map->cells[y][x]) && !isCellMonster(st->map->cells[y][x])) {
        // Pay the keeper to exit the dungeon
        if (st->player->inventory->gold >= EXIT_COST) {
            st->mode = EXIT_MODE;
            return;
        }
        else {
            char *message = (char *) malloc(sizeof(char) * st->map->width);
            snprintf(message, st->map->width, "The Gate Keeper says: \"Give me %d gold to exit now!\"", EXIT_COST);
            sendMenuMessage(st, message);
            free(message);
        }
    }
    moveMonsters(st, st->monsters, st->nMonsters);
    updateProjectiles(st);
}
