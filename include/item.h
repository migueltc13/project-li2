#ifndef _ITEM_H_
#define _ITEM_H_

/**
 * struct item - represents an item in the game
 * 
 * This struct represents an item in the game.
 * 
 * @name: the name of the item
 * @symbol: the symbol that represents the item
 * @type: the type of item
 * @value: the value of the item in gold
 * @is_picked_up: whether the item has been picked up
 * @x: the x coordinate of the item
 * @y: the y coordinate of the item
 * @damage: the damage of the item if it is a weapon
 * @defense: the defense of the item if it is armor
 * @hp: the hp of the item if it is a potion
*/
typedef struct item {
    char *name;
    char symbol;
    int type; // 0: weapon, 1: armor, 2: potion, 3: gold
    int value; // TODO: ability to sell items
    // int is_equipped;
    int is_picked_up;
    // TODO: color 
    unsigned int x;
    unsigned int y;
    int damage;
    int defense;
    int hp;
} Item;

#endif
