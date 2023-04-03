#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "state.h"
#include "player.h"
#include "position.h"

State newState() {
    State st = malloc(sizeof(struct state));
    Position p = newPosition(0,0);
    st->player = newPlayer('@',p);
    return st;
}

// TODO movement module
void do_movement_action(State st, int dx, int dy) {
	Player player = st->player;
    Position position = player->position;
    position->x += dx;
	position->y += dy;
}

void updateState(State st, int input_key) {

    Player player = st->player;
    Position position = player->position;
    int x = position->x;
    int y = position->y;

	switch(input_key) {
		case KEY_A1:
		case '7': do_movement_action(st, -1, -1); break;
		case KEY_UP:
		case '8': do_movement_action(st, -1, +0); break;
		case KEY_A3:
		case '9': do_movement_action(st, -1, +1); break;
		case KEY_LEFT:
		case '4': do_movement_action(st, +0, -1); break;
		case KEY_B2:
		case '5': break;
		case KEY_RIGHT:
		case '6': do_movement_action(st, +0, +1); break;
		case KEY_C1:
		case '1': do_movement_action(st, +1, -1); break;
		case KEY_DOWN:
		case '2': do_movement_action(st, +1, +0); break;
		case KEY_C3:
		case '3': do_movement_action(st, +1, +1); break;
		case 'q': endwin(); exit(0); break;
	}
    mvaddch(x, y, player->symbol);
}

void freeState(State st) {
    freePlayer(st->player);
    free(st);
}
