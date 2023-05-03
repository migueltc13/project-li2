#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "player.h"
#include "map.h"

typedef struct state {
    Map *map;
    Player *player;
    // Monster **monsters;
} State;


/**
 * @brief Initialize a new game state
 * - generate the map
 * 
 * @return State*
 */ 
State *initState(int width, int height) {
    State *st = (State *) malloc(sizeof(struct state));
    st->map = initMap(width, height);
    generateMap(st->map);
    // TODO Function to get player initial position 
    st->player = initPlayer(10,10);
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
 * @param State *st 
 * @param int input_key 
 * @return void
 */
void updateState(State *st, int input_key) {
    // TODO
    parseInput(input_key, st->player);
    drawState(st);
}

/**
 * @brief Draw the game state
 * 
 * @param State *st 
 * @return void
 */
void drawState(State *st) {
    drawMap(st->map);
    drawPlayer(st->player);
}
