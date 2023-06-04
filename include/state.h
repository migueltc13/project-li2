#ifndef _STATE_H_
#define _STATE_H_

#define DEFAULT_MODE VISION_MODE
#define NORMAL_MODE 0
#define DISTANCE_MODE 1
#define VISION_MODE 2 // default mode
// #define CAVERN_MODE 3 // TODO: implement cavern mode
#define EXIT_MODE -1
#define QUIT_MODE -2 // When the player quits the game

/** 
 * @brief The cost of the exit
 * 
 * @details The cost of the maze exit is 500 gold.
 * It's the only way to win the game.
 * Pay the cost by stepping on the Gate Keeper.
*/
#define EXIT_COST 500 // gold

typedef struct map Map;
typedef struct player Player;
typedef struct monster Monster;

/**
 * @brief Sructure of the game state
 * 
 * @param turn The current turn of the game
 * @param mode The mode of the game, 0: normal, 1: distance, 2 vision, -1: exit mode
 * @param map The map of the game, contains the cells, items and projectiles
 * @param first_pass The first pass is used in the generation of the map
 * @param second_pass The second pass is used in the generation of the map as well
 * @param wall_prob The probability of a wall to be generated, default is 40%
 * @param player The player of the game, contains the inventory
 * @param monsters The monsters of the game (array of pointers)
 * @param nMonsters The number of monsters
*/
typedef struct state {
    unsigned long int turn;
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
