#include <stdlib.h>
#include <ncurses.h>
#include "player.h"

/**
 * @brief Initializes a new player. Allocates memory for a new player and initializes its fields.
 * 
 * @param x Player x initial coordinate
 * @param y Player y initial coordinate
 * @return Player* Initialized player at (x,y)
 */
Player *initPlayer(int x, int y) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    p->symbol = PLAYER_SYMBOL;
    p->health = MAX_HEALTH;
    p->vision = PLAYER_VISION;
    return p;
}

/**
 * @brief Draws the player on the screen.
 * 
 * TODO: colors damage the player and add color to player
 * 
 * @param p Player to draw
 * @return void
 */
void drawPlayer(Player *p) {
    // TODO: Add color to player; use attron(COLOR_PAIR(1)) and attroff(COLOR_PAIR(1))
    mvaddch(p->y, p->x, p->symbol);
}

/**
 * @brief frees the player. Sets all fields to 0 and frees the memory.
 * 
 * @param p Player to free
 * @return void
 */
void freePlayer(void *p) {
    Player *player = (Player *) p;
    player->x = 0;
    player->y = 0;
    player->symbol = '\0';
    player->health = 0;
    player->vision = 0;
    free(player);
}
