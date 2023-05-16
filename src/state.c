#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
// #include <mybool.h> TODO: implement
#include "state.h"
#include "player.h"
#include "map.h"
#include "cell.h"

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
    calculateDistances(st->map, x, y);
    return st;
}

/**
 * @brief Free the game state
 * 
 * @param void *p (a pointer to a State) 
 * @return void
 */
void freeState(void *p) {
    State *st = (State *) p;
    freeMap(st->map);
    freePlayer(st->player);
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
    drawMap(st->map, st->mode);
    drawPlayer(st->player);
}

/**
 * @brief Calulates the state based on the input key
 * 
 * @param State *st
 * @param int input_key
 * @return void 
 * (0: normal view, 1: distance view enabled, -1: the player win/lost/quit)
*/
void calculateState(State *st, int input_key) {
    int dx = 0, dy = 0;
    switch(input_key) {
        // Movement keys
        case KEY_A1:
        case '7': dx = -1; dy = -1; break;
        case KEY_UP:
        case 'w':
        case '8': dx =  0; dy = -1; break;
        case KEY_A3:
        case '9': dx =  1; dy = -1; break;
        case KEY_LEFT:
        case 'a':
        case '4': dx = -1; dy =  0; break;
        case KEY_B2:
        case '5': break;
        case KEY_RIGHT:
        case 'd':
        case '6': dx =  1; dy =  0; break;
        case KEY_C1:
        case '1': dx = -1; dy =  1; break;
        case KEY_DOWN:
        case 's':
        case '2': dx =  0; dy =  1; break;
        case KEY_C3:
        case '3': dx =  1; dy =  1; break;
        // Menu keys
        case 'n': st->mode = NORMAL_MODE;   return; // normal view
        case 'v': st->mode = VISION_MODE;   return; // normal view
        case 'm': st->mode = DISTANCE_MODE; return; // distance view
        case 'q': st->mode = EXIT_MODE;     return; // quit
	}
    int x = st->player->x, y = st->player->y;
    x += dx; y += dy;

    if (isCellWalkable(st->map->cells[y][x])) {
        // Moves the player
        st->player->x = x;
        st->player->y = y;
        // if the player moves we recalculate the distances and is_visible
        calculateDistances(st->map, x, y); // TODO enhance this (unnecessary calculations) by using updateDistances function
    }   calculateVision(st->map, x, y);
    /*
    else if (isCellMonster(st->map, x, y)) {
        // TODO attack monster (combat.h)
        return 2;
    }
    */
}
