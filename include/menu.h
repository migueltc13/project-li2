#ifndef _MENU_H_
#define _MENU_H_

#define MENU_HEIGHT 6
#define MENU_BORDERS_COLOR COLOR_BLUE

typedef struct state State;

void generateMenu(State *st);
void sendMenuMessage(State *st, char *message);

#endif
