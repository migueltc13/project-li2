#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#define INVENTORY_SIZE 10

typedef struct item Item;
typedef struct state State;


/** 
 * @brief Inventory structure represents the player inventory
 * 
 * @details This struct represents the player inventory.
 * It contains the items the player has picked up,
 *  the equipped item and is index in the inventory,
 *  and the amount of gold.
 * 
 * @param items: the items in the inventory
 * @param nr_items: the number of items in the inventory
 * @param equipped_item: the equipped item
 * @param equiped_index: the index of the equipped item
 * @param gold: the amount of gold in the inventory
*/
typedef struct inventory {
    Item **items;
    int nr_items;

    Item *equipped_item;
    int equiped_index;

    int gold;
    // ...
} Inventory;

Inventory *initInventory();
void addItem(Inventory *i, Item *item);
// void equipItem(Inventory *i, Item *item); // removed deprecated
void removeItem(Inventory *i, Item *item);
void sellEquippedItem(State *st, Inventory *i);
void switchEquippedItem(Inventory *i);
void freeInventory(void *p);
void useEquippedItem(State *st);

#endif
