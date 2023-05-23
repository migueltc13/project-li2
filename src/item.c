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
 * @param symbol Item symbol
 * @param type Item type
 * @param value Item value
 * @param is_picked_up Is the item picked up?
 * @param damage Item damage
 * @param defense Item defense
 * @param hp Item hp (Healing potion)
 * @param color Item color
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
               int hp,
               int color)
{
    Item *i = (Item *) malloc(sizeof(Item));
    i->name = name;
    i->symbol = symbol;
    i->type = type;
    i->value = value;
    i->is_picked_up = is_picked_up;
    i->x = x;
    i->y = y;
    i->damage = damage;
    i->defense = defense;
    i->hp = hp;
    i->count = 1;
    i->color = color;
    return i;
}

/**
 * @brief Initialize a new item: rock (weapon projectile)
 * 
 * Coords: 0, 0 (not on the map)
 */
Item *initRock() {
    int damage = ROCK_DAMAGE_MIN + rand() % (ROCK_DAMAGE_MAX - ROCK_DAMAGE_MIN + 1);
    return initItem("Rock", ROCK_SYMBOL, ROCK_TYPE, ROCK_VALUE, 0, 0, 0, damage, 0, 0, ROCK_COLOR);
}

/**
 * @brief Initialize a new item: Smoke bomb (weapon projectile)
 * 
 * TODO: Implement smoke bomb effect
 */
Item *initSmokeBomb() {
    int damage = SMOKE_BOMB_DAMAGE_MIN + rand() % (SMOKE_BOMB_DAMAGE_MAX - SMOKE_BOMB_DAMAGE_MIN + 1);
    return initItem("Smoke bomb", SMOKE_BOMB_SYMBOL, SMOKE_BOMB_TYPE, SMOKE_BOMB_VALUE, 0, 0, 0, damage, 0, 0, SMOKE_BOMB_COLOR);
}

/**
 * @brief Initialize a new item: Fire bomb (weapon projectile)
 *
 * TODO: Implement fire bomb effect
 */
Item *initFireBomb() {
    int damage = FIRE_BOMB_DAMAGE_MIN + rand() % (FIRE_BOMB_DAMAGE_MAX - FIRE_BOMB_DAMAGE_MIN + 1);
    return initItem("Fire bomb", FIRE_BOMB_SYMBOL, FIRE_BOMB_TYPE, FIRE_BOMB_VALUE, 0, 0, 0, damage, 0, 0, FIRE_BOMB_COLOR);
}

/**
 * @brief Initialize a new item: Sword (weapon body)
 */
/* Item *initSword() {
    // TODO is_stackable = false
    int attack = SWORD_ATTACK_MIN + rand() % (SWORD_ATTACK_MAX - SWORD_ATTACK_MIN + 1);
    return initItem("Sword", SWORD_SYMBOL, SWORD_TYPE, SWORD_VALUE, 0, 0, 0, attack, 0, 0, SWORD_COLOR);
} */

/**
 * @brief Initialize a new item: Pot of gold
 */
Item *initPotOfGold() {
    int value = POT_OF_GOLD_VALUE_MIN + rand() % (POT_OF_GOLD_VALUE_MAX - POT_OF_GOLD_VALUE_MIN + 1);
    return initItem("Pot of Gold", POT_OF_GOLD_SYMBOL, POT_OF_GOLD_TYPE, value, 0, 0, 0, 0, 0, 0, POT_OF_GOLD_COLOR);
}

/**
 * @brief Initialize a new item: Potion of healing
 */
Item *initPotionOfHealing() {
    return initItem("Potion of Healing", POTION_OF_HEALING_SYMBOL, POTION_OF_HEALING_TYPE, POTION_OF_HEALING_VALUE, 0, 0, 0, 0, 0, POTION_OF_HEALING_HP, POTION_OF_HEALING_COLOR);
}

/**
 * @brief Initialize a new item: Sensory Potion
 *
 * TODO: Implement sensory potion effect
 */
/* Item *initSensoryPotion() {
    return initItem("Sensory Potion", SENSORY_POTION_SYMBOL, SENSORY_POTION_TYPE, SENSORY_POTION_VALUE, 0, 0, 0, 0, 0, SENSORY_POTION_HP, SENSORY_POTION_COLOR);
} */

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
        return initSmokeBomb();
    
    // 10% chance of generating a fire bomb
    if (r < 40)
        return initFireBomb();
    
    // 10% chance of generating a sword
    if (r < 50)
        return initRock(); // TODO: initSword();
    
    // 10% chance of generating a pot of gold
    if (r < 60)
        return initPotOfGold();
    
    // 10% chance of generating a potion of healing
    if (r < 70)
        return initPotionOfHealing();
    
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
    attron(COLOR_PAIR(i->color));
    mvaddch(i->y, i->x, i->symbol);
    attroff(COLOR_PAIR(i->color));
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
 * @param map Map
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
 * 
 * TODO: use cell->item_index instead of x, y
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
    // free(name); do not because it is a string literal
    free(i);
}
