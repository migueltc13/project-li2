#include <stdlib.h>
#include <ncurses.h>
#include "item.h"
#include "map.h"
#include "cell.h"
#include "inventory.h" // macro INVENTORY_SIZE

/**
 * @brief Initialize a new item
 * 
 * @param x Item x coordinate on the map if any
 * @param y Item y coordinate on the map if any
 * @param name Item name
 * @param symbol Item symbol
 * @param type Item type
 * @param value Item value
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
 * @return (Item *) Rock
 */
Item *initRock() {
    int damage = ROCK_DAMAGE_MIN + rand() % (ROCK_DAMAGE_MAX - ROCK_DAMAGE_MIN + 1);
    return initItem("Rock", ROCK_SYMBOL, ROCK_TYPE, ROCK_VALUE, 0, 0, damage, 0, 0, ROCK_COLOR);
}

/**
 * @brief Initialize a new item: Smoke bomb (weapon projectile)
 * 
 * TODO: Implement smoke bomb effect in pathfinding functions to cut the line of sight 
 * @return (Item *) Smoke bomb
 */
Item *initSmokeBomb() {
    int damage = SMOKE_BOMB_DAMAGE_MIN + rand() % (SMOKE_BOMB_DAMAGE_MAX - SMOKE_BOMB_DAMAGE_MIN + 1);
    return initItem("Smoke bomb", SMOKE_BOMB_SYMBOL, SMOKE_BOMB_TYPE, SMOKE_BOMB_VALUE, 0, 0, damage, 0, 0, SMOKE_BOMB_COLOR);
}

/**
 * @brief Initialize a new item: Fire bomb (weapon projectile)
 *
 * @return (Item *) Fire bomb
 */
Item *initFireBomb() {
    int damage = FIRE_BOMB_DAMAGE_MIN + rand() % (FIRE_BOMB_DAMAGE_MAX - FIRE_BOMB_DAMAGE_MIN + 1);
    return initItem("Fire bomb", FIRE_BOMB_SYMBOL, FIRE_BOMB_TYPE, FIRE_BOMB_VALUE, 0, 0, damage, 0, 0, FIRE_BOMB_COLOR);
}

/**
 * @brief Initialize a new item: Ice bomb (weapon projectile)
 * 
 * @return (Item *) Ice bomb
 */
Item *initIceBomb() {
    int damage = ICE_BOMB_DAMAGE_MIN + rand() % (ICE_BOMB_DAMAGE_MAX - ICE_BOMB_DAMAGE_MIN + 1);
    return initItem("Ice bomb", ICE_BOMB_SYMBOL, ICE_BOMB_TYPE, ICE_BOMB_VALUE, 0, 0, damage, 0, 0, ICE_BOMB_COLOR);
}

/**
 * @brief Initialize a new item: Iron Sword (weapon body)
 * 
 * @return (Item *) Iron Sword
 */
Item *initIronSword() {
    int attack = IRON_SWORD_DAMAGE_MIN + rand() % (IRON_SWORD_DAMAGE_MAX - IRON_SWORD_DAMAGE_MIN + 1);
    return initItem("Iron Sword", IRON_SWORD_SYMBOL, IRON_SWORD_TYPE, IRON_SWORD_VALUE, 0, 0, attack, 0, 0, IRON_SWORD_COLOR);
}

/**
 * @brief Initialize a new item: Gold Sword (weapon body)
 * 
 * @return (Item *) Gold Sword
 */
Item *initGoldSword() {
    int attack = GOLD_SWORD_DAMAGE_MIN + rand() % (GOLD_SWORD_DAMAGE_MAX - GOLD_SWORD_DAMAGE_MIN + 1);
    return initItem("Gold Sword", GOLD_SWORD_SYMBOL, GOLD_SWORD_TYPE, GOLD_SWORD_VALUE, 0, 0, attack, 0, 0, GOLD_SWORD_COLOR);
}

/**
 * @brief Initialize a new item: Diamond Sword (weapon body)
 *
 * @return (Item *) Diamond Sword
 */
Item *initDiamondSword() {
    int attack = DIAMOND_SWORD_DAMAGE_MIN + rand() % (DIAMOND_SWORD_DAMAGE_MAX - DIAMOND_SWORD_DAMAGE_MIN + 1);
    return initItem("Diamond Sword", DIAMOND_SWORD_SYMBOL, DIAMOND_SWORD_TYPE, DIAMOND_SWORD_VALUE, 0, 0, attack, 0, 0, DIAMOND_SWORD_COLOR);
}

/**
 * @brief Initialize a new item: Leather armor (armor)
 * 
 * @return (Item *) Leather armor 
 */
Item *initLeatherArmor() {
    return initItem("Leather Armor", LEATHER_ARMOR_SYMBOL, LEATHER_ARMOR_TYPE, LEATHER_ARMOR_VALUE, 0, 0, 0, LEATHER_ARMOR_DEFENSE, 0, LEATHER_ARMOR_COLOR);
}

