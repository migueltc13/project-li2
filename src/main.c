#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
// #include "window.h"
#include "state.h"

int main() {
    // Window
    WINDOW *window  = initscr();
    if (window == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    int ncols, nrows;
	getmaxyx(window,nrows,ncols);

    srandom(time(NULL)); // Seed random number generator
	start_color(); // Enable color

	cbreak(); // Disable line buffering
	noecho(); // Disable input echoing
	nonl();   // Disable newline translation
	intrflush(stdscr, false); // Disable flushing on interrupt
	keypad(stdscr, true); // Enable keypad input

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);

    // Game state
    State *st = initState(ncols, nrows);
    drawState(st);

    char c;
    while ((c = getch()) != 'q') {
        updateState(st, c);
        drawState(st);
    }
    endwin();
    freeState(st);
    return 0;
}
