#include <stdio.h> /// sprintf
#include <stdlib.h>
#include "state.h" // useEquippedItem()
#include "player.h" // useEquippedItem()
#include "inventory.h"
#include "item.h"
#include "menu.h"

/**
 * @brief Initializes a new inventory. Allocates memory for a new inventory and initializes its fields.
 * 
 * @return Inventory* Initialized inventory
 */
Inventory *initInventory() {
    Inventory *i = (Inventory *) malloc(sizeof(Inventory));
    i->equipped_item = NULL; 
    i->items = (Item **) malloc(sizeof(Item *) * INVENTORY_SIZE);
    i->nr_items = 0;
    i->gold = 0;
    return i;
}

/**
 * @brief Adds an item to the inventory.
 * 
 * @details Adds an item to the inventory. If the inventory is full, the item is not added.
 * If the item is a rock, the number of rocks is increased. // free item
 * 
 * @param i Inventory to add item to
 * @param item Item to add
 * @return void
 */
void addItem(Inventory *i, Item *item) {
    if (item->symbol == POT_OF_GOLD_SYMBOL) {
        i->gold += item->value;
    }
    else if (i->nr_items < INVENTORY_SIZE) {
        // TODO iterate through the inventory to check if the item is already there if yes increase the count
        for (int x = 0; x < INVENTORY_SIZE; x++) {
            if (i->items[x] != NULL) {
                if (i->items[x]->symbol == item->symbol) {
                    i->items[x]->count++;
                    // i->nr_items++; // TODO check this
                    return;
                }
            }
        }
        i->items[i->nr_items] = item;
        i->nr_items++;
    }
}

/**
 * @brief Removes an item from the inventory.
 * 
 * @details Removes an item from the inventory. If the item is not in the inventory, nothing happens.
 * 
 * @param i Inventory to remove item from
 * @param item Item to remove
 * @return void
 */
void removeItem(Inventory *i, Item *item) {
    for (int j = 0; j < i->nr_items; j++) {
        if (i->items[j] != NULL) {
            if (i->items[j]->symbol == item->symbol) {
                // TODO remove 1 from count of items
                if (i->items[j]->count > 1) {
                    i->items[j]->count--;
                }
                else {
                    //i->items[j] = NULL;
                    freeItem(i->items[j]); 
                    i->nr_items--;
                }
                break;
            }
        }
    }
}

/**
 * @brief Sells the equipped item.
 * 
 * @details Sells the equipped item. If the equipped item is NULL, nothing happens.
 * 
 * @param i Inventory to sell equipped item
 * @param st State to send menu message (Other option send msg before calling this function)
 * @return void
*/
void sellEquippedItem(State *st, Inventory *i) {
    if (i->equipped_item == NULL) return;
    
    // Menu message
    char *message = (char *) malloc(sizeof(char) * 1024);
    sprintf(message, "You sold \"%s\" for %d gold.", i->equipped_item->name, i->equipped_item->value);
    sendMenuMessage(st, message);
    free(message);

    // Add gold to the inventory
    i->gold += i->equipped_item->value;
    // if the equipped item has counter > 1, decrease the counter
    if (i->equipped_item->count > 1) {
        i->equipped_item->count--;
    }
    else {
        // remove the equipped item from the inventory
        removeItem(i, i->equipped_item);
        i->equipped_item = NULL;
        // i->equiped_index = 0; // TODO check inventory implementation
    }
}


/**
 * @brief Switches the equipped item on a inventory.
 * 
 * @details Switches the equipped item on a inventory.
 * If there is no possible equipable item, nothing happens.
 * 
 * @param i Inventory to switch equipped item
 * @return void
*/
void switchEquippedItem(Inventory *i) {
    // Uses equiped_index to find the next value
    // If the next value is NULL, it goes to the next one
    // the inventory can have all slots to null
    i->equiped_index++;
    if (i->equiped_index >= i->nr_items) {
        i->equiped_index = 0;
    }
    int tries = 0;
    while (i->items[i->equiped_index] == NULL && tries < i->nr_items) {
        i->equiped_index++;
        if (i->equiped_index >= i->nr_items) {
            i->equiped_index = 0;
        }
        tries++;
    }
    if (tries < i->nr_items) {
        i->equipped_item = i->items[i->equiped_index];
    }
    else {
        i->equipped_item = NULL;
    }
}

/**
 * @brief Frees the memory allocated for an inventory.
 * 
 * @details Frees the memory allocated for an inventory.
 * 
 * @param p Inventory to free
 * @return void
*/
void freeInventory(void *p) {
    Inventory *inventory = (Inventory *) p;
    inventory->equipped_item = NULL;
    int i;
    for (i = 0; i < inventory->nr_items; i++) {
        freeItem(inventory->items[i]);
    }
    inventory->nr_items = 0;
    free(inventory);
}

/**
 * @brief Uses the equipped item.
 * 
 * @details Uses the equipped item.
 * If the equipped item is a potion of healing, the player's health is increased.
 * Receives a State struct as argument because needs to update the player, the map and game stats.
 * 
 * @param st State to use the equipped item
 * @return void
*/
void useEquippedItem(State *st) {
    if (st->player->inventory->equipped_item == NULL) return;

    if (st->player->inventory->equipped_item->symbol == ROCK_SYMBOL) {
        sendMenuMessage(st, "You threw a rock.");
        removeItem(st->player->inventory, st->player->inventory->equipped_item);
        st->player->inventory->equipped_item = NULL;
        // st->player->inventory->nr_of_rocks--; // deprecated remove item count instead
        // TODO: !!! the Rock will be item like the other ones (Potions, etc: except gol)
        // TODO: combat.h throwRock
        return;
    }
    // TODO check item type if it's necessary
    else if (st->player->inventory->equipped_item->symbol == POTION_OF_HEALING_SYMBOL) {
        // Encrease player's health by POTION_OF_HEALING_HP (50)
        if (st->player->health == st->player->max_health) {
            // dont use the potion
            sendMenuMessage(st, "You are already at full health.");
            return;
        }
        else if (st->player->health + POTION_OF_HEALING_HP > st->player->max_health) {
            st->player->health = st->player->max_health;
            removeItem(st->player->inventory, st->player->inventory->equipped_item);
            st->player->inventory->equipped_item = NULL;
        }
        else {
            st->player->health += POTION_OF_HEALING_HP;
            removeItem(st->player->inventory, st->player->inventory->equipped_item);
            st->player->inventory->equipped_item = NULL;
        }
        sendMenuMessage(st, "You used a potion of healing.");
    }
}