/**
 * @brief Initialize a new item: Chainmail armor (armor)
 * 
 * @return (Item *) Chainmail armor 
 */
Item *initChainmailArmor() {
    return initItem("Chainmail Armor", CHAINMAIL_ARMOR_SYMBOL, CHAINMAIL_ARMOR_TYPE, CHAINMAIL_ARMOR_VALUE, 0, 0, 0, CHAINMAIL_ARMOR_DEFENSE, 0, CHAINMAIL_ARMOR_COLOR);
}

/**
 * @brief Initialize a new item: Plate armor (armor)
 * 
 * @return (Item *) Plate armor 
 */
Item *initPlateArmor() {
    return initItem("Plate Armor", PLATE_ARMOR_SYMBOL, PLATE_ARMOR_TYPE, PLATE_ARMOR_VALUE, 0, 0, 0, PLATE_ARMOR_DEFENSE, 0, PLATE_ARMOR_COLOR);
}

/**
 * @brief Initialize a new item: Potion of healing
 * 
 * @return (Item *) Potion of healing
 */
Item *initPotionOfHealing() {
    return initItem("Potion of Healing", POTION_OF_HEALING_SYMBOL, POTION_OF_HEALING_TYPE, POTION_OF_HEALING_VALUE, 0, 0, 0, 0, POTION_OF_HEALING_HP, POTION_OF_HEALING_COLOR);
}

/**
 * @brief Initialize a new item: Sensory Potion
 *
 * TODO: Implement sensory potion width effect
 * @return (Item *) Sensory Potion
 */
Item *initSensoryPotion() {
    return initItem("Sensory Potion", SENSORY_POTION_SYMBOL, SENSORY_POTION_TYPE, SENSORY_POTION_VALUE, 0, 0, 0, 0, 0, SENSORY_POTION_COLOR);
}

/**
 * @brief Initialize a new item: Potion of invincibility
 * 
 * @return (Item *) Potion of invincibility
 */
Item *initPotionOfInvincibility() {
    return initItem("Potion of Invincibility", POTION_OF_INVINCIBILITY_SYMBOL, POTION_OF_INVINCIBILITY_TYPE, POTION_OF_INVINCIBILITY_VALUE, 0, 0, 0, 0, 0, POTION_OF_INVINCIBILITY_COLOR);
}

/**
 * @brief Initialize a new item: Pot of gold
 * 
 * @details Pot of gold is a special item that can be used to bribe the gate keeper.
 * @details It's value is between 10 and 100 gold.
 * @details It's added as gold to the player's inventory.
 * 
 * @return (Item *) Pot of gold
 */
Item *initPotOfGold() {
    int value = POT_OF_GOLD_VALUE_MIN * 10 + (rand() % (POT_OF_GOLD_VALUE_MAX - POT_OF_GOLD_VALUE_MIN + 1)) * 10;
    return initItem("Pot of Gold", POT_OF_GOLD_SYMBOL, POT_OF_GOLD_TYPE, value, 0, 0, 0, 0, 0, POT_OF_GOLD_COLOR);
}

// ----------------

/**
 * @brief Generate a item randomly based on the probability of each item
 * 
 * TODO: Global variables for the probability of each item maybe
 * @return Item* Item generated
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
    
    // 5% chance of generating a iron sword
    if (r < 45)
        return initIronSword();
    
    // 4% chance of generating a gold sword
    if (r < 49)
        return initGoldSword();
    
    // 1% chance of generating a diamond sword
    if (r < 50)
        return initDiamondSword();

    // 10% chance of generating a pot of gold
    if (r < 60)
        return initPotOfGold();
    
    // 10% chance of generating a potion of healing
    if (r < 70)
        return initPotionOfHealing();
    
    // 10% chance of generating a sensory potion
    if (r < 80)
        return initSensoryPotion();

    // 5% chance of generating a leather armor
    if (r < 85)
        return initLeatherArmor();
    
    // 3% chance of generating a chainmail armor
    if (r < 88)
        return initChainmailArmor();
    
    // 2% chance of generating a plate armor
    if (r < 90)
        return initPlateArmor();
    
    // 5% chance of generating a ice bomb
    if (r < 95)
        return initIceBomb();

    // 5% chance of generating a potion of invincibility
    else
        return initPotionOfInvincibility();
}

/**
 * @brief Generate n *random* items
 * 
 * @details Used to generate items to distribute on the map.
 * @details *random* items are generated based on each item probability.
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
            items[nr_items - 1] = NULL;
            return item;
        }
    }
    return NULL;
}

/**
 * @brief insert item into the items array (inventory)
 *
 * 
 * @return int the number of items in the array 
*/
int insertItem(Item **items, int nr_items, Item *item) {
    // Copy item to the first empty slot in the array
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (items[i] == NULL) {
            items[i] = item;
            nr_items++;
            return nr_items;
        }
    }
    return nr_items;
}

/** 
 * @brief Free an item
 * 
 * @param i Item to free
 * @return void
 */
void freeItem(void *i) {
    // free(name); commented because it's a string literal
    free(i);
}
