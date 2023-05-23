#ifndef _STATE_H_
#define _STATE_H_

#define NORMAL_MODE 0
#define DISTANCE_MODE 1
#define VISION_MODE 2
#define EXIT_MODE -1

typedef struct map Map; // or #include "map.h"
typedef struct player Player; // or #include "player.h"
typedef struct monster Monster; // or #include "monster.h"

/**
 * @brief Sructure of the game state
 * 
 * @param turn The turn of the game
 * @param mode The mode of the game, 0: normal, 1: distance, 2 vision, -1: exit mode
 * @param map The map of the game
 * @param first_pass The first pass is used in the generation of the map
 * @param second_pass The second pass is used in the generation of the map as well
 * @param wall_prob The probability of a wall to be generated, default is 40%
 * @param player The player of the game
 * @param monsters The monsters of the game
 * @param nMonsters The number of monsters
*/
typedef struct state {
    int turn;
    int mode;

    Map *map;
    int first_pass;
    int second_pass;
    int wall_prob;

    Player *player;
    Monster **monsters;
    int nMonsters;
} State;

State *initState(int width, int height);
void freeState(void *p);
void updateState(State *st, int input_key);
void drawState(State *st);
void calculateState(State *st, int input_key);

#endif
