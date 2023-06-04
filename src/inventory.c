#include <stdio.h> // snprintf
#include <stdlib.h>
#include <ncurses.h> // colors
#include "state.h" // useEquippedItem()
#include "player.h" // useEquippedItem()
#include "inventory.h"
#include "item.h"
#include "combat.h" // projectiles
#include "map.h"
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
    i->equiped_index = 0;
    i->equipped_sword = NULL;
    i->equipped_armor = NULL;
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
                if (i->items[x]->symbol == item->symbol && i->items[x]->color == item->color) {
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
            if (i->items[j]->symbol == item->symbol && i->items[j]->color == item->color) {
                // TODO remove 1 from count of items
                if (i->items[j]->count > 1) {
                    i->items[j]->count--;
                }
                else {
                    // left shift the items
                    for (int x = j; x < i->nr_items - 1; x++) {
                        i->items[x] = i->items[x + 1];
                    }
                    i->items[i->nr_items - 1] = NULL;
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
    
    // Confirm message
    char *confirm_message = (char *) malloc(sizeof(char) * st->map->width);
    snprintf(confirm_message, st->map->width, "Do you want to sell \"%s\" for %d gold? (y/n)", i->equipped_item->name, i->equipped_item->value);
    sendMenuMessage(st, confirm_message);
    free(confirm_message);
    char input = getch();
    if (input != 'y') { sendMenuMessage(st, ""); return; }

    // Menu message
    char *message = (char *) malloc(sizeof(char) * st->map->width);
    snprintf(message, st->map->width, "You sold \"%s\" for %d gold.", i->equipped_item->name, i->equipped_item->value);
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
        switchEquippedItem(i); // switch to the next item
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
    Item *equipped_item = st->player->inventory->equipped_item;
    if (equipped_item == NULL) return;
    else if (equipped_item->type == PROJECTILE)
    {
        if (equipped_item->symbol == ROCK_SYMBOL) {
            sendMenuMessage(st, "You threw a rock.");
            throwRock(st);
            // Remove the rock from the inventory
            // TODO: remove 1 from count of items, then if count == 0 remove item
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
        }
        else if (equipped_item->symbol == SMOKE_BOMB_SYMBOL
            && equipped_item->color == SMOKE_BOMB_COLOR) {
            sendMenuMessage(st, "You threw a smoke bomb.");
            throwProjectile(st);
            // Remove the smoke bomb from the inventory
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
        }
        else if (equipped_item->symbol == FIRE_BOMB_SYMBOL
            && equipped_item->color == FIRE_BOMB_COLOR) {
            sendMenuMessage(st, "You threw a fire bomb.");
            throwProjectile(st);
            // Remove the fire bomb from the inventory
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
        }
        else if (equipped_item->symbol == ICE_BOMB_SYMBOL
            && equipped_item->color == ICE_BOMB_COLOR) {
            sendMenuMessage(st, "You threw an ice bomb.");
            throwProjectile(st);
            // Remove the ice bomb from the inventory
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
        }
    }
    else if (equipped_item->type == POTION) {
        if (equipped_item->symbol == POTION_OF_HEALING_SYMBOL &&
            equipped_item->color  == POTION_OF_HEALING_COLOR) 
        {
            // Encrease player's health by POTION_OF_HEALING_HP (50)
            if (st->player->health == st->player->max_health) {
                // dont use the potion
                sendMenuMessage(st, "You are already at full health.");
            }
            else {
                if (st->player->health + POTION_OF_HEALING_HP > st->player->max_health) {
                    st->player->health = st->player->max_health;
                    removeItem(st->player->inventory, equipped_item);
                    equipped_item = NULL;
                }
                else {
                    st->player->health += POTION_OF_HEALING_HP;
                    removeItem(st->player->inventory, equipped_item);
                    equipped_item = NULL;
                }
            }
            sendMenuMessage(st, "You used a potion of healing.");
        }
        else if (equipped_item->symbol == SENSORY_POTION_SYMBOL &&
                 equipped_item->color  == SENSORY_POTION_COLOR) 
        {
            // Encrease player's vision (and earing TODO)
            st->player->vision += SENSORY_POTION_VISION_RANGE;
            st->player->vision_width = SENSORY_POTION_VISION_WIDTH;
            st->player->sensory_potion_turns = SENSORY_POTION_TURNS;
            st->player->color = SENSORY_POTION_COLOR;
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
            sendMenuMessage(st, "You used a sensory potion. You can see and ear further.");
            return;
        }
        else if (equipped_item->symbol == POTION_OF_INVINCIBILITY_SYMBOL &&
                 equipped_item->color  == POTION_OF_INVINCIBILITY_COLOR) 
        {
            // Encrease player's defense by POTION_OF_INVINCIBILITY_DEFENSE (100)
            st->player->defense += POTION_OF_INVINCIBILITY_DEFENSE;
            st->player->potion_invincibility_turns = POTION_OF_INVINCIBILITY_TURNS;
            st->player->color = POTION_OF_INVINCIBILITY_COLOR;
            removeItem(st->player->inventory, equipped_item);
            equipped_item = NULL;
            sendMenuMessage(st, "You used a potion of invincibility. You are invincible for a while.");
        }
    }
    else if (equipped_item->type == WEAPON)
    {
        // If there is a sword equipped, remove the sword attack from the player attack
        if (st->player->inventory->equipped_sword != NULL) {
            st->player->attack -= st->player->inventory->equipped_sword->damage;
            st->player->inventory->nr_items = insertItem(st->player->inventory->items, st->player->inventory->nr_items, st->player->inventory->equipped_sword);
            st->player->inventory->equipped_sword = NULL;
        }

        // Equip the sword
        st->player->inventory->equipped_sword = equipped_item;
        // remove the sword from the inventory
        removeItem(st->player->inventory, equipped_item);
        equipped_item = NULL;
        
        // Send menu message
        char *message = (char *) malloc(sizeof(char) * st->map->width);
        snprintf(message, st->map->width, "You equipped the \"%s\".", st->player->inventory->equipped_sword->name);
        sendMenuMessage(st, message);
        free(message);
        
        // Add the sword attack to the player attack
        st->player->attack += st->player->inventory->equipped_sword->damage;
    }
    else if (equipped_item->type == ARMOR)
    {
        if (st->player->inventory->equipped_armor != NULL) {
            st->player->defense -= st->player->inventory->equipped_armor->defense;
            st->player->inventory->nr_items = insertItem(st->player->inventory->items, st->player->inventory->nr_items, st->player->inventory->equipped_armor);
            st->player->inventory->equipped_armor = NULL;
        }

        // Equip the armor
        st->player->inventory->equipped_armor = equipped_item;
        // remove the armor from the inventory
        removeItem(st->player->inventory, equipped_item);
        equipped_item = NULL;

        // Send menu message
        char *message = (char *) malloc(sizeof(char) * st->map->width);
        snprintf(message, st->map->width, "You equipped the \"%s\".", st->player->inventory->equipped_armor->name);
        sendMenuMessage(st, message);
        free(message);

        // Add the armor defense to the player defense
        st->player->defense += st->player->inventory->equipped_armor->defense;
    }
    switchEquippedItem(st->player->inventory);
}
