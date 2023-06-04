#include <stdlib.h>
#include <ncurses.h>
#include "player.h"
#include "item.h" // macro POTION_OF_INVINCIBILITY_DEFENSE
#include "inventory.h"

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
    p->defense = 0;
    p->attack = PLAYER_STARTING_ATTACK;
    p->vision = PLAYER_VISION;
    p->vision_width = PLAYER_VISION_WIDTH;
    p->sensory_potion_turns = -1; // -1 means no sensory potion effect
    p->potion_invincibility_turns = -1; // -1 means no invincibility potion effect
    p->max_health = MAX_HEALTH;
    p->direction = 0;
    p->inventory = initInventory();
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
    attron(COLOR_PAIR(p->color));
    mvaddch(p->y, p->x, p->symbol);
    attroff(COLOR_PAIR(p->color));
}

/**
 * @brief Updates the potion effects of the player.
 * 
 * Sensory potion: increases the player vision and earing
 * Invincibility potion: increases the player defense
 * 
 * @param p Player to update
 * @return void
 */
void updatePotionEffects(Player *p) {
    if (p->sensory_potion_turns > 0) {
        p->sensory_potion_turns--;
    }
    if (p->sensory_potion_turns == 0) {
        // Reset player vision to default
        p->vision = PLAYER_VISION;
        p->vision_width = PLAYER_VISION_WIDTH;
        p->color = PLAYER_COLOR; // Reset player color to default
        p->sensory_potion_turns = -1; // Reset potion effect
    }

    if (p->potion_invincibility_turns > 0) {
         p->potion_invincibility_turns--;
    }
    if (p->potion_invincibility_turns == 0) {
        p->defense -= POTION_OF_INVINCIBILITY_DEFENSE;
        p->color = PLAYER_COLOR; // Reset player color to default
        p->potion_invincibility_turns = -1; // Reset potion effect
    }
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
    player->direction = 0;
    freeInventory(player->inventory);
    free(player);
}
