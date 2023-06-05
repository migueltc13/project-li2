#ifndef _PLAYER_H_
#define _PLAYER_H_

/// @brief default symbol of the player
#define PLAYER_SYMBOL '@'
/// @brief default color of the player
#define PLAYER_COLOR COLOR_WHITE
/// @brief maximum health of the player
#define MAX_HEALTH 100
/// @brief starting attack of the player
#define PLAYER_STARTING_ATTACK 5
/// @brief starting defense of the player
#define PLAYER_STARTING_DEFENSE 2

/// @brief range of the vision of the player (cells)
#define PLAYER_VISION 10 // cells
/// @brief width of the player's vision (cells)
#define PLAYER_VISION_WIDTH 1 // cells (nr adjacent lines of sight = 2 * PLAYER_VISION_WIDTH)

/// @brief range of hearing a close monster (cells)
#define PLAYER_EARING_RANGE_CLOSE 10
/// @brief range of hearing a far monster (cells)
#define PLAYER_EARING_RANGE_FAR 40
/// @brief probability of hearing a close monster when a player moves (in %)
#define PLAYER_EARING_PROB 30


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
 * @param max_health Maximum health of the player set to MAX_HEALTH macro
 * @param defense Defense of the player
 * @param attack Attack of the player
 * @param vision Vision of the player in cells
 * @param vision_width Vision width of the player in cells
 * @param earing_range_close Earling range of the player in cells
 * @param earing_range_far Earling range of the player in cells
 * @param earing_prob Probability of hearing a close monster when a player moves (in %)
 * @param sensory_potion_turns Number of turns the player has sensory potion effect
 * @param potion_invincibility_turns Number of turns the player has invincibility potion effect
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
    int vision;
    int vision_width;
    int earing_range_close;
    int earing_range_far;
    int earing_prob;
    int sensory_potion_turns;
    int potion_invincibility_turns;
    int max_health;
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
