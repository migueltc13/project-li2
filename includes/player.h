#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct position *Position;

// Struct that represents a player
typedef struct player {
    char symbol;
    unsigned int health; // 0-100
    Position position;
    //Inventory *inventory;
} *Player;

Player newPlayer(char symbol, Position pos);
void freePlayer(Player p);

#endif
