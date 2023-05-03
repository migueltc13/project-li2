#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "player.h"

#define PLAYER_SYMBOL '@'
#define MAX_HEALTH 100

typedef struct player {
    unsigned int x;
    unsigned int y;
    char symbol;
    int health;
    // ...
} Player;

Player *initPlayer(int x, int y) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    p->symbol = PLAYER_SYMBOL;
    p->health = MAX_HEALTH;
    return p;
}

// add map or use funtion is_valid_move(map, x, y) on coords x+dx, y+dy
/** 
 * @brief Moves the player by dx and dy
 * 
 * Assumes that the player is on the map and that the move is valid.
 * 
 * @param p Pointer to the player
 * @param dx Change in x
 * @param dy Change in y
 */
void movePlayer(Player *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

/**
 * @brief Parse input key and updates x,y player coordinates
 * 
 * @param input_key
 * @param player
*/
void parseInput(int input_key, Player* player) {
    switch(input_key) {
        // Movement keys
        // TODO add map or use funtion is_valid_move(map, x, y) on coords x+dx, y+dy
        // TODO add arrow keys
        case KEY_A1:
        case '7': movePlayer(player, -1, -1); break;
        case KEY_UP:
        case '8': movePlayer(player, -1, +0); break;
        case KEY_A3:
        case '9': movePlayer(player, -1, +1); break;
        case KEY_LEFT:
        case '4': movePlayer(player, +0, -1); break;
        case KEY_B2:
        case '5': break;
        case KEY_RIGHT:
        case '6': movePlayer(player, +0, +1); break;
        case KEY_C1:
        case '1': movePlayer(player, +1, -1); break;
        case KEY_DOWN:
        case '2': movePlayer(player, +1, +0); break;
        case KEY_C3:
        case '3': movePlayer(player, +1, +1); break;
        // Menu keys
        case 'q': endwin(); exit(0); break; // TODO free memory
	}
}

void drawPlayer(Player *p) {
    // TODO: Add color to player; use attron(COLOR_PAIR(1)) and attroff(COLOR_PAIR(1))
    mvaddch(p->y, p->x, p->symbol);
}

void freePlayer(void *p) {
    Player *player = (Player *) p;
    // p->x = 0;
    // p->y = 0;
    // p->symbol = ' ';
    // p->health = 0;
    free(player);
}
