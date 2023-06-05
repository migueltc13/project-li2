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
 * Returns NULL if the state could not be initialized
 * 
 * @return State*
 */
State *initState(int width, int height)
{
    State *st = (State *) malloc(sizeof(struct state));
    if (st == NULL) {
        endwin();
        fprintf(stderr, "Fatal: Failed to allocate memory for the game state\n");
        exit(EXIT_FAILURE);
    }

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
    int x = 0, y = 0;
    getPlayerInitialPosition(st->map, &x, &y);
    st->player = initPlayer(x, y);
    if (x == 0 && y == 0) {
        endwin();
        fprintf(stderr, "Fatal: Failed to find player initial position.\n");
        exit(EXIT_FAILURE);
    }

    // Set the exit (aka the gate keeper)
    Cell *exit = st->map->cells[y + 1][x];
    exit->symbol = EXIT_SYMBOL;
    exit->color = EXIT_COLOR;
    sendMenuMessage(st, "Gate Keeper: \"Never forget where you came from!\"");

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
 * @brief Draw the game state
 * 
 * @details Draws the map, the player, the monsters, the items and the projectiles
 * 
 * @param st The game state to draw
 */
void drawState(State *st) {
    drawMap(st->map, st->mode);
    drawProjectiles(st->map); // draw projectiles indepedent of st->mode

    if (st->mode == VISION_MODE)
        drawVisibleItems(st->map, st->map->items, st->map->nr_items);
    else if (st->mode == NORMAL_MODE)
        drawAllItems(st->map->items, st->map->nr_items);
    // No items are drawed in distance mode

    drawPlayer(st->player);

    drawMonsters(st);
    
    if (st->map->has_menu)
        generateMenu(st);

    refresh();
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
        drawState(st);
        // Winner / Loser
        if (st->player->health <= 0) {
            attron(COLOR_PAIR(COLOR_RED));
            sendMenuMessage(st, "You died. Game over. (Press q key to exit.)");
            attroff(COLOR_PAIR(COLOR_RED));
        }
        else {
            st->player->inventory->gold -= EXIT_COST;
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

    // Get the player
    Player *player = st->player;

    // Update the player position and the map
    int x = player->x + dx;
    int y = player->y + dy;

    // Get the next cell
    Cell *next_cell = st->map->cells[y][x];

    // Update the player direction
    if (direction != 0) player->direction = direction;

    // If next cell is not walkable, don't update the state (only update the player direction)
    if (!isCellWalkable(next_cell)) return;

    // Increment the turn number
    st->turn++;

    // Update projectiles and potion effects
    updateProjectiles(st);
    updatePotionEffects(st->player);

    if (!isCellMonster(next_cell) && !isCellExit(next_cell)) {
        // Moves the player
        st->map->cells[st->player->y][st->player->x]->has_player = 0; // false
        st->player->x = x;
        st->player->y = y;
        next_cell->has_player = 1; // true

        // Pick up the item if any
        if (isCellItem(next_cell))
            pickUpItem(st);

        // if the player moves we recalculate the distances, bomb effects, visibility and noises
        calculateDistances(st->map, x, y);
        calculateVision(st->map, st->player);

        // Send message to the menu: Noise based on distance close, medium or faraway
        if (!isCellItem(next_cell)) { // this if statement avoids losing menu messages when picking up items
            Monster *monster = getCloserUnhurtMonster(st);
            if (monster != NULL) {
                int chance = rand() % 100;
                if (chance < 100 - st->player->earing_prob) return;
                int distance = st->map->cells[monster->y][monster->x]->distance_to_player;
                char *message = (char *) malloc(sizeof(char) * st->map->width);
                if (distance <= st->player->earing_range_close)
                    snprintf(message, st->map->width, "You hear %s very close to you.", monster->noise);
                else if (distance <= st->player->earing_range_far)
                    snprintf(message, st->map->width, "You hear %s near by.", monster->noise);
                else
                    snprintf(message, st->map->width, "You hear %s faraway.", monster->noise);
                sendMenuMessage(st, message);
                free(message);
            }
        }
    }

    if (isCellMonster(next_cell)) {
        int monster_index = next_cell->monster_index;
        Monster *monster = st->monsters[monster_index];
        moveMonsters(st, st->monsters, st->nMonsters); // move the monsters when the player attacks
        if (st->mode == EXIT_MODE) return; // if the player died break the function
        playerAttacksMonster(st, monster);
        return;
    }
    if (isCellExit(next_cell)) {
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
    // Update the monsters if the game is not over
    if (st->mode != EXIT_MODE)
        moveMonsters(st, st->monsters, st->nMonsters);
}
