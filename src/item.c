#include <stdlib.h>
#include <ncurses.h>
#include "item.h"
#include "map.h"
#include "cell.h"

/**
 * @brief Initialize a new item
 * 
 * @param x Item x coordinate on the map if any
 * @param y Item y coordinate on the map if any
 * @param name Item name
 * @return Item*
 */
Item *initItem(char *name,
               char symbol,
               int type,
               int value,
               int is_picked_up,
               int x,
               int y,
               int damage,
               int defense,
               int hp)
{
    Item *i = (Item *) malloc(sizeof(Item));
    i->name = name; // Aloccate memory for the name + strdup
    i->symbol = symbol;
    i->type = type;
    i->value = value;
    i->is_picked_up = is_picked_up;
    i->x = x;
    i->y = y;
    i->damage = damage;
    i->defense = defense;
    i->hp = hp;
    return i;
}

/**
 * @brief Initialize a new item: rock (weapon projectile)
 * 
 * Coords: 0, 0 (not on the map)
 */
Item *initRock() {
    int damage = ROCK_DAMAGE_MIN + rand() % (ROCK_DAMAGE_MAX - ROCK_DAMAGE_MIN + 1);
    return initItem("rock", ':', 1, ROCK_VALUE, 0, 0, 0, damage, 0, 0);
}

// ----------------

/**
 * @brief Initialize a new item: Smoke bomb (weapon projectile)
 */

/**
 * @brief Initialize a new item: Fire bomb (weapon projectile)
 */

/**
 * @brief Initialize a new item: Sword (weapon body)
 */

/**
 * @brief Initialize a new item: Pot of gold
 */

/**
 * @brief Initialize a new item: Potion of healing
 */

/**
 * @brief Initialize a new item: Potion of vision
 */

// ----------------

/**
 * @brief Generate a random item
 * 
 * TODO: Global variables for the probability of each item
*/
Item* generateItem() {
    int r = rand() % 100;
    // 20% chance of generating a rock
    if (r < 20)
        return initRock();
    
    // 10% chance of generating a smoke bomb
    if (r < 30)
        return initRock(); // TODO: initSmokeBomb();
    
    // 10% chance of generating a fire bomb
    if (r < 40)
        return initRock(); // TODO: initFireBomb();
    
    // 10% chance of generating a sword
    if (r < 50)
        return initRock(); // TODO: initSword();
    
    // 10% chance of generating a pot of gold
    if (r < 60)
        return initRock(); // TODO: initPotOfGold();
    
    // 10% chance of generating a potion of healing
    if (r < 70)
        return initRock(); // TODO: initPotionOfHealing();
    
    // 10% chance of generating a potion of vision
    if (r < 80)
        return initRock(); // TODO: initPotionOfVision();
    
    // 5% chance of generating a leather armor
    if (r < 85)
        return initRock(); // TODO: initLeatherArmor();
    
    // 3% chance of generating a chainmail armor
    if (r < 88)
        return initRock(); // TODO: initChainmailArmor();
    
    // 2% chance of generating a plate armor
    if (r < 90)
        return initRock(); // TODO: initPlateArmor();
    
    // 10% chance of generating a potion of invincibility
    else
        return initRock(); // TODO: initPotionOfInvincibility();
}

/**
 * @brief Generate n *random* items
 * 
 * @param n Number of items to generate
 * @return Item** Array of items
 */
Item** generateItems(int n) {
    // Allocate memory for the array of n items
    Item **items = (Item **) malloc(n * sizeof(Item *));
    // Generate n items
    for (int i = 0; i < n; i++)
        items[i] = generateItem();
    return items;
}

void pickUpItem() {
    // TODO
}

void dropItem() {
    // TODO
}

/**
 * @brief Draw an item
 * 
 * @param i Item to draw
 * @return void
 */
void drawItem(Item *i) {
    // TODO color
    mvaddch(i->y, i->x, i->symbol);
}

/**
 * @brief Draw all items in the Map
 * 
 * @param items Array of items to draw
 * @param n Number of items in the array
 * @return void
 */
void drawAllItems(Item **items, int n) {
    for (int i = 0; i < n; i++) {
        drawItem(items[i]);
    }
}

/**
 * @brief Draw all visible items in the Map
 * 
 * @param items Array of items to draw
 * @param n Number of items in the array
 * @return void
 */
void drawVisibleItems(Map* map, Item **items, int n) {
    for (int i = 0; i < n; i++) {
        if (items[i] != NULL) {
            if (map->cells[items[i]->y][items[i]->x] != NULL) { 
                if (map->cells[items[i]->y][items[i]->x]->is_visible)
                    drawItem(items[i]);
            }
        }
    }
}

/**
 * @brief Get item at (x, y) in the map, also removes it from the array
*/
Item* getItem(Item **items, int nr_items, unsigned int x, unsigned int y) {
    for (int i = 0; i < nr_items; i++) {
        if (items[i]->x == x && items[i]->y == y) {
            Item *item = items[i];
            // Remove item from the array
            for (int j = i; j < nr_items - 1; j++)
                items[j] = items[j + 1];
            
            return item;
        }
    }
    return NULL;
}

/** 
 * @brief Free an item
 * 
 * @param i Item to free
 * @return void
 */
void freeItem(void *i) {
    free(i);
}
