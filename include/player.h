#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_SYMBOL '@'
#define PLAYER_COLOR COLOR_WHITE
#define MAX_HEALTH 100
#define PLAYER_VISION 16 // cells
#define PLAYER_VISION_WIDTH 3 // cells (total range = 2 * PLAYER_VISION_WIDTH + 1)
#define PLAYER_STARTING_ATTACK 5

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
 * @param vision_width Vision width of the player set to PLAYER_VISION_WIDTH macro
 * @param sensory_potion_turns Number of turns the player has sensory potion effect
 * @param potion_invincibility_turns Number of turns the player has invincibility potion effect
 * @param max_health Maximum health of the player set to MAX_HEALTH macro
 * @param direction Direction of the player (used for throwing projectiles)
 * @param color Color of the player changes when he takes damage or uses a potion
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
    int vision_width; // PLAYER_VISION_WIDTH
    int sensory_potion_turns;
    int potion_invincibility_turns;
    int max_health; // MAX_HEALTH
    int direction;
    int color;
    Inventory *inventory;
} Player;

Player *initPlayer(int x, int y);
//void movePlayer(Player *p, int dx, int dy);
void updatePotionEffects(Player *p);
void drawPlayer(Player *p);
void freePlayer(void *p);

#endif
