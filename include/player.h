#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_SYMBOL '@'
#define MAX_HEALTH 100
#define PLAYER_VISION 20 // cells

typedef struct inventory Inventory;

/** @brief Structure of a player
 * 
 * @details A player is a singular game unit with 2D coordinates,
 * a symbol, a health, a defense, an attack, a vision and more. TODO
 * 
 * @param symbol Character to be displayed on the screen
 * @param x player x coordinate
 * @param y player y coordinate
 * @param health Health of the player
 * @param defense Defense of the player
 * @param attack Attack of the player
 * @param vision Vision of the player set to PLAYER_VISION macro
 * @param max_health Maximum health of the player set to MAX_HEALTH macro
 * @param direction Direction of the player (used for throwing projectiles)
 * @param inventory Inventory of the player
 */
typedef struct player {
    unsigned int x;
    unsigned int y;
    char symbol;
    int health;
    int defense;
    int attack;
    int vision; // PLAYER_VISION
    int max_health; // MAX_HEALTH
    int direction;
    Inventory *inventory;
} Player;

Player *initPlayer(int x, int y);
//void movePlayer(Player *p, int dx, int dy);
void drawPlayer(Player *p);
void freePlayer(void *p);

#endif
