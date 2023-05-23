#include <stdlib.h>
#include <ncurses.h>
#include "menu.h"
#include "state.h"
#include "map.h"
#include "player.h"
#include "inventory.h"
#include "item.h"
#include "monster.h"

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
    sprintf(menu_options, "'i' Switch items | 'o' Sell equipped items | 'u' Use equipped item | 'l' Legend menu | 'q' Quit\n");
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

void legendMenu(State *st) {
    WINDOW *legend_menu = NULL;
    // Create a new ncurses window for the legend menu
    int start_height, start_width;
    int lm_height = st->map->height; // + MENU_HEIGHT;
    int lm_width = st->map->width > 40 ? 40 : st->map->width; // 40: MAX_LMENU_WIDTH 
    if ((signed) st->player->x + lm_width >= (signed) st->map->width) {
        start_height = 0;
        start_width = 0;
        legend_menu = newwin(lm_height, lm_width, start_height, start_width);
    }
    else {
        start_height = 0;
        start_width = st->map->width - lm_width;
        legend_menu = newwin(lm_height, lm_width, start_height, start_width);
    }

    start_color();

    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);

    // Print the legend menu content
    wprintw(legend_menu, "\n");
    wprintw(legend_menu, "            Legend Menu\n");
    wprintw(legend_menu, "  -------------------------------\n");
    wprintw(legend_menu, "  Key   Description\n");
    wprintw(legend_menu, "  -------------------------------\n");
    wprintw(legend_menu, "  move  Numpad & arrows & 'WASD'\n");
    wprintw(legend_menu, "  i     Switch inventory items\n");
    wprintw(legend_menu, "  o     Sell equipped items\n");
    wprintw(legend_menu, "  u     Use equipped item\n");
    wprintw(legend_menu, "  l     Open this legend menu\n");
    wprintw(legend_menu, "  q     Quit\n");
    
    wprintw(legend_menu, "  -------------------------------\n");
    wprintw(legend_menu, "  Items legend & stats\n");
    wprintw(legend_menu, "  -------------------------------\n");
    
    wattron(legend_menu, COLOR_PAIR(ROCK_COLOR));
    wprintw(legend_menu, "  %c", ROCK_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(ROCK_COLOR));
    wprintw(legend_menu, " Rock: %d-%d damage\n", ROCK_DAMAGE_MIN, ROCK_DAMAGE_MAX);

    wattron(legend_menu, COLOR_PAIR(POTION_OF_HEALING_COLOR));
    wprintw(legend_menu, "  %c", POTION_OF_HEALING_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(POTION_OF_HEALING_COLOR));
    wprintw(legend_menu, " Potion of Healing: +%d health\n", POTION_OF_HEALING_HP);
    
    // wprintw(legend_menu, "Potion of Strength: +%d strength\n", POTION_OF_STRENGTH_STR);
    // wprintw(legend_menu, "Potion of Defense: +%d defense\n", POTION_OF_DEFENSE_DEF);
    // wprintw(legend_menu, "Sensory Potion: +%d vison +%d earing\n", SENSORY_POTION_VISION, SENSORY_POTION_EARING);
    // TODO: projectiles, weapons, armor

    wattron(legend_menu, COLOR_PAIR(POT_OF_GOLD_COLOR));
    wprintw(legend_menu, "  %c", POT_OF_GOLD_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(POT_OF_GOLD_COLOR));
    wprintw(legend_menu, " Pot of Gold: %d-%d gold\n", POT_OF_GOLD_VALUE_MIN, POT_OF_GOLD_VALUE_MAX);

    // monsters legend
    wprintw(legend_menu, "  -------------------------------\n");
    wprintw(legend_menu, "  Monsters legend and stats\n");
    wprintw(legend_menu, "  -------------------------------\n");

    wattron(legend_menu, COLOR_PAIR(RAT_COLOR));
    wprintw(legend_menu, "  %c", RAT_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(RAT_COLOR));
    // TODO: add monster stats
    wprintw(legend_menu, " Rat:    %d atk %d def %d hp TODO\n", 0, 0, 0);

    wattron(legend_menu, COLOR_PAIR(GOBLIN_COLOR));
    wprintw(legend_menu, "  %c", GOBLIN_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(GOBLIN_COLOR));
    // TODO: add monster stats
    wprintw(legend_menu, " Goblin: %d atk %d def %d hp TODO\n", 0, 0, 0);

    wattron(legend_menu, COLOR_PAIR(ORC_COLOR));
    wprintw(legend_menu, "  %c", ORC_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(ORC_COLOR));
    // TODO: add monster stats
    wprintw(legend_menu, " Orc:    %d atk %d def %d hp TODO\n", 0, 0, 0);

    wattron(legend_menu, COLOR_PAIR(TROLL_COLOR));
    wprintw(legend_menu, "  %c", TROLL_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(TROLL_COLOR));
    // TODO: add monster stats
    wprintw(legend_menu, " Troll:  %d atk %d def %d hp TODO\n", 0, 0, 0);

    wattron(legend_menu, COLOR_PAIR(DRAGON_COLOR));
    wprintw(legend_menu, "  %c", DRAGON_SYMBOL);
    wattroff(legend_menu, COLOR_PAIR(DRAGON_COLOR));
    // TODO: add monster stats
    wprintw(legend_menu, " Dragon: %d atk %d def %d hp TODO\n", 0, 0, 0);

    wprintw(legend_menu, "  -------------------------------\n");
    wprintw(legend_menu, "  Press any key to close...");

    // Print the legend menu borders
    wattron(legend_menu, COLOR_PAIR(MENU_BORDERS_COLOR));
    for (int i = 0; i < lm_width; i++) {
        mvwaddch(legend_menu, 0, i, '*');
        mvwaddch(legend_menu, lm_height - 1, i, '*');
    }
    for (int i = 0; i < lm_height; i++) {
        mvwaddch(legend_menu, i, 0, '*');
        mvwaddch(legend_menu, i, lm_width - 1, '*');
    }
    wattroff(legend_menu, COLOR_PAIR(MENU_BORDERS_COLOR));

    // Refresh the legend menu window to show the content
    wrefresh(legend_menu);

    // Wait for any key press to close the legend menu
    getch();

    // Delete the legend menu window and return to the main window
    delwin(legend_menu);
    refresh();
}
