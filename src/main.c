#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "state.h"
#include "player.h"
#include "position.h"

// TODO change all structs pointers

void drawLight(int x, int y) { // TODO use int radius maybe ?
    // TODO check symbols
    attron(COLOR_PAIR(COLOR_YELLOW));
    mvaddch(x - 1, y - 1, '.' | A_BOLD);
    mvaddch(x - 1, y + 0, '.' | A_BOLD);
    mvaddch(x - 1, y + 1, '.' | A_BOLD);
    mvaddch(x + 0, y - 1, '.' | A_BOLD);
    mvaddch(x + 0, y + 1, '.' | A_BOLD);
    mvaddch(x + 1, y - 1, '.' | A_BOLD);
    mvaddch(x + 1, y + 0, '.' | A_BOLD);
	mvaddch(x + 1, y + 1, '.' | A_BOLD);
    attroff(COLOR_PAIR(COLOR_YELLOW));
}

void drawPlayer(int x, int y, char symbol) {
    attron(COLOR_PAIR(COLOR_WHITE));
    mvaddch(x, y, symbol | A_BOLD);
    attroff(COLOR_PAIR(COLOR_WHITE));
}

void drawState(State *st) {
    Player* player = st->player;
    Position *position = player->position;
    int x = position->x;
    int y = position->y;
    
    drawLight(x, y);
    drawPlayer(x, y, player->symbol);
	move(position->x, position->y);
    refresh();
}

int main () // int argc, char *argv[]
{
    WINDOW *window = initscr();
    if (window == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    int ncols, nrows;
	getmaxyx(window,nrows,ncols);

    srandom(time(NULL));
	start_color();

	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, false);
	keypad(stdscr, true);

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);

    // printw("Hello, world!");

    State *st = newState();

    int input_key;
    while ((input_key = getch()) != 'q' && input_key != 'Q') {
        move(nrows - 1, 0);
		updateState(st, input_key);
        drawState(st);
    }
    
    endwin();
    freeState(st);
    printf("Exited cleanly\n");
    return 0;
}