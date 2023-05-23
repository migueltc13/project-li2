#include <stdlib.h>
#include <ncurses.h>
#include "menu.h"
#include "state.h"
#include "map.h"
#include "player.h"
#include "inventory.h"
#include "item.h"

/**
 * @brief Sends a message to the menu.
 * 
 * @details Sends a message to the menu. The message is displayed on the bottom of the menu.
*/
void sendMenuMessage(State *st, char *message) {
    // clear message
    for (int i = 1; i < st->map->width - 1; i++) {
        mvaddch(st->map->height + MENU_HEIGHT - 2, i, ' ');
    }
    mvprintw(st->map->height + MENU_HEIGHT - 2, 2, "%s", message);
}

/**
 * @brief Generates and display the menu.
 * 
 * @details Fetches the menu information from the player and generates the menu.
 * 
 * @return void
 */
void generateMenu(State *st) {
    // Top border
    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    for (int i = 0; i < st->map->width; i++) {
        mvaddch(st->map->height + MENU_HEIGHT - 6, i, '*');
    }
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));

    // First line -> Player stats
    char *player_stats = (char *) malloc(sizeof(char) * st->map->width);
    sprintf(player_stats, "HP: %d/%d | Gold: %d | Items: %d | Equipped: %s\n",
        st->player->health, st->player->max_health,
        st->player->inventory->gold,
        st->player->inventory->nr_items,
        st->player->inventory->equipped_item == NULL ? "None" : st->player->inventory->equipped_item->name
        );
    mvprintw(st->map->height + MENU_HEIGHT - 5, 2, "%s", player_stats);
    free(player_stats);

    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    mvaddch(st->map->height + MENU_HEIGHT - 5, 0, '*');
    mvaddch(st->map->height + MENU_HEIGHT - 5, st->map->width - 1, '*');
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));

    // Second line -> Player inventory
    char *player_inventory = (char *) malloc(sizeof(char) * st->map->width);
    sprintf(player_inventory, "Inventory");
    // print (name_item) x(item_count) for each item in inventory
    for (int i = 0; i < st->player->inventory->nr_items; i++) {
        sprintf(player_inventory, "%s | %s x%d",
            player_inventory,
            st->player->inventory->items[i]->name,
            st->player->inventory->items[i]->count
            );
    }
    sprintf(player_inventory, "%s\n", player_inventory);
    mvprintw(st->map->height + MENU_HEIGHT - 4, 2, "%s", player_inventory);
    free(player_inventory);

    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    mvaddch(st->map->height + MENU_HEIGHT - 4, 0, '*');
    mvaddch(st->map->height + MENU_HEIGHT - 4, st->map->width - 1, '*');
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));

    // Third line -> Menu: Modes, inventory, combat
    char *menu_options = (char *) malloc(sizeof(char) * st->map->width);
    sprintf(menu_options, "'i' Switch items | 'o' Sell equipped items | 'u' Use equipped item | 'q' Quit\n");
    mvprintw(st->map->height + MENU_HEIGHT - 3, 2, "%s", menu_options);
    free(menu_options);

    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    mvaddch(st->map->height + MENU_HEIGHT - 3, 0, '*');
    mvaddch(st->map->height + MENU_HEIGHT - 3, st->map->width - 1, '*');
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));

    // Fourth line -> Menu message
    // clear message

    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    mvaddch(st->map->height + MENU_HEIGHT - 2, 0, '*');
    mvaddch(st->map->height + MENU_HEIGHT - 2, st->map->width - 1, '*');
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));

    // Bottom border
    attron(COLOR_PAIR(MENU_BORDERS_COLOR));
    for (int i = 0; i < st->map->width; i++) {
        mvaddch(st->map->height + MENU_HEIGHT - 1, i, '*');
    }
    attroff(COLOR_PAIR(MENU_BORDERS_COLOR));
}
