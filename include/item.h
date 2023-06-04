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
 * @param x the x coordinate of the item
 * @param y the y coordinate of the item
 * @param damage the damage of the item if it is a weapon
 * @param defense the defense of the item if it is armor
 * @param hp the hp of the item if it is a healing potion
 * @param count the number of the same items
 * @param range the range of the item if it is a projectile
 * @param color the color of the item
*/
typedef struct item {
    char *name;
    char symbol;
    int type; // 0: body weapon, 1: projectile weapon, 2: armor, 3: potion, 4: gold
    int value;
    unsigned int x;
    unsigned int y;
    int damage;
    int defense;
    int hp;
    int count;
    int range;
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
#define ROCK_VALUE 1
#define ROCK_DAMAGE_MIN 5
#define ROCK_DAMAGE_MAX 7
#define ROCK_SYMBOL ':'
#define ROCK_TYPE PROJECTILE
#define ROCK_RANGE 7 // cells
#define ROCK_COLOR COLOR_CYAN

// Smoke bomb
#define SMOKE_BOMB_VALUE 30
#define SMOKE_BOMB_DAMAGE_MIN 10
#define SMOKE_BOMB_DAMAGE_MAX 20
#define SMOKE_BOMB_SYMBOL '*'
#define SMOKE_BOMB_TYPE PROJECTILE
#define SMOKE_BOMB_RANGE 5 // cells
#define SMOKE_BOMB_COLOR COLOR_WHITE
#define SMOKE_BOMB_EFFECT 1
#define SMOKE_BOMB_DURATION 5 // turns

// Fire bomb
#define FIRE_BOMB_VALUE 50
#define FIRE_BOMB_DAMAGE_MIN 15
#define FIRE_BOMB_DAMAGE_MAX 25
#define FIRE_BOMB_SYMBOL '*'
#define FIRE_BOMB_TYPE PROJECTILE
#define FIRE_BOMB_RANGE 6 // cells
#define FIRE_BOMB_COLOR COLOR_RED
#define FIRE_BOMB_EFFECT 2
#define FIRE_BOMB_EFFECT_DAMAGE 5 // hp
#define FIRE_BOMB_DURATION 5 // turns

// Ice bomb
#define ICE_BOMB_VALUE 70
#define ICE_BOMB_DAMAGE_MIN 20
#define ICE_BOMB_DAMAGE_MAX 30
#define ICE_BOMB_SYMBOL '*'
#define ICE_BOMB_TYPE PROJECTILE
#define ICE_BOMB_RANGE 7 // cells
#define ICE_BOMB_COLOR COLOR_CYAN
#define ICE_BOMB_EFFECT 3
#define ICE_BOMB_DURATION 7 // turns

// Iron sword
#define IRON_SWORD_VALUE 20
#define IRON_SWORD_DAMAGE_MIN 10
#define IRON_SWORD_DAMAGE_MAX 15
#define IRON_SWORD_SYMBOL 'l'
#define IRON_SWORD_TYPE WEAPON
#define IRON_SWORD_COLOR COLOR_WHITE

// Gold sword
#define GOLD_SWORD_VALUE 40
#define GOLD_SWORD_DAMAGE_MIN 10
#define GOLD_SWORD_DAMAGE_MAX 25
#define GOLD_SWORD_SYMBOL 'l'
#define GOLD_SWORD_TYPE WEAPON
#define GOLD_SWORD_COLOR COLOR_YELLOW

// Diamond sword
#define DIAMOND_SWORD_VALUE 60
#define DIAMOND_SWORD_DAMAGE_MIN 20
#define DIAMOND_SWORD_DAMAGE_MAX 40
#define DIAMOND_SWORD_SYMBOL 'l'
#define DIAMOND_SWORD_TYPE WEAPON
#define DIAMOND_SWORD_COLOR COLOR_CYAN

// Leather armor
#define LEATHER_ARMOR_VALUE 20
#define LEATHER_ARMOR_DEFENSE 5
#define LEATHER_ARMOR_SYMBOL 'T'
#define LEATHER_ARMOR_TYPE ARMOR
#define LEATHER_ARMOR_COLOR COLOR_WHITE

// Chainmail armor
#define CHAINMAIL_ARMOR_VALUE 40
#define CHAINMAIL_ARMOR_DEFENSE 10
#define CHAINMAIL_ARMOR_SYMBOL 'T'
#define CHAINMAIL_ARMOR_TYPE ARMOR
#define CHAINMAIL_ARMOR_COLOR COLOR_CYAN

// Plate armor
#define PLATE_ARMOR_VALUE 60
#define PLATE_ARMOR_DEFENSE 15
#define PLATE_ARMOR_SYMBOL 'T'
#define PLATE_ARMOR_TYPE ARMOR
#define PLATE_ARMOR_COLOR COLOR_MAGENTA

// Potion of healing
#define POTION_OF_HEALING_SYMBOL '!'
#define POTION_OF_HEALING_TYPE POTION
#define POTION_OF_HEALING_VALUE 20
#define POTION_OF_HEALING_HP 50
#define POTION_OF_HEALING_COLOR COLOR_MAGENTA

// Sensory potion
#define SENSORY_POTION_SYMBOL '!'
#define SENSORY_POTION_TYPE POTION
#define SENSORY_POTION_VALUE 50
#define SENSORY_POTION_VISION_RANGE 10 // cells
#define SENSORY_POTION_VISION_WIDTH 3 // adjacent cells
#define SENSORY_POTION_EARING 7
#define SENSORY_POTION_TURNS 60 // turns
#define SENSORY_POTION_RANGE 3 // cells
#define SENSORY_POTION_COLOR COLOR_BLUE

// Potion of invincibility
#define POTION_OF_INVINCIBILITY_SYMBOL '!'
#define POTION_OF_INVINCIBILITY_TYPE POTION
#define POTION_OF_INVINCIBILITY_VALUE 100
#define POTION_OF_INVINCIBILITY_TURNS 40 // turns
#define POTION_OF_INVINCIBILITY_DEFENSE 100 // MAX_DEFENSE
#define POTION_OF_INVINCIBILITY_COLOR COLOR_YELLOW

// TODO: Potion of strength
// TODO: Potion of defense

// Pot of gold
#define POT_OF_GOLD_SYMBOL 'O'
#define POT_OF_GOLD_TYPE GOLD
#define POT_OF_GOLD_VALUE_MIN 1 // * 10
#define POT_OF_GOLD_VALUE_MAX 10 // * 10
#define POT_OF_GOLD_COLOR COLOR_YELLOW

Item** generateItems(int n);

void drawItem(Item *item);
void drawAllItems(Item **items, int n);
void drawVisibleItems(Map *map, Item **items, int n);
Item* getItem(Item **items, int nr_items, unsigned int x, unsigned int y);
int insertItem(Item **items, int nr_items, Item *item);
void freeItem(void *i);

#endif
