#ifndef _ITEM_H_
#define _ITEM_H_

typedef struct map Map;

/**
 * @brief Item structure represents an item in the game
 * 
 * This struct represents an item in the game.
 * It can be a weapon, armor, potion or gold.
 * Items are drawn in the map if the cell is visible.
 * They are picked up by the player when he steps on them.
 * And they are added to the player inventory.
 * 
 * @param name the name of the item
 * @param symbol the symbol that represents the item
 * @param type the type of item
 * @param value the value of the item in gold
 * @param is_picked_up whether the item has been picked up
 * @param x the x coordinate of the item
 * @param y the y coordinate of the item
 * @param damage the damage of the item if it is a weapon
 * @param defense the defense of the item if it is armor
 * @param hp the hp of the item if it is a healing potion
 * @param count the number of the same items
 * @param color the color of the item
*/
typedef struct item {
    char *name;
    char symbol;
    int type; // 0: body weapon, 1: projectile weapon, 2: armor, 3: potion, 4: gold
    int value; // TODO: ability to sell items
    // int is_equipped;
    int is_picked_up;
    unsigned int x;
    unsigned int y;
    int damage;
    int defense;
    int hp;
    int count; // number of the same items
    int color;
} Item;

#define MAX_ITEMS 100 // TODO use this
#define MAX_ITEM_NAME 20 // TODO use this 

// Item types
#define WEAPON 0
#define PROJECTILE 1
#define ARMOR 2
#define POTION 3
#define GOLD 4

// Speficic item macros
// Rock
#define ROCK_VALUE 0
#define ROCK_DAMAGE_MIN 5
#define ROCK_DAMAGE_MAX 7
#define ROCK_SYMBOL ':'
#define ROCK_TYPE 1

// Potion of healing
#define POTION_OF_HEALING_SYMBOL '!'
#define POTION_OF_HEALING_TYPE 3
#define POTION_OF_HEALING_VALUE 20
#define POTION_OF_HEALING_HP 50

// Pot of gold
#define POT_OF_GOLD_SYMBOL 'O'
#define POT_OF_GOLD_TYPE 4
#define POT_OF_GOLD_VALUE_MIN 10
#define POT_OF_GOLD_VALUE_MAX 100

Item** generateItems(int n);

void drawItem(Item *item);
void drawAllItems(Item **items, int n);
void drawVisibleItems(Map *map, Item **items, int n);
Item* getItem(Item **items, int nr_items, unsigned int x, unsigned int y);
void freeItem(void *i);

#endif
