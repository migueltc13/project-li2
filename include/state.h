#ifndef _STATE_H_
#define _STATE_H_

#define NORMAL_MODE 0
#define DISTANCE_MODE 1
#define VISION_MODE 2
#define EXIT_MODE -1

typedef struct map Map; //#include "map.h"
typedef struct player Player; //#include "player.h"

/**
 * @brief Sructure of the game state
 * 
 * Includes:
 * - Map
 * - Player
 * - Monsters (Queue)
 * - Turn number
*/
typedef struct state {
    Map *map;
    int first_pass;
    int second_pass;
    int wall_prob;
    Player *player;
    // Monster **monsters;
    // int nMonsters;
    int turn;
    int mode; // 0: normal, 1: distance, -1: exit
} State;

State *initState(int width, int height);
void freeState(void *p);
void updateState(State *st, int input_key);
void drawState(State *st);
void calculateState(State *st, int input_key);

#endif
