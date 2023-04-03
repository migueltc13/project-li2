#ifndef _STATE_H_
#define _STATE_H_

typedef struct player *Player;

// Struct to hold the state of the game
typedef struct state {
    //WINDOW *mainwin; // maybe ?
    Player player;
    //Mobs *mobs;
    //Map *map;
} *State;

State newState();
void updateState(State st, int input_key);
void freeState(State st);

#endif
