#ifndef _MENU_H_
#define _MENU_H_

#define MENU_HEIGHT 6
#define MENU_BORDERS_COLOR COLOR_CYAN

#define LEGEND_MENU_WIDTH 35
#define LEGEND_MENU_COLOR COLOR_GREEN

typedef struct state State;

void sendMenuMessage(State *st, char *message);
void generateMenu(State *st);
void legendMenu(State *st);

#endif
